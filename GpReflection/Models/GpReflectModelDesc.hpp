#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/Models/GpReflectPropDesc.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectModelDesc final: public GpReflectObject
{
public:
    CLASS_DD(GpReflectModelDesc)
    REFLECT_DECLARE("4a415ee4-6a0a-4a9b-95be-96194071eea1"_uuid)

public:
                                    GpReflectModelDesc  (void) noexcept = default;
    inline                          GpReflectModelDesc  (const GpReflectModelDesc& aDesc);
    inline                          GpReflectModelDesc  (GpReflectModelDesc&& aDesc) noexcept;
    inline                          GpReflectModelDesc  (const GpUUID&                          aUid,
                                                         const GpUUID&                          aBaseUid,
                                                         const GpUUID&                          aGroupId,
                                                         std::string                            aName,
                                                         const GpReflectPropDesc::C::Vec::SP&   aProps);
    inline                          GpReflectModelDesc  (const GpUUID&                      aUid,
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
    std::string                             aName,
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
    std::string                     aName,
    GpReflectPropDesc::C::Vec::SP&& aProps
) noexcept:
uid     (aUid),
base_uid(aBaseUid),
group_id(aGroupId),
name    (std::move(aName)),
props   (std::move(aProps))
{
}

}// namespace GPlatform
