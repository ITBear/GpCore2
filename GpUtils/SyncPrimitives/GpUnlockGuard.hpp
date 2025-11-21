#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Threads/GpThreadsSafety.hpp>

namespace GPlatform {

template<typename T>
class GpUnlockGuard
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpUnlockGuard)

public:
    inline  GpUnlockGuard   (T& aSyncPrimitive) noexcept;
    inline  ~GpUnlockGuard  (void) noexcept;

private:
    T&  iSyncPrimitive;
};

template<typename T>
GpUnlockGuard<T>::GpUnlockGuard (T& aSyncPrimitive) noexcept: iSyncPrimitive{aSyncPrimitive}
{
#if defined(GP_LOCK_TRACE)
    GpLockTrace::S().OnUnlock(&iSyncPrimitive.internal(), this);
#endif// #if defined(GP_LOCK_TRACE)

    iSyncPrimitive.unlock();
}

template<typename T>
GpUnlockGuard<T>::~GpUnlockGuard (void) noexcept
{
    iSyncPrimitive.lock();

#if defined(GP_LOCK_TRACE)
    GpLockTrace::S().OnLock(&iSyncPrimitive.internal(), this);
#endif// #if defined(GP_LOCK_TRACE)
}

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
