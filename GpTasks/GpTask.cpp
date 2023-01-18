#include "GpTask.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskScheduler.hpp"

namespace GPlatform {

decltype(GpTask::sTasksByThreadId)  GpTask::sTasksByThreadId;

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

void    GpTask::SAddExecutorThreadId (const std::thread::id& aThreadId)
{
    GpTask* taskNullPtr = nullptr;

    sTasksByThreadId.Insert
    (
        aThreadId,
        taskNullPtr
    );
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
