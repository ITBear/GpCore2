#include "GpTaskSchedulerV1Factory.hpp"
#include "GpTaskSchedulerV1.hpp"

namespace GPlatform {

GpTaskSchedulerV1Factory::~GpTaskSchedulerV1Factory (void) noexcept
{
}

GpSP<GpTaskScheduler>   GpTaskSchedulerV1Factory::NewInstance (StopServiceFnT aStopServiceFn) const
{
    return MakeSP<GpTaskSchedulerV1>(aStopServiceFn);
}

}// namespace GPlatform
