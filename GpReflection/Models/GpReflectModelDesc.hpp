#pragma once

#include "../GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

#include "../GpReflectObject.hpp"
#include "../GpReflectUtils.hpp"
#include "../GpReflectManager.hpp"

#include "GpReflectPropDesc.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectModelDesc final: public GpReflectObject
{
public:
    CLASS_DECLARE_DEFAULTS(GpReflectModelDesc)
    REFLECT_DECLARE("4a415ee4-6a0a-4a9b-95be-96194071eea1"_uuid)

public:
                                    GpReflectModelDesc  (void) noexcept;
                                    GpReflectModelDesc  (const GpReflectModelDesc& aDesc);
                                    GpReflectModelDesc  (GpReflectModelDesc&& aDesc) noexcept;
                                    GpReflectModelDesc  (const GpUUID&                          aUid,
                                                         const GpUUID&                          aBaseUid,
                                                         const GpUUID&                          aGroupId,
                                                         std::string                            aName,
                                                         const GpReflectPropDesc::C::Vec::SP&   aProps);
                                    GpReflectModelDesc  (const GpUUID&                      aUid,
                                                         const GpUUID&                      aBaseUid,
                                                         const GpUUID&                      aGroupId,
                                                         std::string                        aName,
                                                         GpReflectPropDesc::C::Vec::SP&&    aProps) noexcept;
    virtual                         ~GpReflectModelDesc (void) noexcept override final;

public:
    GpUUID                          uid;
    GpUUID                          base_uid;
    GpUUID                          group_id;
    std::string                     name;
    GpReflectPropDesc::C::Vec::SP   props;
};

}//namespace GPlatform

#endif//GP_USE_REFLECTION
