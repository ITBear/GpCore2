#include "GpTaskBase.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskBase::~GpTaskBase (void) noexcept
{
}

GpTaskDoRes GpTaskBase::_Run (GpThreadStopToken aStopToken) noexcept
{
    bool isCatchException = false;

    try
    {
        if (!iIsStarted)
        {
            OnStart();
            iIsStarted = true;
        }

        GpEvent::SP event;
        while (   (aStopToken.stop_requested() == false)
               && (IsStopRequested() == false))
        {
            //const GpTaskDoRes stepRes = OnStep(event.IsNotNULL() ? EventOptRefT(event.Vn()) : std::nullopt);
            //Step
            GpTaskDoRes stepRes;
            {
                if (event.IsNULL())
                {
                    event = PopNextEvent();
                }

                stepRes = OnStep(event.IsNotNULL() ? EventOptRefT(event.Vn()) : std::nullopt);
                event.Clear();
            }

            if (stepRes == GpTaskDoRes::READY_TO_EXEC)
            {
                return GpTaskDoRes::READY_TO_EXEC;
            } else if (stepRes == GpTaskDoRes::WAITING)
            {
                event = PopNextEvent();
                if (event.IsNULL())
                {
                    return GpTaskDoRes::WAITING;
                }
            } else if (stepRes == GpTaskDoRes::DONE)
            {
                break;
            }
        }
    } catch (const GpException& e)
    {
        CompletePromise(MakeSP<CompleteItcResultT>(e));
        isCatchException = true;
    } catch (const std::exception& e)
    {
        CompletePromise(MakeSP<CompleteItcResultT>(e));
        isCatchException = true;
    } catch (...)
    {
        //CompletePromise(MakeSP<CompleteItcResultT>(std::runtime_error("Unknown exception")));
        CompletePromise(MakeSP<CompleteItcResultT>(GpException(u8"Unknown exception")));
        isCatchException = true;
    }

    OnStop();

    if (!isCatchException)
    {
        CompletePromise(MakeSP<CompleteItcResultT>(size_t(0)));
    }

    return GpTaskDoRes::DONE;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
