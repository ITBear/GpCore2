#include "GpBaseTaskFiber.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../Utils/RAII/GpRAIIonDestruct.hpp"

namespace GPlatform {

GpBaseTaskFiber::GpBaseTaskFiber (void) noexcept
{
}

GpBaseTaskFiber::~GpBaseTaskFiber (void) noexcept
{
}

void    GpBaseTaskFiber::FiberFn (GpThreadStopToken aStopToken)
{
    GpRAIIonDestruct stopGuard([&]()
    {
        this->OnStop();
    });

    OnStart();

    while (!aStopToken.stop_requested())
    {
        GpEvent::SP event = GpTaskFiberCtx::SCurrentTask().V().PopNextEvent();
        OnStep(event.IsNotNULL() ? EventOptRefT(event.Vn()) : std::nullopt);
        GpTaskFiberCtx::SYeld(GpTask::Res::WAITING);
    }

    GpTaskFiberCtx::SYeld(GpTask::Res::DONE);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
