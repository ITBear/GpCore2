#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflection_global.hpp"
#include "../GpUtils/Types/Enums/GpEnum.hpp"
#include "../GpUtils/Types/Strings/GpStringOps.hpp"

namespace GPlatform {

GP_ENUM(GP_REFLECTION_API, GpReflectContainerType,
    NO,
    VECTOR,
    MAP
);

}//GPlatform

#endif//GP_USE_REFLECTION
