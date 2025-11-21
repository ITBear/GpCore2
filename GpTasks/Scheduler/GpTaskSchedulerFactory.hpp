#pragma once

#include <GpCore2/GpTasks/GpTasks_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

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
    virtual                                     ~GpTaskSchedulerFactory (void) noexcept = default;

    virtual std::unique_ptr<GpTaskScheduler>    NewInstance             (size_t aExecutorsCount,
                                                                         size_t aTasksMaxCount) const = 0;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
