#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../Types/Classes/GpClassesDefines.hpp"

namespace GPlatform{

template<typename T> class GpUnlockGuard
{
    CLASS_REMOVE_CTRS(GpUnlockGuard)

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

#endif//#if defined(GP_USE_MULTITHREADING)
