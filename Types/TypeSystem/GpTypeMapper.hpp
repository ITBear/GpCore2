#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructBase.hpp"

namespace GPlatform {

class GpByteWriter;

class GPCORE_API GpTypeMapper
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypeMapper)

public:
                                    GpTypeMapper    (void) noexcept;
    virtual                         ~GpTypeMapper   (void) noexcept;

    virtual GpTypeStructBase::SP    ToStruct        (GpRawPtrByteR aData) const = 0;
    virtual GpTypeStructBase::SP    ToStruct        (GpRawPtrByteR              aData,
                                                     const GpTypeStructInfo&    aTypeInfo) const = 0;

    GpBytesArray                    FromStruct      (const GpTypeStructBase&    aStruct) const;
    virtual void                    FromStruct      (const GpTypeStructBase&    aStruct,
                                                     GpByteWriter&              aWriter) const = 0;
};

}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM
