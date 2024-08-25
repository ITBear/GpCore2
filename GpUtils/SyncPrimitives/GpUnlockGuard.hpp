#pragma once

//#include <GpCore2/Config/GpConfig.hpp>
//#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)

// namespace GPlatform {
//
//template<typename T>
//class /*SCOPED_CAPABILITY*/ GpUnlockGuard
//{
//  CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpUnlockGuard)
//
//public:
//              GpUnlockGuard   (T& aLock) noexcept: iLock{aLock} {iLock.unlock();}
//              ~GpUnlockGuard  (void) noexcept {iLock.lock();}
//
//private:
//  T&          iLock;
//};
//
//}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
