#include "GpTaskFiberManager.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

GpTaskFiberManager  GpTaskFiberManager::sInstance;

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
