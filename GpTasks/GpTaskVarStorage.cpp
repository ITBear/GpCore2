#include <GpCore2/GpTasks/GpTaskVarStorage.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskVarStorage    GpTaskVarStorage::sInstance;

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
