#pragma once

#include "GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

GP_ENUM(GP_REFLECTION_API, GpReflectPropFlag,
    UNWRAP_CONTAINER,
    UNWRAP_CONTAINER_KEY,
    CONTAINER_ELEMENT_NAME,
    AS_TUPLE,
    NAME_OVERRIDE,
    GENERATED_ONCE,
    GENERATED_OUTSIDE,
    PRIMARY_KEY
);

using GpReflectPropFlags = GpEnumFlagsST<GpReflectPropFlag>;

}//GPlatform

#endif//GP_USE_REFLECTION
