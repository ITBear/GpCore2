#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflection_global.hpp"
#include "../GpUtils/Types/Enums/GpEnum.hpp"
#include "../GpUtils/Types/Strings/GpStringOps.hpp"

namespace GPlatform {

GP_ENUM(GP_REFLECTION_API, GpReflectPropFlag,
    UNWRAP_CONTAINER,
    UNWRAP_CONTAINER_KEY,
    CONTAINER_ELEMENT_NAME,
    AS_TUPLE,
    NAME_OVERRIDE,
    GENERATED_ONCE,
    GENERATED_OUTSIDE,
    PRIMARY_KEY,
    MULTILANGUAGE_STRING
);

using GpReflectPropFlags = GpEnumFlagsST<GpReflectPropFlag>;

}//GPlatform

#endif//GP_USE_REFLECTION
