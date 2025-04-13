#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtxFactory.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

GpTaskFiberCtxFactory::CSP  GpTaskFiberCtxFactory::sInstance;

void    GpTaskFiberCtxFactory::SInit (GpTaskFiberCtxFactory::CSP aFactory)
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

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
