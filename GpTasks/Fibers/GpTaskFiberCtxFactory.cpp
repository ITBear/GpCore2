#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtxFactory.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

GpTaskFiberCtxFactory::UP   GpTaskFiberCtxFactory::sInstance;

void    GpTaskFiberCtxFactory::SInit (GpTaskFiberCtxFactory::UP aFactoryUP)
{
    sInstance = std::move(aFactoryUP);
}

void    GpTaskFiberCtxFactory::SClear (void)
{
    if (sInstance != nullptr)
    {
        sInstance.reset();
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
