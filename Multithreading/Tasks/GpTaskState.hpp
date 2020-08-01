#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../Types/Enums/GpEnum.hpp"

namespace GPlatform {

GP_ENUM(GPCORE_API, GpTaskState,
    NOT_ASSIGNED_TO_SCHEDULER,
    READY_TO_RUN,
    RUNNING,
    WAITING,
    FINISHED
);

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
