#pragma once

#include <GpCore2/GpTasks/GpTasks_global.hpp>
#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpTypeShell.hpp>

#if defined(GP_USE_MULTITHREADING)

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

GP_ENUM(GP_TASKS_API, GpTaskState,
    NOT_STARTED,
    WAITING,
    RUNING,
    DONE
);

class GpTaskId_type;
using GpTaskId      = GpTypeShell<u_int_64, GpTaskId_type>;

class GpTaskGroupId_type;
using GpTaskGroupId = GpTypeShell<u_int_64, GpTaskGroupId_type>;

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
