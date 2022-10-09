#pragma once

#include "GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectModelSource.hpp"

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

}//GPlatform

#endif//GP_USE_REFLECTION
