#include <GpCore2/GpReflection/Models/GpReflectModelDesc.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>

namespace GPlatform {

REFLECT_IMPLEMENT(GpReflectModelDesc, GP_MODULE_UUID)

GpReflectModelDesc::~GpReflectModelDesc (void) noexcept
{
}

void    GpReflectModelDesc::_SReflectCollectProps (GpReflectProp::SmallVecVal& aPropsOut)
{
    PROP(uid);
    PROP(base_uid);
    PROP(group_id);
    PROP(name);
    PROP(props);
}

}// namespace GPlatform
