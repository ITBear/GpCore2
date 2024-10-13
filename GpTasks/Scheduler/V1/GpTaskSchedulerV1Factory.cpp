#include <GpCore2/GpTasks/Scheduler/V1/GpTaskSchedulerV1Factory.hpp>
#include <GpCore2/GpTasks/Scheduler/V1/GpTaskSchedulerV1.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskSchedulerV1Factory::~GpTaskSchedulerV1Factory (void) noexcept
{
}

GpSP<GpTaskScheduler>   GpTaskSchedulerV1Factory::NewInstance (StopServiceFnT aStopServiceFn) const
{
    return MakeSP<GpTaskSchedulerV1>(aStopServiceFn);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
