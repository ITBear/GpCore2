#include <GpCore2/GpTasks/GpTaskThread.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {
/*
GpTaskRunRes::EnumT GpTaskThread::Run (void) noexcept
{
    GpTaskRunRes::EnumT res = GpTaskRunRes::DONE;
    GpException::C::Opt ex;

    // Catch all exceptions
    try
    {
        // Check if OnStart was called
        if (!iIsStartCalled)
        {
            OnStart();

            iIsStartCalled = true;
            StartPromise(GpMethodAccess{this}).Fulfill(StartPromiseRes{});
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
                GpException::C::Opt stopExOpt = CallOnStop();
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
            CallOnStop();
        }
    } catch (const std::exception& e)
    {
        ex = GpException{e.what()};

        if (!iIsStopCalled)
        {
            // TODO: do something with result (logout for example)
            CallOnStop();
        }
    } catch (...)
    {
        ex = GpException{"[GpTaskThread::Run]: unknown exception"_sv};

        if (!iIsStopCalled)
        {
            // TODO: do something with result (logout for example)
            CallOnStop();
        }
    }

    // Check if there are was exception
    if (ex.has_value())
    {
        //GpOutUtils::S().Err(ex->what());
        res = GpTaskRunRes::DONE;

        StartPromise(GpMethodAccess{this}).Fulfill(ex.value());
        DonePromise(GpMethodAccess{this}).Fulfill(ex.value());
    } else if (res == GpTaskRunRes::DONE) // Check if result is DONE
    {
        StartPromise(GpMethodAccess{this}).Fulfill(StartPromiseRes{});
        DonePromise(GpMethodAccess{this}).Fulfill(DonePromiseRes{});
    }

    return res;
}

GpException::C::Opt GpTaskThread::CallOnStop (void) noexcept
{
    iIsStopCalled = true;
    return OnStop();
}
*/
}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
