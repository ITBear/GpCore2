#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTasks_global.hpp"
#include "../GpUtils/Types/Enums/GpEnum.hpp"
#include "../GpUtils/Types/Strings/GpStringOps.hpp"

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
