#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskScheduler.hpp"

namespace GPlatform {

class GpTaskSchedulerAccessor
{
    CLASS_REMOVE_CTRS(GpTaskSchedulerAccessor)

public:
    static void         SMoveToReady        (GpTaskScheduler& aScheduler, GpTask::SP aTask)
    {
        aScheduler.MoveToReady(std::move(aTask));
    }
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
