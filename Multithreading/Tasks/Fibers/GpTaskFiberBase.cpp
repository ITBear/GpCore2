#include "GpTaskFiberBase.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../Utils/RAII/GpRAIIonDestruct.hpp"

namespace GPlatform {

GpTaskFiberBase::GpTaskFiberBase (void) noexcept
{   
}

GpTaskFiberBase::~GpTaskFiberBase (void) noexcept
{   
}

void    GpTaskFiberBase::FiberFn (GpThreadStopToken aStopToken)
{
    GpRAIIonDestruct stopGuard([&]()
    {
        this->OnStop();
    });

    OnStart();

    GpEvent::SP event;
    while (!aStopToken.stop_requested())
    {
        //Step
        GpTask::ResT stepRes;
        {
            if (event.IsNULL())
            {
                event = PopNextEvent();
            }

            stepRes = OnStep(event.IsNotNULL() ? EventOptRefT(event.Vn()) : std::nullopt);
            event.Clear();
        }

        if (stepRes == GpTask::ResT::READY_TO_EXEC)
        {
            GpTaskFiberCtx::SYeld(GpTask::ResT::READY_TO_EXEC);
        } else if (stepRes == GpTask::ResT::WAITING)
        {
            event = PopNextEvent();
            if (event.IsNULL())
            {
                GpTaskFiberCtx::SYeld(GpTask::ResT::WAITING);
            }
        } else if (stepRes == GpTask::ResT::DONE)
        {
            //break;
            return;
        }
    }

    //GpTaskFiberCtx::SYeld(GpTask::ResT::DONE);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
