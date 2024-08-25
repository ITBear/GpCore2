#include <GpCore2/GpUtils/Threads/GpThread.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Threads/GpSleepStrategy.hpp>

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
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    // Check if started
    THROW_COND_GP
    (
        iRunnable.IsNULL(),
        "Already run"_sv
    );

    iThreadRunnableDoneF.clear();
    iRunnable = std::move(aRunnable);

#if defined(GP_USE_MULTITHREADING_IMPL_STD_THREAD)
    iThread = std::thread
    (
        [
            _runnable               = iRunnable,
            _threadName             = std::string(Name()),
            _stopRequest            = &iThreadStopRequestF,
            _threadRunnableDoneF    = &iThreadRunnableDoneF
        ]() noexcept
        {
            _threadRunnableDoneF->clear();

            SSetSysNameForCurrent(_threadName);
            GpRunnable::SP _r = std::move(_runnable);
            _r->Run(*_stopRequest);

            _threadRunnableDoneF->test_and_set();
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
        GpUniqueLock<GpMutex> uniqueLock{iMutex};

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
            std::pair<size_t, std::chrono::milliseconds>{10000, std::chrono::milliseconds(0)},
            std::pair<size_t, std::chrono::milliseconds>{100, std::chrono::milliseconds(1)}
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
            GpUniqueLock<GpMutex> uniqueLock{iMutex};

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

    // TODO: find out why SetThreadDescription wont compile

    //const std::string name(aName);
    //[[maybe_unused]] const HRESULT res = SetThreadDescription
    //(
    //  GetCurrentThread(),
    //  std::data(name)
    //);
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
