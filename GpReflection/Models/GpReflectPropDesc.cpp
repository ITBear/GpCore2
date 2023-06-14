#include "GpReflectPropDesc.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

REFLECT_IMPLEMENT(GpReflectPropDesc, GP_MODULE_UUID)

GpReflectPropDesc::GpReflectPropDesc (void) noexcept
{
}

GpReflectPropDesc::GpReflectPropDesc (const GpReflectPropDesc& aDesc):
GpReflectObject(aDesc),
val_type      (GpReflectUtils::SCopyValue(aDesc.val_type)),
key_type      (GpReflectUtils::SCopyValue(aDesc.key_type)),
container_type(GpReflectUtils::SCopyValue(aDesc.container_type)),
model_uid     (GpReflectUtils::SCopyValue(aDesc.model_uid)),
name          (GpReflectUtils::SCopyValue(aDesc.name))
{
}

GpReflectPropDesc::GpReflectPropDesc (GpReflectPropDesc&& aDesc) noexcept:
GpReflectObject(std::move(aDesc)),
val_type      (std::move(aDesc.val_type)),
key_type      (std::move(aDesc.key_type)),
container_type(std::move(aDesc.container_type)),
model_uid     (std::move(aDesc.model_uid)),
name          (std::move(aDesc.name))
{
}

GpReflectPropDesc::GpReflectPropDesc
(
    const GpReflectType::EnumT          aValType,
    const GpReflectType::EnumT          aKeyType,
    const GpReflectContainerType::EnumT aContainerType,
    const GpUUID&                       aModelUid,
    std::u8string                       aName
) noexcept:
val_type      (aValType),
key_type      (aKeyType),
container_type(aContainerType),
model_uid     (aModelUid),
name          (std::move(aName))
{
}

GpReflectPropDesc::~GpReflectPropDesc (void) noexcept
{
}

void    GpReflectPropDesc::_SReflectCollectProps (GpReflectProp::C::Vec::Val& aPropsOut)
{
    PROP(val_type);
    PROP(key_type);
    PROP(container_type);
    PROP(model_uid);
    PROP(name);
}

}//namespace GPlatform

#endif//GP_USE_REFLECTION
