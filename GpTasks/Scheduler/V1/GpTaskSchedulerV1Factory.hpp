#pragma once

#include "../GpTaskSchedulerFactory.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskSchedulerV1Factory final: public GpTaskSchedulerFactory
{
public:
    CLASS_DD(GpTaskSchedulerV1Factory)
    TAG_SET(THREAD_SAFE)

public:
                                    GpTaskSchedulerV1Factory    (void) noexcept = default;


    virtual                         ~GpTaskSchedulerV1Factory   (void) noexcept override final;

    virtual GpSP<GpTaskScheduler>   NewInstance                 (StopServiceFnT aStopServiceFn) const override final;
};

}// namespace GPlatform
