#pragma once

#include "../GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectSerializer.hpp"

namespace GPlatform {

class GpReflectSerializerFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReflectSerializerFactory)
    CLASS_DECLARE_DEFAULTS(GpReflectSerializerFactory)

protected:
                                    GpReflectSerializerFactory  (void) noexcept = default;

public:
    virtual                         ~GpReflectSerializerFactory (void) noexcept = default;

    virtual GpReflectSerializer::SP NewInstance                 (void) const = 0;
};

}//GPlatform

#endif//GP_USE_REFLECTION
