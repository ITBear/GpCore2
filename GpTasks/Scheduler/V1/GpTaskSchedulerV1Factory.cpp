#include "GpTaskSchedulerV1Factory.hpp"
#include "GpTaskSchedulerV1.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskSchedulerV1Factory::~GpTaskSchedulerV1Factory (void) noexcept
{
}

GpSP<GpTaskScheduler>   GpTaskSchedulerV1Factory::NewInstance (void) const
{
    return MakeSP<GpTaskSchedulerV1>();
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
