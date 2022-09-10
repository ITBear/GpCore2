#pragma once

#include "GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

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

}//GPlatform

#endif//GP_USE_REFLECTION
