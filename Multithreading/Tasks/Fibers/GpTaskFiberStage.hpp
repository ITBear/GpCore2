#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../Types/Enums/GpEnum.hpp"

namespace GPlatform {

GP_ENUM(GPCORE_API, GpTaskFiberStage,
    NOT_RUN,
    RUN,
    FINISHED
);

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
