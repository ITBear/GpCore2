#include <GpCore2/GpUtils/Threads/GpThread.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Threads/GpSleepStrategy.hpp>

#if defined(GP_USE_MULTITHREADING)

#if defined(GP_OS_WINDOWS)
#   include <GpCore2/Config/IncludeExt/windows.hpp>
#endif// #if defined(GP_OS_WINDOWS)

#if defined(GP_OS_LINUX)
#   include <sys/prctl.h>
#endif// #if defined(GP_OS_LINUX)

namespace GPlatform {

GpThread::GpThread (std::string aName) noexcept:
iName{std::move(aName)}
{
    iThreadStopRequestF.clear();
    iThreadRunnableDoneF.test_and_set(std::memory_order_relaxed);
}

GpThread::~GpThread (void) noexcept
{
    RequestStop();
    Join();
    iThread = {};
}

std::thread::id GpThread::Run (GpRunnable::SP aRunnable)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

    // Check if started
    VERIFY
    (
        iRunnable.IsNULL(),
        "The thread has already started"_sv
    );

    iThreadRunnableDoneF.clear();
    iRunnable = std::move(aRunnable);

#if defined(GP_USE_MULTITHREADING_IMPL_STD_THREAD)
    iThread = std::thread
    (
        [
            runnable            = iRunnable,
            name                = std::string{this->Name()},
            threadStopRequestF  = &iThreadStopRequestF,
            threadRunnableDoneF = &iThreadRunnableDoneF
        ]() mutable noexcept
        {
            threadRunnableDoneF->clear();

            SSetSysNameForCurrent(std::move(name));
            runnable->Run(*threadStopRequestF);

            threadRunnableDoneF->test_and_set();
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
    iThreadStopRequestF.test_and_set();

    {
        GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

        if (iRunnable.IsNotNULL())
        {
            iRunnable.Vn().Notify();
        }
    }
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
                return iThreadRunnableDoneF.test();
            },
            tryStages,
            std::chrono::milliseconds(10)
        );

        {
            GpUniqueLock<GpSpinLockRW> uniqueLock{iSpinLockRW};

            if (iRunnable.IsNotNULL())
            {
                iRunnable.Clear();
            }
        }
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr("[GpThread::Join]: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr("[GpThread::Join]: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr("[GpThread::Join]: Unknown exception"_sv);
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

    const std::wstring name{aName.begin(), aName.end()};

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
#elif defined(GP_OS_MACOSX)
#   error Need to be implemented
#elif defined(GP_OS_BARE_METAL)
#   error Need to be implemented
#endif
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
