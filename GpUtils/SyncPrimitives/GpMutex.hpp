#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SYNC_PRIMITIVES)

#include "../Threads/GpThreadsSafety.hpp"
#include <mutex>

namespace GPlatform {

using GpMutex       = ThreadSafety::MutexWrap<std::mutex>;

template<class MutexWrapT>
using GpUniqueLock  = ThreadSafety::MutexLockerWrap<MutexWrapT, std::unique_lock>;

}//GPlatform

#endif//#if defined(GP_USE_SYNC_PRIMITIVES)
