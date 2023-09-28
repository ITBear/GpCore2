#pragma once

#include "../GpTasks_global.hpp"
#include "../../GpUtils/Macro/GpMacroTags.hpp"
#include "../../GpUtils/Macro/GpMacroClass.hpp"
#include "../../GpUtils/Types/Containers/GpContainersT.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpTaskScheduler;

class GpTaskSchedulerFactory
{
public:
    CLASS_DD(GpTaskSchedulerFactory)
    TAG_SET(THREAD_SAFE)

protected:
                                    GpTaskSchedulerFactory  (void) noexcept = default;

public:
    virtual                         ~GpTaskSchedulerFactory (void) noexcept = default;

    virtual GpSP<GpTaskScheduler>   NewInstance             (void) const = 0;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
