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
                                GpTypeStructInfo    (void) noexcept = delete;
                                GpTypeStructInfo    (const GpUUID&              aUID,
                                                     const GpUUID&              aBaseUID,
                                                     std::string&&              aName,
                                                     PropsT&&                   aProps,
                                                     const GpUUID&              aGroupID,
                                                     GpTypeStructFactory::SP    aFactory,
                                                     const size_t               aAlign,
                                                     const size_t               aSize) noexcept;
                                GpTypeStructInfo    (const GpTypeStructInfo& aTypeInfo);
                                GpTypeStructInfo    (GpTypeStructInfo&& aTypeInfo) noexcept;
                                ~GpTypeStructInfo   (void) noexcept;

    GpTypeStructInfo&           operator=           (const GpTypeStructInfo& aTypeInfo);
    GpTypeStructInfo&           operator=           (GpTypeStructInfo&& aTypeInfo) noexcept;

    const GpUUID&               UID                 (void) const noexcept {return iUID;}
    const GpUUID&               BaseUID             (void) const noexcept {return iBaseUID;}
    std::string_view            Name                (void) const noexcept {return iName;}
    const PropsT&               Props               (void) const noexcept {return iProps;}
    const GpTypePropInfo&       Prop                (const size_t aId) const {return iProps.at(aId);}
    const GpTypePropInfo&       Prop                (std::string_view aName) const;
    const GpUUID&               GroupID             (void) const noexcept {return iGroupID;}
    size_t                      Align               (void) const noexcept {return iAlign;}
    size_t                      Size                (void) const noexcept {return iSize;}
    size_t                      SizeOfProps         (void) const;

    GpTypeStructFactory::SP     Factory             (void) const noexcept {return iFactory;}
    GpSP<GpTypeStructBase>      NewInstance         (void) const {return iFactory.VCn().NewInstance(*this);}
    void                        ConstructStruct     (void* aStructDataPtr) const {iFactory.VCn().ConstructStruct(aStructDataPtr);}
    void                        DestructStruct      (void* aStructDataPtr) const {iFactory.VCn().DestructStruct(aStructDataPtr);}

private:
    GpUUID                      iUID;
    GpUUID                      iBaseUID;
    std::string                 iName;
    PropsT                      iProps;
    GpUUID                      iGroupID;
    GpTypeStructFactory::SP     iFactory;
    size_t                      iAlign;
    size_t                      iSize;
    mutable size_t              iSizeOfProps = 0;
};

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
