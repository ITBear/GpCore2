#include "GpReflectModelDesc.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

REFLECT_IMPLEMENT(GpReflectModelDesc, GP_MODULE_UUID)

GpReflectModelDesc::GpReflectModelDesc (void) noexcept
{
}

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

GpReflectModelDesc::~GpReflectModelDesc (void) noexcept
{
}

void    GpReflectModelDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(uid);
    PROP(base_uid);
    PROP(group_id);
    PROP(name);
    PROP(props);
}

}//namespace GPlatform

#endif//GP_USE_REFLECTION
