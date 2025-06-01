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
              GpUnlockGuard   (T& aLock) noexcept: iLock{aLock} {iLock.unlock();}
              ~GpUnlockGuard  (void) noexcept {iLock.lock();}

private:
  T&          iLock;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
