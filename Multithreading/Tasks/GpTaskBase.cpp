#include "GpTaskBase.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskBase::GpTaskBase (void) noexcept
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

        if (!aStopToken.stop_requested())
        {
            GpEvent::SP event = PopNextEvent();

            GpTask::ResT stepRes = OnStep(event.IsNotNULL() ? EventOptRefT(event.Vn()) : std::nullopt);

            if (stepRes == ResT::DONE)
            {
                OnStop();
                return ResT::DONE;
            }

            //Pop until event queue is empty
            while ((event = PopNextEvent()).IsNotNULL())
            {
                if (aStopToken.stop_requested())
                {
                    OnStop();
                    return ResT::DONE;
                }

                 stepRes = OnStep(EventOptRefT(event.Vn()));

                if (stepRes == ResT::DONE)
                {
                    OnStop();
                    return ResT::DONE;
                }
            }

            return stepRes;
        }
    } catch (const std::exception& e)
    {
        GpExceptionsSink::SSink(e);
    } catch (...)
    {
        GpExceptionsSink::SSinkUnknown();
    }

    OnStop();
    return ResT::DONE;
}

void    GpTaskBase::Terminate (void) noexcept
{
    OnStop();
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
