#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructDynamicInfoDesc.hpp"
#include "GpTypePropInfoBuilder.hpp"

namespace GPlatform {

class GPCORE_API GpTypeStructDynamicBuilder
{
private:
                                        GpTypeStructDynamicBuilder  (void) noexcept;

public:
                                        ~GpTypeStructDynamicBuilder (void) noexcept;

    static GpTypeStructDynamicBuilder   SNew                        (void) noexcept {return GpTypeStructDynamicBuilder();}
    static GpTypeStructInfo::SP         SBuild                      (const GpTypeStructDynamicInfoDesc& aDesc);

    GpTypeStructInfo::SP                Build                       (void);

    GpTypeStructDynamicBuilder&         UID                         (const GpUUID&  aUid);
    GpTypeStructDynamicBuilder&         BaseUID                     (const GpUUID&  aBaseUid);
    GpTypeStructDynamicBuilder&         Name                        (std::string_view aName);
    GpTypePropInfoBuilder&              Props                       (void);
    GpTypeStructDynamicBuilder&         GroupID                     (const GpUUID&  aGroupId);

private:
    GpUUID                              iUID;
    GpUUID                              iBaseUID;
    std::string                         iName;
    GpUUID                              iGroupID;

    GpTypePropInfoBuilder               iPropInfoBuilder;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
