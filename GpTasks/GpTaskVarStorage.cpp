#include "GpTaskVarStorage.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskVarStorage    GpTaskVarStorage::sInstance;

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
