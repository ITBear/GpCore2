#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectModelSource.hpp>

namespace GPlatform {

class GpReflectModelSourceFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReflectModelSourceFactory)
    CLASS_DD(GpReflectModelSourceFactory)

protected:
                                        GpReflectModelSourceFactory     (void) noexcept = default;

public:
    virtual                             ~GpReflectModelSourceFactory    (void) noexcept = default;

    virtual GpReflectModelSource::SP    NewInstance                     (void) const = 0;
};

}// namespace GPlatform
