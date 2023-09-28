#include "GpTaskThread.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskRunRes::EnumT GpTaskThread::Run (void) noexcept
{
    GpTaskRunRes::EnumT         res = GpTaskRunRes::DONE;
    std::optional<GpException>  ex;

    // Catch all exceptions
    try
    {
        // Check if OnStart was called
        if (!iIsStartCalled)
        {
            OnStart();

            iIsStartCalled = true;
            StartPromiseHolder().Fulfill();
        }

        //
        res = GpTaskRunRes::READY_TO_RUN;

        while (   (IsStopRequested() == false)
               && (res == GpTaskRunRes::READY_TO_RUN))
        {
            res = OnStep();
        }

        if (res == GpTaskRunRes::DONE)
        {
            if (!iIsStopCalled)
            {
                std::optional<GpException> stopExOpt = CallStop();
                if (stopExOpt.has_value())
                {
                    throw stopExOpt.value();
                }
            }
        }

        return res;
    } catch (const GpException& e)
    {
        ex = e;

        if (!iIsStopCalled)
        {
            // TODO: do something with result (logout for example)
            CallStop();
        }
    } catch (const std::exception& e)
    {
        ex = GpException(e.what());

        if (!iIsStopCalled)
        {
            // TODO: do something with result (logout for example)
            CallStop();
        }
    } catch (...)
    {
        ex = GpException(u8"[GpTaskThread::Run]: unknown exception"_sv);

        if (!iIsStopCalled)
        {
            // TODO: do something with result (logout for example)
            CallStop();
        }
    }

    // Check if there are was exception
    if (ex.has_value())
    {
        GpStringUtils::SCerr(ex->what());
        res = GpTaskRunRes::DONE;

        StartPromiseHolder().Fulfill(ex.value());
        DonePromiseHolder().Fulfill(ex.value());
    } else if (res == GpTaskRunRes::DONE) // Check if result is DONE
    {
        StartPromiseHolder().Fulfill();
        DonePromiseHolder().Fulfill();
    }

    return res;
}

std::optional<GpException>  GpTaskThread::CallStop (void) noexcept
{
    iIsStopCalled = true;
    return OnStop();
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
