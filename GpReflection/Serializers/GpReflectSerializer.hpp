#pragma once

#include "../GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

#include "../GpReflectObject.hpp"

namespace GPlatform {

class GpByteWriter;

class GP_REFLECTION_API GpReflectSerializer
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectSerializer)

public:
                                GpReflectSerializer     (void) noexcept = default;
    virtual                     ~GpReflectSerializer    (void) noexcept = default;

    virtual GpReflectObject::SP ToObject                (GpSpanPtrByteR aData) const = 0;
    virtual GpReflectObject::SP ToObject                (GpSpanPtrByteR         aData,
                                                         const GpReflectModel&  aModel) const = 0;
    virtual GpReflectObject::SP ToObject                (GpSpanPtrByteR                         aData,
                                                         const GpVector<const GpReflectModel*>& aModelVariants) const = 0;

    GpBytesArray                FromObject              (const GpReflectObject& aObject) const;
    virtual void                FromObject              (const GpReflectObject& aObject,
                                                         GpByteWriter&          aWriter) const = 0;
};

}//namespace GPlatform

#endif//GP_USE_REFLECTION
