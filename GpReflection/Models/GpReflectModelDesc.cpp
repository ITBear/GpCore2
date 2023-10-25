#include "GpReflectModelDesc.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

REFLECT_IMPLEMENT(GpReflectModelDesc, GP_MODULE_UUID)

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
