#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypePropInfo.hpp"
#include "GpTypeStructFactory.hpp"

namespace GPlatform {

class GPCORE_API GpTypeStructInfo
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypeStructInfo)

    using PropT         = GpTypePropInfo;
    using PropsT        = PropT::C::Vec::Val;

public:
                                GpTypeStructInfo    (void) noexcept {}
                                GpTypeStructInfo    (const GpUUID&              aUID,
                                                     const GpUUID&              aBaseUID,
                                                     std::string&&              aName,
                                                     PropsT&&                   aProps,
                                                     const GpUUID&              aGroupID,
                                                     GpTypeStructFactory::SP    aFactory) noexcept;
                                GpTypeStructInfo    (const GpTypeStructInfo& aTypeInfo);
                                GpTypeStructInfo    (GpTypeStructInfo&& aTypeInfo) noexcept;
                                ~GpTypeStructInfo   (void) noexcept;

    GpTypeStructInfo&           operator=           (const GpTypeStructInfo& aTypeInfo);
    GpTypeStructInfo&           operator=           (GpTypeStructInfo&& aTypeInfo) noexcept;

    const GpUUID&               UID                 (void) const noexcept {return iUID;}
    const GpUUID&               BaseUID             (void) const noexcept {return iBaseUID;}
    std::string_view            Name                (void) const noexcept {return iName;}
    const PropsT&               Props               (void) const noexcept {return iProps;}
    const GpUUID&               GroupID             (void) const noexcept {return iGroupID;}

    GpTypeStructFactory::SP     Factory             (void) const noexcept {return iFactory;}
    GpSP<GpTypeStructBase>      NewInstance         (void) const {return iFactory.VCn().NewInstance();}


private:
    GpUUID                      iUID;
    GpUUID                      iBaseUID;
    std::string                 iName;
    PropsT                      iProps;
    GpUUID                      iGroupID;
    GpTypeStructFactory::SP     iFactory;
};

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
