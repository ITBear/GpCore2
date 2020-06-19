#include "GpThread.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpThread::GpThread (void) noexcept
{
}

GpThread::GpThread (std::string_view aName):
iName(aName)
{
}

GpThread::GpThread (GpThread&& aThread) noexcept
{
    std::scoped_lock lock(iMutex, aThread.iMutex);

    iName		= std::move(aThread.iName);
    iThread		= std::move(aThread.iThread);
    iRunnable	= std::move(aThread.iRunnable);
}

GpThread::~GpThread	(void) noexcept
{
    RequestStop();
}

void	GpThread::Run (GpRunnable::SP aRunnable)
{
    {
        std::scoped_lock lock(iMutex);
        THROW_GPE_COND_CHECK_M(iRunnable.IsNULL(), "Already run"_sv);
        iRunnable = aRunnable;
    }

#if defined(GP_USE_MULTITHREADING_IMPL_JTHREAD)
    iThread = std::jthread([aRunnable](GpThreadStopToken aStopToken) noexcept
    {
        GpRunnable::SP runnable = aRunnable;
        runnable.V().Run(aStopToken);
    });
#else
#	error Unimplemented
#endif
}

bool	GpThread::Joinable (void) const noexcept
{
    return iThread.joinable();
}

void	GpThread::Join (void) noexcept
{
    if (iThread.joinable())
    {
        try
        {
            if (iRunnable.IsNotNULL())
            {
                iRunnable.Vn().WakeupAll();
            }

            iThread.join();
        } catch (const std::system_error&)
        {
            //thread was stoped between joinable() and join() calls
        }
    }
}

bool	GpThread::RequestStop (void) noexcept
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
