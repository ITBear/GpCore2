#include <GpCore2/GpUtils/Debugging/GpStackTrace.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_PUSH()
    GP_WARNING_DISABLE(shadow)
    GP_WARNING_POP()
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#if defined(GP_POSIX)
#   include <execinfo.h>
#   include <dlfcn.h>
#   include <cxxabi.h>
#elif defined(GP_OS_WINDOWS) || defined(GP_OS_BROWSER)
#   include <boost/stacktrace/stacktrace.hpp>
#else
#   error Unsupported platform
#endif

namespace GPlatform {

std::string SDemangle (const char* mangled)
{
    if (!mangled)
    {
        return {};
    }

    int status = 0;
    size_t len = 0;
    char* dem = abi::__cxa_demangle(mangled, nullptr, &len, &status);
    std::string out = (status == 0 && dem) ? dem : mangled;
    std::free(dem);
    return out;
}

std::string GpStackTrace::STraceToStr (void)
{
    std::string stackTraceStr;
    stackTraceStr.reserve(512);
    stackTraceStr.append("Stack trace:");

#if defined(GP_POSIX)
    void*       ptrs[100];
    const int   size = backtrace(ptrs, 100);

    char** stackTracePtrs = backtrace_symbols(ptrs, size);

    for (int id = 0; id < size; id++)
    {
        Dl_info info{};
        const void* addr = ptrs[id];
        std::string line;

        if (::dladdr(const_cast<void*>(addr), &info) && info.dli_sname)
        {
            std::string fn = SDemangle(info.dli_sname);

            const char* mod = info.dli_fname ? info.dli_fname : "?";

            std::uintptr_t sym  = reinterpret_cast<std::uintptr_t>(info.dli_saddr);
            std::uintptr_t pc   = reinterpret_cast<std::uintptr_t>(addr);
            std::uintptr_t off  = (pc >= sym) ? (pc - sym) : 0;

            line = fmt::format("\n    [{}]: {} ({}+0x{:x}) [{:#x}]",
                               size - id, mod, fn, off, pc);
        } else
        {
            line = fmt::format
            (
                "\n    [{}]: {}",
                size - id,
                stackTracePtrs[id]
            );
        }

        stackTraceStr.append(line);
    }

    std::free(stackTracePtrs);
#elif defined(GP_OS_WINDOWS) || defined(GP_OS_BROWSER)
    stackTraceStr = boost::stacktrace::to_string(boost::stacktrace::stacktrace());
#else
#   error Unsupported platform
#endif//

    return stackTraceStr;
}

void    GpStackTrace::SPrintStacktrace (void)
{
    GpOutUtils::S().StdErr(STraceToStr());
}

void    GpStackTrace::SPrintStacktrace (std::string_view aMessage)
{
    GpOutUtils::S().StdErr(aMessage + STraceToStr());
}

}// namespace GPlatform
