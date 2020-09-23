#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "../Enums/GpEnums.hpp"
#include "../Strings/GpStringOps.hpp"

namespace GPlatform {

GP_ENUM(GPCORE_API, GpTypeContainer,
    NO,
    VECTOR,
    LIST,
    SET,
    MAP
);

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
