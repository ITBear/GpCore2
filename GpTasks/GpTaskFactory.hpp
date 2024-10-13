#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>

#if defined(GP_USE_MULTITHREADING)

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

    virtual GpTask::SP          NewInstance     (std::string aTaskName) const = 0;
    virtual GpTask::SP          NewInstance     (void) const = 0;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
