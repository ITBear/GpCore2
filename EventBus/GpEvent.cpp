#include "GpEvent.hpp"

#if defined (GP_USE_EVENT_BUS)

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpEvent, GP_MODULE_UUID)

GpEvent::GpEvent (void) noexcept
{
}

GpEvent::~GpEvent (void) noexcept
{
}

void    GpEvent::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& /*aPropsOut*/)
{
    //PROP();
}

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
