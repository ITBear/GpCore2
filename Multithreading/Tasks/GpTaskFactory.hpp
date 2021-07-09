#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTask.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFactory)
    CLASS_DECLARE_DEFAULTS(GpTaskFactory)

protected:
                                GpTaskFactory   (void) noexcept {}

public:
    virtual                     ~GpTaskFactory  (void) noexcept {}

    virtual GpTask::SP          NewInstance     (std::string_view aTaskName) const = 0;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
