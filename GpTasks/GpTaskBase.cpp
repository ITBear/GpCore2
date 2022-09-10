#include "GpTaskBase.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskBase::~GpTaskBase (void) noexcept
{
}

GpTaskDoRes GpTaskBase::_Run (GpThreadStopToken aStopToken) noexcept
{
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
        CompletePromise(MakeSP<GpItcResult>(e));
    } catch (const std::exception& e)
    {
        CompletePromise(MakeSP<GpItcResult>(e));
    } catch (...)
    {
        CompletePromise(MakeSP<GpItcResult>(std::runtime_error("Unknown exception")));
    }

    OnStop();

    CompletePromise(MakeSP<GpItcResult>());

    return GpTaskDoRes::DONE;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
