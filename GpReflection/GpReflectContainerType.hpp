#pragma once

#include "GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

GP_ENUM(GP_REFLECTION_API, GpReflectContainerType,
    NO,
    VECTOR,
    MAP
);

}//GPlatform

#endif//GP_USE_REFLECTION
