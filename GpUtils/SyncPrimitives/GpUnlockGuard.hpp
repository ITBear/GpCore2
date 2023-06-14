#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SYNC_PRIMITIVES)

#include "../Macro/GpMacroClass.hpp"

namespace GPlatform{

template<typename T> class GpUnlockGuard
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpUnlockGuard)

public:
                            GpUnlockGuard       (T& aLock) noexcept;
                            ~GpUnlockGuard      (void) noexcept;

private:
    T&                      iLock;
};

template<typename T>
GpUnlockGuard<T>::GpUnlockGuard (T& aLock) noexcept:
iLock(aLock)
{
    iLock.unlock();
}

template<typename T>
GpUnlockGuard<T>::~GpUnlockGuard (void) noexcept
{
    iLock.lock();
}

}//GPlatform

#endif//#if defined(GP_USE_SYNC_PRIMITIVES)
