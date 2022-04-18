#include "GpEvent.hpp"

#if defined (GP_USE_EVENT_BUS)

namespace GPlatform {

TYPE_STRUCT_IMPLEMENT(GpEvent, GP_MODULE_UUID)

void    GpEvent::_SCollectStructProps (GpTypePropInfo::C::Vec::Val& /*aPropsOut*/)
{
    //PROP();
}

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
