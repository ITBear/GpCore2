#include "GpTaskFiberBase.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../GpUtils/Other/GpRAIIonDestruct.hpp"

namespace GPlatform {

GpTaskFiberBase::~GpTaskFiberBase (void) noexcept
{
}

void    GpTaskFiberBase::FiberFn (GpThreadStopToken aStopToken)
{
    GpRAIIonDestruct stopGuard
    (
        [&]()
        {
            this->OnStop();
        }
    );

    OnStart();

    GpEvent::SP event;

    while (   (aStopToken.stop_requested() == false)
           && (IsStopRequested() == false))
    {
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
            YELD_READY();
        } else if (stepRes == GpTaskDoRes::WAITING)
        {
            event = PopNextEvent();
            if (event.IsNULL())
            {
                YELD_WAITING();
            }
        } else if (stepRes == GpTaskDoRes::DONE)
        {
            return;
        }
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
