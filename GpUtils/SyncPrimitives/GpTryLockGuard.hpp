#pragma once

#include "../GpMacro.hpp"

#if defined(GP_USE_SYNC_PRIMITIVES)

namespace GPlatform{

template<typename T> class GpTryLockGuard
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTryLockGuard)

public:
                            GpTryLockGuard      (T& aLock) noexcept;
                            ~GpTryLockGuard     (void) noexcept;

    bool                    IsLocked            (void) const noexcept {return iIsLocked;}

private:
    const bool              iIsLocked;
    T&                      iLock;
};

template<typename T>
GpTryLockGuard<T>::GpTryLockGuard (T& aLock) noexcept:
iIsLocked(aLock.try_lock()),
iLock(aLock)
{
}

template<typename T>
GpTryLockGuard<T>::~GpTryLockGuard (void) noexcept
{
    if (iIsLocked)
    {
        iLock.unlock();
    }
}

}//GPlatform

#endif//#if defined(GP_USE_SYNC_PRIMITIVES)
