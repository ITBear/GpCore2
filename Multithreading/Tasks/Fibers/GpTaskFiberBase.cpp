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

    GpEvent::SP event = PopNextEvent();
    while (!aStopToken.stop_requested())
    {
        const GpTask::ResT stepRes = OnStep(event.IsNotNULL() ? EventOptRefT(event.Vn()) : std::nullopt);

        if (stepRes == GpTask::ResT::READY_TO_EXEC)
        {
            GpTaskFiberCtx::SYeld(GpTask::ResT::READY_TO_EXEC);
            //event = PopNextEvent();
            //continue;
        } else if (stepRes == GpTask::ResT::WAITING)
        {
            event = PopNextEvent();
            if (event.IsNotNULL())
            {
                continue;
            } else
            {
                GpTaskFiberCtx::SYeld(GpTask::ResT::WAITING);
                event = PopNextEvent();
            }
        } else if (stepRes == GpTask::ResT::DONE)
        {
            break;
        }
    }

    GpTaskFiberCtx::SYeld(GpTask::ResT::DONE);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
