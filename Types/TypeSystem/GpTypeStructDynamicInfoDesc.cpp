#include "GpTypeStructDynamicInfoDesc.hpp"
#include "GpTypeManager.hpp"
#include "GpTypeUtils.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpTypeStructDynamicInfoDesc, GP_MODULE_UUID)

GpTypeStructDynamicInfoDesc::GpTypeStructDynamicInfoDesc (void) noexcept
{
}

GpTypeStructDynamicInfoDesc::GpTypeStructDynamicInfoDesc (const GpTypeStructDynamicInfoDesc& aDesc):
GpTypeStructBase(aDesc),
uid(aDesc.uid),
base_uid(aDesc.base_uid),
group_id(aDesc.group_id),
name(aDesc.name),
props(aDesc.props)
{
}

GpTypeStructDynamicInfoDesc::GpTypeStructDynamicInfoDesc (GpTypeStructDynamicInfoDesc&& aDesc) noexcept:
GpTypeStructBase(std::move(aDesc)),
uid(std::move(aDesc.uid)),
base_uid(std::move(aDesc.base_uid)),
group_id(std::move(aDesc.group_id)),
name(std::move(aDesc.name)),
props(std::move(aDesc.props))
{
}

GpTypeStructDynamicInfoDesc::GpTypeStructDynamicInfoDesc
(
    const GpUUID&                                   aUid,
    const GpUUID&                                   aBaseUid,
    const GpUUID&                                   aGroupId,
    std::string                                     aName,
    const GpTypeStructDynamicPropDesc::C::Vec::SP&  aProps
):
uid(aUid),
base_uid(aBaseUid),
group_id(aGroupId),
name(std::move(aName)),
props(aProps)
{
}

GpTypeStructDynamicInfoDesc::~GpTypeStructDynamicInfoDesc (void) noexcept
{
}

void    GpTypeStructDynamicInfoDesc::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(uid);
    PROP(base_uid);
    PROP(group_id);
    PROP(name);
    PROP(props);
}

}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM
