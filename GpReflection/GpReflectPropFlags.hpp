#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

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

}// namespace GPlatform
