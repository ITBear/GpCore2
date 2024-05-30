#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)

#include "../Threads/GpThreadsSafety.hpp"
#include <mutex>

namespace GPlatform {

using GpMutex = ThreadSafety::MutexWrap<std::mutex>;

template<class MutexWrapT>
using GpUniqueLock  = ThreadSafety::MutexLockerWrap<MutexWrapT, std::unique_lock>;

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
