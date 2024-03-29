#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "../GpReflectObject.hpp"
#include "../GpReflectUtils.hpp"
#include "../GpReflectManager.hpp"

#include "GpReflectPropDesc.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectModelDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpReflectModelDesc)
    REFLECT_DECLARE(u8"4a415ee4-6a0a-4a9b-95be-96194071eea1"_uuid)

public:
                                    GpReflectModelDesc  (void) noexcept = default;
    inline                          GpReflectModelDesc  (const GpReflectModelDesc& aDesc);
    inline                          GpReflectModelDesc  (GpReflectModelDesc&& aDesc) noexcept;
    inline                          GpReflectModelDesc  (const GpUUID&                          aUid,
                                                         const GpUUID&                          aBaseUid,
                                                         const GpUUID&                          aGroupId,
                                                         std::u8string                          aName,
                                                         const GpReflectPropDesc::C::Vec::SP&   aProps);
    inline                          GpReflectModelDesc  (const GpUUID&                      aUid,
                                                         const GpUUID&                      aBaseUid,
                                                         const GpUUID&                      aGroupId,
                                                         std::u8string                      aName,
                                                         GpReflectPropDesc::C::Vec::SP&&    aProps) noexcept;
    virtual                         ~GpReflectModelDesc (void) noexcept override final;

public:
    GpUUID                          uid;
    GpUUID                          base_uid;
    GpUUID                          group_id;
    std::u8string                   name;
    GpReflectPropDesc::C::Vec::SP   props;
};

GpReflectModelDesc::GpReflectModelDesc (const GpReflectModelDesc& aDesc):
GpReflectObject(aDesc),
uid     (GpReflectUtils::SCopyValue(aDesc.uid)),
base_uid(GpReflectUtils::SCopyValue(aDesc.base_uid)),
group_id(GpReflectUtils::SCopyValue(aDesc.group_id)),
name    (GpReflectUtils::SCopyValue(aDesc.name)),
props   (GpReflectUtils::SCopyValue(aDesc.props))
{
}

GpReflectModelDesc::GpReflectModelDesc (GpReflectModelDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
uid     (std::move(aDesc.uid)),
base_uid(std::move(aDesc.base_uid)),
group_id(std::move(aDesc.group_id)),
name    (std::move(aDesc.name)),
props   (std::move(aDesc.props))
{
}

GpReflectModelDesc::GpReflectModelDesc
(
    const GpUUID&                           aUid,
    const GpUUID&                           aBaseUid,
    const GpUUID&                           aGroupId,
    std::u8string                           aName,
    const GpReflectPropDesc::C::Vec::SP&    aProps
):
uid     (aUid),
base_uid(aBaseUid),
group_id(aGroupId),
name    (std::move(aName)),
props   (GpReflectUtils::SCopyValue(aProps))
{
}

GpReflectModelDesc::GpReflectModelDesc
(
    const GpUUID&                   aUid,
    const GpUUID&                   aBaseUid,
    const GpUUID&                   aGroupId,
    std::u8string                   aName,
    GpReflectPropDesc::C::Vec::SP&& aProps
) noexcept:
uid     (aUid),
base_uid(aBaseUid),
group_id(aGroupId),
name    (std::move(aName)),
props   (std::move(aProps))
{
}

}//namespace GPlatform

#endif//GP_USE_REFLECTION
