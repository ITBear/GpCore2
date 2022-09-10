#include "GpThread.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpThread::GpThread (void) noexcept
{
}

GpThread::GpThread (std::string aName) noexcept:
iName(std::move(aName))
{
}

GpThread::GpThread (GpThread&& aThread) noexcept
{
    //std::scoped_lock lock(iRWLock, aThread.iRWLock);

    iName       = std::move(aThread.iName);
    iThread     = std::move(aThread.iThread);
    iRunnable   = std::move(aThread.iRunnable);
    iThreadId   = std::move(aThread.iThreadId);
}

GpThread::~GpThread (void) noexcept
{
    RequestStop();
}

std::thread::id GpThread::Run (GpRunnable::SP aRunnable)
{
    {
        std::scoped_lock lock(iRWLock);

        THROW_COND_GP
        (
            iRunnable.IsNULL(),
            "Already run"_sv
        );

        iRunnable = aRunnable;
    }

#if defined(GP_USE_MULTITHREADING_IMPL_JTHREAD)
    iThread = std::jthread([aRunnable](GpThreadStopToken aStopToken) noexcept
    {
        GpRunnable::SP runnable = std::move(aRunnable);
        runnable.V().Run(aStopToken);
    });
#else
#   error Unimplemented
#endif

    {
        std::scoped_lock lock(iRWLock);
        iThreadId = iThread.get_id();
        return iThreadId;
    }
}

void    GpThread::Join (void) noexcept
{
    if (iThread.joinable())
    {
        try
        {
            {
                std::scoped_lock lock(iRWLock);

                if (iRunnable.IsNotNULL())
                {
                    iRunnable.Vn().WakeupAll();
                }
            }

            iThread.join();
        } catch (const std::system_error&)
        {
            //thread was stoped between joinable() and join() calls
        }
    }

    {
        std::scoped_lock lock(iRWLock);

        if (iRunnable.IsNotNULL())
        {
            iRunnable.Clear();
        }
    }
}

bool    GpThread::RequestStop (void) noexcept
{
    const bool res = iThread.request_stop();

    if (res)
    {
        if (iRunnable.IsNotNULL())
        {
            iRunnable.Vn().WakeupAll();
        }
    }

    return res;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
