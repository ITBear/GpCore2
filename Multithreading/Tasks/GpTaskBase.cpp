#include "GpTaskBase.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskBase::GpTaskBase (std::string_view aName):
GpTask(aName)
{
}

GpTaskBase::~GpTaskBase (void) noexcept
{
}

GpTask::ResT    GpTaskBase::Do (GpThreadStopToken aStopToken) noexcept
{
    try
    {
        if (!iIsStarted)
        {
            OnStart();
            iIsStarted = true;
        }

        GpEvent::SP event = PopNextEvent();
        while (!aStopToken.stop_requested())
        {
            const GpTask::ResT stepRes = OnStep(event.IsNotNULL() ? EventOptRefT(event.Vn()) : std::nullopt);

            if (stepRes == GpTask::ResT::READY_TO_EXEC)
            {
                return GpTask::ResT::READY_TO_EXEC;
            } else if (stepRes == GpTask::ResT::WAITING)
            {
                event = PopNextEvent();
                if (event.IsNotNULL())
                {
                    continue;
                } else
                {
                    return GpTask::ResT::WAITING;
                }
            } else if (stepRes == GpTask::ResT::DONE)
            {
                break;
            }
        }
    } catch (const std::exception& e)
    {
        GpExceptionsSink::SSink(e);
    } catch (...)
    {
        GpExceptionsSink::SSinkUnknown();
    }

    OnStop();
    return GpTask::ResT::DONE;
}

void    GpTaskBase::Terminate (void) noexcept
{
    OnStop();
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
