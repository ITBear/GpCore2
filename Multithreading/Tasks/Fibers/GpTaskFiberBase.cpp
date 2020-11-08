#include "GpTaskFiberBase.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include <iostream>

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

    while (!aStopToken.stop_requested())
    {
        GpEvent::SP event = PopNextEvent();
        OnStep(event.IsNotNULL() ? EventOptRefT(event.Vn()) : std::nullopt);

        //Pop until event queue is empty
        while ((event = PopNextEvent()).IsNotNULL())
        {
            if (aStopToken.stop_requested())
            {
                GpTaskFiberCtx::SYeld(GpTask::ResT::DONE);
            }

            OnStep(EventOptRefT(event.Vn()));
        }

        GpTaskFiberCtx::SYeld(GpTask::ResT::WAITING);
    }

    GpTaskFiberCtx::SYeld(GpTask::ResT::DONE);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
