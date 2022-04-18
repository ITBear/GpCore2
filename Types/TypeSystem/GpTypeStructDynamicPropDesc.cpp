#include "GpTypeStructDynamicPropDesc.hpp"
#include "GpTypeManager.hpp"
#include "GpTypeUtils.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpTypeStructDynamicPropDesc, GP_MODULE_UUID)

GpTypeStructDynamicPropDesc::GpTypeStructDynamicPropDesc (void) noexcept
{
}

GpTypeStructDynamicPropDesc::GpTypeStructDynamicPropDesc (const GpTypeStructDynamicPropDesc& aDesc):
GpTypeStructBase(aDesc),
val_type(aDesc.val_type),
key_type(aDesc.key_type),
container_type(aDesc.container_type),
struct_uid(aDesc.struct_uid),
name(aDesc.name)
{
}

GpTypeStructDynamicPropDesc::GpTypeStructDynamicPropDesc (GpTypeStructDynamicPropDesc&& aDesc) noexcept:
GpTypeStructBase(std::move(aDesc)),
val_type(std::move(aDesc.val_type)),
key_type(std::move(aDesc.key_type)),
container_type(std::move(aDesc.container_type)),
struct_uid(std::move(aDesc.struct_uid)),
name(std::move(aDesc.name))
{
}

GpTypeStructDynamicPropDesc::GpTypeStructDynamicPropDesc
(
    const GpType::EnumT             aValType,
    const GpType::EnumT             aKeyType,
    const GpTypeContainer::EnumT    aContainerType,
    const GpUUID&                   aStructUid,
    std::string                     aName
) noexcept:
val_type(aValType),
key_type(aKeyType),
container_type(aContainerType),
struct_uid(aStructUid),
name(std::move(aName))
{
}

GpTypeStructDynamicPropDesc::~GpTypeStructDynamicPropDesc (void) noexcept
{
}

void    GpTypeStructDynamicPropDesc::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& aPropsOut)
{
    PROP(val_type);
    PROP(key_type);
    PROP(container_type);
    PROP(struct_uid);
    PROP(name);
}

}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM
