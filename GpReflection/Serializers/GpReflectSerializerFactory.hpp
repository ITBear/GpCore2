#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/Serializers/GpReflectSerializer.hpp>

namespace GPlatform {

class GpReflectSerializerFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReflectSerializerFactory)
    CLASS_DD(GpReflectSerializerFactory)

protected:
                                    GpReflectSerializerFactory  (void) noexcept = default;

public:
    virtual                         ~GpReflectSerializerFactory (void) noexcept = default;

    virtual GpReflectSerializer::SP NewInstance                 (void) const = 0;
};

}// namespace GPlatform
