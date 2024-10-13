#pragma once

#include <GpCore2/GpTasks/GpTasks_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

#include <functional>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpTaskScheduler;

class GpTaskSchedulerFactory
{
public:
    CLASS_DD(GpTaskSchedulerFactory)
    TAG_SET(THREAD_SAFE)

    using StopServiceFnT = std::function<void()>;

protected:
                                    GpTaskSchedulerFactory  (void) noexcept = default;

public:
    virtual                         ~GpTaskSchedulerFactory (void) noexcept = default;

    virtual GpSP<GpTaskScheduler>   NewInstance             (StopServiceFnT aStopServiceFn) const = 0;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
