#include <GpCore2/GpTasks/Scheduler/V1/GpTaskSchedulerV1Factory.hpp>
#include <GpCore2/GpTasks/Scheduler/V1/GpTaskSchedulerV1.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskSchedulerV1Factory::~GpTaskSchedulerV1Factory (void) noexcept
{
}

GpTaskScheduler::UP GpTaskSchedulerV1Factory::NewInstance
(
    const size_t aExecutorsCount,
    const size_t aTasksMaxCount
) const
{
    return std::make_unique<GpTaskSchedulerV1>(aExecutorsCount, aTasksMaxCount);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
