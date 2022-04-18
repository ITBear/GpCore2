#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructDynamicPropDesc.hpp"

namespace GPlatform {

class GPCORE_API GpTypeStructDynamicInfoDesc final: public GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypeStructDynamicInfoDesc)
    TYPE_STRUCT_DECLARE("4a415ee4-6a0a-4a9b-95be-96194071eea1"_sv)

public:
                                GpTypeStructDynamicInfoDesc     (void) noexcept;
    explicit                    GpTypeStructDynamicInfoDesc     (const GpTypeStructDynamicInfoDesc& aDesc);
    explicit                    GpTypeStructDynamicInfoDesc     (GpTypeStructDynamicInfoDesc&& aDesc) noexcept;
                                GpTypeStructDynamicInfoDesc     (const GpUUID&                                  aUid,
                                                                 const GpUUID&                                  aBaseUid,
                                                                 const GpUUID&                                  aGroupId,
                                                                 std::string                                    aName,
                                                                 const GpTypeStructDynamicPropDesc::C::Vec::SP& aProps);
    virtual                     ~GpTypeStructDynamicInfoDesc    (void) noexcept override final;

public:
    GpUUID                                  uid;
    GpUUID                                  base_uid;
    GpUUID                                  group_id;
    std::string                             name;
    GpTypeStructDynamicPropDesc::C::Vec::SP props;
};

}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM
