#pragma once

#include "GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GP_ENUM(GP_TASKS_API, GpTaskState,
    NOT_ASSIGNED_TO_SCHEDULER,
    READY_TO_RUN,
    RUNNING,
    WAITING,
    FINISHED
);

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
