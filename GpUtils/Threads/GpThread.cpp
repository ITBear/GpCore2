#include "GpThread.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../Types/Strings/GpStringUtils.hpp"
#include "../Types/Strings/GpStringOps.hpp"

#if defined(GP_OS_LINUX)
    #include <sys/prctl.h>
#endif

namespace GPlatform {

GpThread::GpThread (std::u8string aName) noexcept:
iName(std::move(aName))
{
}

GpThread::~GpThread (void) noexcept
{
    GpStringUtils::SCout(u8"[GpThread::~GpThread]: "_sv + Name());
    RequestStop();
    iThread = {};
}

std::thread::id GpThread::Run (GpRunnable::SP aRunnable)
{
    GpUniqueLock<GpMutex> uniqueLock(iMutex);

    // Check if started
    THROW_COND_GP
    (
        iRunnable.IsNULL(),
        u8"Already run"_sv
    );

    iThreadRunnableDoneF.clear();
    iRunnable = std::move(aRunnable);

#if defined(GP_USE_MULTITHREADING_IMPL_STD_THREAD)

    iThread = std::thread
    (
        [
            _runnable               = iRunnable,
            _threadName             = std::u8string(Name()),
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

    iThread.detach();
#else
#   error Unimplemented
#endif

    iThreadId = iThread.get_id();

    return iThreadId;
}

void    GpThread::RequestStop (void) noexcept
{
    iThreadStopRequestF.test_and_set();

    {
        GpUniqueLock<GpMutex> uniqueLock(iMutex);

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
        while (!iThreadRunnableDoneF.test())
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        {
            GpUniqueLock<GpMutex> uniqueLock(iMutex);

            if (iRunnable.IsNotNULL())
            {
                iRunnable.Clear();
            }
        }
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr(u8"[GpThread::Join]: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr(u8"[GpThread::Join]: "_sv + e.what());
    } catch (...)
    {
        GpStringUtils::SCerr(u8"[GpThread::Join]: Unknown exception"_sv);
    }
}

void    GpThread::SSetSysNameForCurrent (std::u8string_view aName)
{
    if (aName.empty())
    {
        return;
    }

#if defined(GP_OS_WINDOWS)
#   error Need to be implemented
#elif defined(GP_OS_LINUX)
    const std::u8string name(aName.substr(0, NumOps::SMin<std::u8string_view::size_type>(15, aName.size())));

    /*pthread_setname_np
    (
        pthread_self(),
        GpUTF::S_As_STR(name).data()
    );*/

    prctl(PR_SET_NAME, (unsigned long)name.data(), 0, 0, 0);

#elif defined(GP_OS_ANDROID)
    const std::u8string name(aName);

    pthread_setname_np
    (
        pthread_self(),
        GpUTF::S_As_STR(name).data()
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

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
