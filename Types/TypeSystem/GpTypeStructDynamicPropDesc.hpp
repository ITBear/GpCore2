#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructBase.hpp"

namespace GPlatform {

class GPCORE_API GpTypeStructDynamicPropDesc final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypeStructDynamicPropDesc)
    TYPE_STRUCT_DECLARE("f7a8691c-070a-4576-9e00-f4156b61758a"_sv)

public:
                            GpTypeStructDynamicPropDesc     (void) noexcept;
    explicit                GpTypeStructDynamicPropDesc     (const GpTypeStructDynamicPropDesc& aDesc);
    explicit                GpTypeStructDynamicPropDesc     (GpTypeStructDynamicPropDesc&& aDesc) noexcept;
                            GpTypeStructDynamicPropDesc     (const GpType::EnumT            aValType,
                                                             const GpType::EnumT            aKeyType,
                                                             const GpTypeContainer::EnumT   aContainerType,
                                                             const GpUUID&                  aStructUid,
                                                             std::string                    aName) noexcept;
    virtual                 ~GpTypeStructDynamicPropDesc    (void) noexcept override final;

public:
    GpType                  val_type;
    GpType                  key_type;
    GpTypeContainer         container_type;
    GpUUID                  struct_uid;
    std::string             name;
};

}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM
