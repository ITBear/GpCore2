#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)

#include "../Threads/GpThreadsSafety.hpp"
#include <shared_mutex>

namespace GPlatform {

using GpSharedMutex = ThreadSafety::SharedMutexWrap<std::shared_mutex>;

template<class MutexWrapT>
using GpSharedLock  = ThreadSafety::SharedMutexLockerWrap<MutexWrapT, std::shared_lock>;

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
