#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTask.hpp"

namespace GPlatform {

class GpTaskFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFactory)
    CLASS_DD(GpTaskFactory)

protected:
                                GpTaskFactory   (void) noexcept = default;

public:
    virtual                     ~GpTaskFactory  (void) noexcept = default;

    virtual GpTask::SP          NewInstance     (std::u8string aTaskName) const = 0;
    virtual GpTask::SP          NewInstance     (void) const = 0;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
