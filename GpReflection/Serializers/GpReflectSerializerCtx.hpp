#pragma once

#include "../GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

class GpReflectSerializerCtx
{
public:
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReflectSerializerCtx)
    CLASS_DD(GpReflectSerializerCtx)

protected:
                        GpReflectSerializerCtx  (void) noexcept = default;

public:
    virtual             ~GpReflectSerializerCtx (void) noexcept = default;
};

}//namespace GPlatform

#endif//GP_USE_REFLECTION
