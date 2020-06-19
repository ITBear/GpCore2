#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpTypePropInfo.hpp"

namespace GPlatform {

class GPCORE_API GpTypeInfo
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypeInfo);

    using TypeT         = GpType;
    using TypeTE        = TypeT::EnumT;
    using PropT         = GpTypePropInfo;
    using PropsT        = PropT::C::Vec;

public:
                        GpTypeInfo          (void) noexcept = default;
                        GpTypeInfo          (const GpTypeInfo& aTypeInfo);
                        GpTypeInfo          (GpTypeInfo&& aTypeInfo) noexcept;
                        ~GpTypeInfo         (void) noexcept;

    GpTypeInfo&         operator=           (const GpTypeInfo& aTypeInfo);
    GpTypeInfo&         operator=           (GpTypeInfo&& aTypeInfo) noexcept;

    const GpUUID&       UID                 (void) const noexcept {return iUID;}
    const GpUUID&       BaseUID             (void) const noexcept {return iBaseUID;}
    std::string_view    Name                (void) const noexcept {return iName;}
    TypeTE              Type                (void) const noexcept {return iType;}
    const PropsT&       Props               (void) const noexcept {return iProps;}

private:
    GpUUID              iUID;
    GpUUID              iBaseUID;
    std::string         iName;
    TypeTE              iType;
    PropsT              iProps;
};

}//GPlatform

#endif//GP_USE_REFLECTION
