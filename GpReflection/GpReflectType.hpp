#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

GP_ENUM(GP_REFLECTION_API, GpReflectType,
    NOT_SET,
    U_INT_8,
    S_INT_8,
    U_INT_16,
    S_INT_16,
    U_INT_32,
    S_INT_32,
    U_INT_64,
    S_INT_64,
    DOUBLE,
    FLOAT,
    BOOLEAN,
    UUID,
    STRING,
    BLOB,
    OBJECT,
    OBJECT_SP,
    ENUM,
    ENUM_FLAGS
);

}// namespace GPlatform
