#pragma once

#include "GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

class GpReflectObject;

class GpReflectObjectFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpReflectObjectFactory)
    CLASS_DD(GpReflectObjectFactory)

protected:
                                    GpReflectObjectFactory  (void) noexcept = default;

public:
    virtual                         ~GpReflectObjectFactory (void) noexcept = default;

    virtual GpSP<GpReflectObject>   NewInstance             (const GpUUID& aModelUid) const = 0;
    virtual void                    Construct               (void* aDataPtr) const = 0;
    virtual void                    Destruct                (void* aDataPtr) const = 0;
};

}//GPlatform

#endif//GP_USE_REFLECTION
