#include "GpTaskFiberManagerEx.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberManager.hpp"

namespace GPlatform {

void    GpTaskFiberManagerEx::SInit
(
    const size_t        aMaxStacksCount,
    const size_byte_t   aStackSize
)
{
    GpTaskFiberManager::S().Init
    (
        aMaxStacksCount,
        aStackSize
    );
}

void    GpTaskFiberManagerEx::SClear (void) noexcept
{
    GpTaskFiberManager::S().Clear();
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
