#include "GpThread.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../Types/Strings/GpStringUtils.hpp"
#include "../Types/Strings/GpStringOps.hpp"

namespace GPlatform {

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
    iThread = std::jthread([&](GpThreadStopToken aStopToken) noexcept
    {
        GpRunnable::SP runnable = iRunnable;
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
                    iRunnable.Vn().CVF().WakeupAll();
                }
            }

            iThread.join();
        } catch (const GpException& e)
        {
            GpStringUtils::SCerr("[GpThread::Join]: "_sv + e.what() + "\n"_sv);
        } catch (const std::exception& e)
        {
            GpStringUtils::SCerr("[GpThread::Join]: "_sv + e.what() + "\n"_sv);
        } catch (...)
        {
            GpStringUtils::SCerr("[GpThread::Join]: Unknown exception\n"_sv);
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
            try
            {
                iRunnable.Vn().CVF().WakeupAll();
            } catch (const GpException& e)
            {
                GpStringUtils::SCerr("[GpThread::RequestStop]: "_sv + e.what() + "\n"_sv);
            } catch (const std::exception& e)
            {
                GpStringUtils::SCerr("[GpThread::RequestStop]: "_sv + e.what() + "\n"_sv);
            } catch (...)
            {
                GpStringUtils::SCerr("[GpThread::RequestStop]: Unknown exception\n"_sv);
            }
        }
    }

    return res;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
