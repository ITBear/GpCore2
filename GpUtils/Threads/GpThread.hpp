#pragma once

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Threads/GpRunnable.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>

#include <thread>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GP_UTILS_API GpThread
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpThread)
    CLASS_DD(GpThread)

#if defined(GP_USE_MULTITHREADING_IMPL_STD_THREAD)
    using ImplT = std::thread;
#endif// #if defined(GP_USE_MULTITHREADING_IMPL_STD_THREAD)

public:
                            GpThread                (GpConditionVarFlag& aStopFlag) noexcept;
                            GpThread                (GpConditionVarFlag&    aStopFlag,
                                                     std::string            aName) noexcept;
                            ~GpThread               (void) noexcept;

    std::string_view        Name                    (void) const noexcept {return iName;}
    std::thread::id         Run                     (GpRunnable::UP aRunnableUP);
    std::thread::id         Run                     (std::function<void(GpConditionVarFlag&)> aRunFn);
    inline std::thread::id  ThreadId                (void) const noexcept;

    void                    RequestStop             (void) noexcept;
    void                    Join                    (void) noexcept;

    GpConditionVarFlag&     StopFlag                (void) noexcept {return iStopFlag;}

    static void             SSetSysNameForCurrent   (std::string_view   aName);

private:
    GpConditionVarFlag&     iStopFlag;          // false
    std::atomic_flag        iRunnableDoneFlag;  // true
    const std::string       iName;

    mutable GpSpinLockRW<>  iSpinLockRW;
    GpRunnable::UP          iRunnableUP GUARDED_BY(iSpinLockRW);
    ImplT                   iThread     GUARDED_BY(iSpinLockRW);
    std::thread::id         iThreadId   GUARDED_BY(iSpinLockRW);
};

std::thread::id GpThread::ThreadId (void) const noexcept
{
    GpSharedLock sharedLock{iSpinLockRW};

    return iThreadId;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
