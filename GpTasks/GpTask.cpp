#include "GpTask.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskScheduler.hpp"

#include <iostream>

namespace GPlatform {

GpFlatMap<std::thread::id, GpTask*, 64> GpTask::sTasksByThreadId;

GpTask::~GpTask (void) noexcept
{
    CompletePromise(MakeSP<GpItcResult>());
}

void    GpTask::OnPushEvent (void)
{
    GpTaskScheduler::S().MakeTaskReady(Guid());
}

GpTaskDoRes GpTask::Run (GpThreadStopToken aStopToken) noexcept
{
    GpRAIIonDestruct onDestruct
    ([](){
        SClearCurrent();
    });

    SSetCurrent(this);
    return _Run(std::move(aStopToken));
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
