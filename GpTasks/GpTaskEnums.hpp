#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTasks_global.hpp"
#include "../GpUtils/Types/Enums/GpEnum.hpp"
#include "../GpUtils/Types/Strings/GpStringOps.hpp"

namespace GPlatform {

GP_ENUM(GP_TASKS_API, GpTaskRunRes,
    WAIT,
    READY_TO_RUN,
    DONE
);

GP_ENUM(GP_TASKS_API, GpTaskMode,
    THREAD,
    FIBER
);

using GpTaskId = u_int_64;

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
