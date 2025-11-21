#include <GpCore2/GpUtils/Threads/GpThread.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Threads/GpSleepStrategy.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSyncPrimitives.hpp>

#if defined(GP_USE_MULTITHREADING)

#if defined(GP_OS_WINDOWS)
#   include <GpCore2/Config/IncludeExt/windows.hpp>
#endif// #if defined(GP_OS_WINDOWS)

#if defined(GP_OS_LINUX)
#   include <sys/prctl.h>
#endif// #if defined(GP_OS_LINUX)

namespace GPlatform {

GpThread::GpThread (GpConditionVarFlag& aStopFlag) noexcept:
iStopFlag{aStopFlag}
{
    iRunnableDoneFlag.test_and_set(std::memory_order_relaxed);
}

GpThread::GpThread
(
    GpConditionVarFlag& aStopFlag,
    std::string         aName
) noexcept:
iStopFlag{aStopFlag},
iName    {std::move(aName)}
{
    iRunnableDoneFlag.test_and_set(std::memory_order_relaxed);
}

GpThread::~GpThread (void) noexcept
{
    RequestStop();
    Join();
    iThread = {};
}

std::thread::id GpThread::Run (GpRunnable::UP aRunnableUP)
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    // Check if started
    VERIFY
    (
        iThread.get_id() == std::thread::id{},
        "The thread has already started"_sv
    );

    iRunnableDoneFlag.clear();
    iRunnableUP = std::move(aRunnableUP);

#if defined(GP_USE_MULTITHREADING_IMPL_STD_THREAD)
    iThread = std::thread
    (
        [
            runnablePtr         = iRunnableUP.get(),
            name                = std::string{this->Name()},
            stopFlag            = &iStopFlag,
            runnableDoneFlag    = &iRunnableDoneFlag
        ]() mutable noexcept
        {
            runnableDoneFlag->clear();

            SSetSysNameForCurrent(std::move(name));
            runnablePtr->Run(*stopFlag);
            runnableDoneFlag->test_and_set();
        }
    );

    iThreadId = iThread.get_id();
    iThread.detach();
    iThread = {};
#else
#   error Unimplemented
#endif

    return iThreadId;
}

std::thread::id GpThread::Run (std::function<void(GpConditionVarFlag&)> aRunFn)
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    // Check if started
    VERIFY
    (
        iThread.get_id() == std::thread::id{},
        "The thread has already started"_sv
    );

    iRunnableDoneFlag.clear();

#if defined(GP_USE_MULTITHREADING_IMPL_STD_THREAD)
    iThread = std::thread
    (
        [
            runFn               = std::move(aRunFn),
            name                = std::string{this->Name()},
            stopFlag            = &iStopFlag,
            runnableDoneFlag    = &iRunnableDoneFlag
        ]() mutable noexcept
        {
            runnableDoneFlag->clear();

            SSetSysNameForCurrent(std::move(name));
            runFn(*stopFlag);
            runnableDoneFlag->test_and_set();
        }
    );

    iThreadId = iThread.get_id();
    iThread.detach();
    iThread = {};
#else
#   error Unimplemented
#endif

    return iThreadId;
}

void    GpThread::RequestStop (void) noexcept
{
    iStopFlag.UpFlagAndNotifyAll();
}

void    GpThread::Join (void) noexcept
{
    try
    {
        constexpr std::array<std::pair<size_t, std::chrono::milliseconds>, 2> tryStages =
        {
            std::pair<size_t, std::chrono::milliseconds>{size_t{10000}, std::chrono::milliseconds(0)},
            std::pair<size_t, std::chrono::milliseconds>{size_t{100}, std::chrono::milliseconds(1)}
        };

        GpSleepStrategy::SWaitFor
        (
            [&]()-> bool
            {
                return iRunnableDoneFlag.test();
            },
            tryStages,
            std::chrono::milliseconds(10)
        );

        {
            GpUniqueLock uniqueLock{iSpinLockRW};

            if (iRunnableUP)
            {
                iRunnableUP.reset();
            }
        }
    } catch (const GpException& e)
    {
        GpOutUtils::S().Err("[GpThread::Join]: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpOutUtils::S().Err("[GpThread::Join]: "_sv + e.what());
    } catch (...)
    {
        GpOutUtils::S().Err("[GpThread::Join]: Unknown exception"_sv);
    }
}

void    GpThread::SSetSysNameForCurrent (std::string_view aName)
{
    if (aName.empty())
    {
        return;
    }

#if defined(GP_OS_WINDOWS)

GP_WARNING_PUSH()
GP_WARNING_DISABLE_MSVC(4365)

    const std::wstring name{std::begin(aName), std::end(aName)};

GP_WARNING_POP()

    SetThreadDescription
    (
        GetCurrentThread(),
        std::data(name)
    );

#elif defined(GP_OS_LINUX)
    const std::string name(aName.substr(0, NumOps::SMin<std::string_view::size_type>(15, std::size(aName))));
    prctl(PR_SET_NAME, (unsigned long)std::data(name), 0, 0, 0);
#elif defined(GP_OS_ANDROID)
    const std::string name(aName);

    pthread_setname_np
    (
        pthread_self(),
        std::data(name)
    );
#elif defined(GP_OS_IOS)
#   error Need to be implemented
#elif defined(GP_OS_IOS_SIMULATOR)
#   error Need to be implemented
#elif defined(GP_OS_MACOS)
    const std::string name(aName);

    pthread_setname_np(name.c_str());
#elif defined(GP_OS_BARE_METAL)
#   error Need to be implemented
#endif
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
