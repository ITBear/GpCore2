#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTask.hpp"

namespace GPlatform {

class GP_TASKS_API GpTaskFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFactory)
    CLASS_DD(GpTaskFactory)

protected:
                                GpTaskFactory   (void) noexcept {}

public:
    virtual                     ~GpTaskFactory  (void) noexcept {}

    virtual GpTask::SP          NewInstance     (std::u8string aTaskName) const = 0;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
