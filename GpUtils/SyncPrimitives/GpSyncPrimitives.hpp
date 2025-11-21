#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)

#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>
#include <mutex>
#include <shared_mutex>

namespace GPlatform {

template<ThreadSafety::LockTraceModeE LTM = ThreadSafety::LockTraceModeE::TRACE_ENABLED>
using GpMutex           = ThreadSafety::SyncPrimitiveWrap<std::mutex, LTM>;

template<ThreadSafety::LockTraceModeE LTM = ThreadSafety::LockTraceModeE::TRACE_ENABLED>
using GpSharedMutex     = ThreadSafety::SharedSyncPrimitiveWrap<std::shared_mutex, LTM>;

template<ThreadSafety::LockTraceModeE LTM = ThreadSafety::LockTraceModeE::TRACE_ENABLED>
using GpRecursiveMutex  = ThreadSafety::SyncPrimitiveWrap<std::recursive_mutex, LTM>;

template<class MutexWrapT>
using GpUniqueLock  = ThreadSafety::SyncPrimitiveLockerWrap<std::unique_lock, MutexWrapT>;

template<class MutexWrapT>
using GpSharedLock  = ThreadSafety::SharedSyncPrimitiveLockerWrap<std::shared_lock, MutexWrapT>;

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
