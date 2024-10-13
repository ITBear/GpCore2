#include <GpCore2/GpTasks/GpTaskGroupsManager.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpTaskGroupsManager GpTaskGroupsManager::sInstance;

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
