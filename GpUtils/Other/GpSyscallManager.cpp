#include <GpCore2/GpUtils/Other/GpSyscallManager.hpp>
#include <GpCore2/GpUtils/Other/GpDefer.hpp>
#include <GpCore2/GpUtils/Other/GpErrno.hpp>

#if defined (GP_POSIX)
#   include <spawn.h>
#   include <csignal>
#   include <sys/wait.h>

#   if defined(GP_OS_MACOS)
        extern char** environ;
#   endif// #if defined(GP_OS_MACOS)

#endif// #if defined (GP_POSIX)

namespace GPlatform {

// ----------------------------------------- POSIX -----------------------------------------
#if defined (GP_POSIX)

GpSyscallManager::HandlerT Posix_Syscall
(
    std::string_view            aCommandToCall,
    const GpSpan<std::string>&  aArguments
)
{
    posix_spawn_file_actions_t actions;
    posix_spawn_file_actions_init(&actions);

    GpDefer onDestruct = [&actions]()
    {
        posix_spawn_file_actions_destroy(&actions);
    };

    // Prepare command to call
    const std::string commandToCall{aCommandToCall};

    // Prepare arguments
    std::vector<char*> argumentsPtr;
    argumentsPtr.reserve(std::size(aArguments) + 2);
    argumentsPtr.emplace_back(const_cast<char*>(commandToCall.data()));

    for (const std::string& arg: aArguments.AsStdSpan())
    {
        argumentsPtr.emplace_back(const_cast<char*>(arg.data()));
    }
    argumentsPtr.emplace_back(nullptr);

    // Spawn process
    GpSyscallManager::HandlerT processId = {};

    const int spawnRes = posix_spawnp
    (
        &processId,
        commandToCall.data(),
        &actions,
        nullptr,
        argumentsPtr.data(),
        environ
    );

    if (spawnRes == 0)
    {
        return processId;
    }

    THROW
    (
        fmt::format
        (
            "Failed to start process: {}",
            GpErrno::SGetAndClear()
        )
    );
}

std::optional<int> Posix_TryGetProcessResult (GpSyscallManager::HandlerT aHandler)
{
    int status = {};

    // Non-blocking wait
    const pid_t result = waitpid(aHandler, &status, WNOHANG);

    if (result == 0) // Process is still running
    {
        return std::nullopt;
    } else if (result == aHandler)
    {
        if (WIFEXITED(status)) // Process has terminated with exit code WEXITSTATUS(status)
        {
            return WEXITSTATUS(status);
        } else if (WIFSIGNALED(status)) // Process was terminated by signal WTERMSIG(status)
        {
            return EXIT_FAILURE;
        }
    }

    THROW
    (
        fmt::format
        (
            "Error waiting for process {}: {}",
            aHandler,
            GpErrno::SGetAndClear()
        )
    );
}

void    Posix_SendSignal
(
    GpSyscallManager::HandlerT  aHandler,
    const int                   aSignal
)
{
    // Check process id
    VERIFY
    (
        aHandler > 0,
        "Process ID is 0"
    );

    VERIFY
    (
        kill(aHandler, aSignal) == 0,// Send signal to the process
        [&aSignal, &aHandler]()
        {
            return fmt::format
            (
                "Failed to send signal {} to process {}. {}",
                aSignal,
                aHandler,
                GpErrno::SGetAndClear()
            );
        }
    );
}

#endif// #if defined (GP_POSIX)

// ----------------------------------------- WINDOWS -----------------------------------------
#if defined (GP_OS_WINDOWS)

GpSyscallManager::HandlerT Windows_SpawnProcess
(
    std::string_view            aCommandToCall,
    const GpSpan<std::string>&  aArguments
)
{
    std::ostringstream cmdLine;
    cmdLine << "\"" << aCommandToCall << "\"";

    for (const auto& arg: aArguments.AsStdSpan())
    {
        cmdLine << " \"" << arg << "\"";
    }

    // Convert command line string to wide string
    std::string     commandLine     = cmdLine.str();

GP_WARNING_PUSH()
GP_WARNING_DISABLE_MSVC(4365)

    std::wstring    commandLineW    = std::wstring{std::begin(commandLine), std::end(commandLine)};

GP_WARNING_POP()

    STARTUPINFOW startupInfo = {0};
    startupInfo.cb = sizeof(startupInfo);

    PROCESS_INFORMATION processInfo = {};

    const bool res = CreateProcessW
    (
        nullptr,                    // Application name
        std::data(commandLineW),    // Command line
        nullptr,                    // Process security attributes
        nullptr,                    // Thread security attributes
        FALSE,                      // Handle inheritance
        0,                          // Creation flags
        nullptr,                    // Environment
        nullptr,                    // Current directory
        &startupInfo,               // Startup info
        &processInfo                // Process information
    );

    VERIFY
    (
        res != false,
        [aCommandToCall]()
        {
            return fmt::format
            (
                "Failed to spawn the process '{}'. {}",
                aCommandToCall,
                GpErrno::SWinGetAndClear()
            );
        }
    );

    return processInfo;
}

std::optional<int> Windows_TryGetProcessResult (GpSyscallManager::HandlerT aHandler)
{
    // Non-blocking wait
    DWORD exitCode = {};

    if (!GetExitCodeProcess(aHandler.hProcess, &exitCode))
    {
        const std::string errorMsg = GpErrno::SWinGetAndClear();

        CloseHandle(aHandler.hProcess);
        CloseHandle(aHandler.hThread);

        THROW
        (
            fmt::format
            (
                "Failed to get exit code with error: {}",
                errorMsg
            )
        );
    }

    if (exitCode == STILL_ACTIVE)
    {
        return std::nullopt;
    }

    CloseHandle(aHandler.hProcess);
    CloseHandle(aHandler.hThread);

    return NumOps::SConvert<int>(exitCode);
}

void Windows_TerminateProcess ( GpSyscallManager::HandlerT aHandler)
{
    if (!TerminateProcess(aHandler.hProcess, 1))
    {
        const std::string errorMsg = GpErrno::SWinGetAndClear();

        CloseHandle(aHandler.hProcess);
        CloseHandle(aHandler.hThread);

        THROW
        (
            fmt::format
            (
                "Failed to terminate process, error: {}",
                errorMsg
            )
        );
    }

    CloseHandle(aHandler.hProcess);
    CloseHandle(aHandler.hThread);
}

void Windows_SendCtrlSignal
(
    GpSyscallManager::HandlerT  aHandler,
    DWORD                       aCtrlEvent
)
{
    // Attach to the process's console
    if (!AttachConsole(aHandler.dwProcessId))
    {
        const std::string errorMsg = GpErrno::SWinGetAndClear();

        CloseHandle(aHandler.hProcess);
        CloseHandle(aHandler.hThread);

        THROW
        (
            fmt::format
            (
                "Failed to attach to console, error: {}",
                errorMsg
            )
        );
    }

    // Send the CTRL signal (CTRL+C or CTRL+BREAK)
    if (!GenerateConsoleCtrlEvent(aCtrlEvent, 0))
    {
        const std::string errorMsg = GpErrno::SWinGetAndClear();

        FreeConsole();

        CloseHandle(aHandler.hProcess);
        CloseHandle(aHandler.hThread);

        THROW
        (
            fmt::format
            (
                "Failed to send CTRL signal, error: {}",
                errorMsg
            )
        );
    }

    FreeConsole();

    CloseHandle(aHandler.hProcess);
    CloseHandle(aHandler.hThread);
}

#endif //#if defined (GP_OS_WINDOWS)

// ----------------------------------- GpSyscallManager -----------------------------------

auto GpSyscallManager::SSpawnProcess
(
    std::string_view            aCommandToCall,
    const GpSpan<std::string>&  aArguments
) -> HandlerT
{
#if defined (GP_POSIX)
    return Posix_Syscall(aCommandToCall, aArguments);
#elif defined (GP_OS_WINDOWS)
    return Windows_SpawnProcess(aCommandToCall, aArguments);
#else
#   error Unsupported OS
#endif
}

std::optional<int>  GpSyscallManager::STryGetProcessResult (HandlerT aHandler)
{
#if defined (GP_POSIX)
    return Posix_TryGetProcessResult(aHandler);
#elif defined (GP_OS_WINDOWS)
    return Windows_TryGetProcessResult(aHandler);
#else
#   error Unsupported OS
#endif
}

void    GpSyscallManager::SSendTerminate (HandlerT aHandler)
{
#if defined (GP_POSIX)
    Posix_SendSignal(aHandler, SIGTERM);
#elif defined (GP_OS_WINDOWS)
    Windows_SendCtrlSignal(aHandler, CTRL_C_EVENT);
#else
#   error Unsupported OS
#endif
}

void    GpSyscallManager::SSendKill (HandlerT aHandler)
{
#if defined (GP_POSIX)
    Posix_SendSignal(aHandler, SIGKILL);
#elif defined (GP_OS_WINDOWS)
    Windows_TerminateProcess(aHandler);
#else
#   error Unsupported OS
#endif
}

}// namespace GPlatform
