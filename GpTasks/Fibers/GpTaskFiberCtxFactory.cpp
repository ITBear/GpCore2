#include "GpTaskFiberCtxFactory.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

GpTaskFiberCtxFactory::SP   GpTaskFiberCtxFactory::sInstance;

void    GpTaskFiberCtxFactory::SInit (GpTaskFiberCtxFactory::SP aFactory)
{
    sInstance = std::move(aFactory);
}

void    GpTaskFiberCtxFactory::SClear (void)
{
    if (sInstance.IsNotNULL())
    {
        sInstance.Clear();
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
