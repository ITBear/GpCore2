#pragma once

#include "../GpUtils_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpRunnable.hpp"
#include "../SyncPrimitives/GpRWSpinLock.hpp"
#include <shared_mutex>
#include <thread>

namespace GPlatform {

class GP_UTILS_API GpThread
{
public:
    CLASS_DD(GpThread)

#if defined(GP_USE_MULTITHREADING_IMPL_STD_THREAD)
    using ImplT = std::thread;
#endif//#if defined(GP_USE_MULTITHREADING_IMPL_STD_THREAD)

public:
                            GpThread                (void) noexcept = delete;
    inline                  GpThread                (std::u8string aName) noexcept;
                            GpThread                (const GpThread& aThread) = delete;
                            GpThread                (GpThread&& aThread) noexcept = delete;
    virtual                 ~GpThread               (void) noexcept;

    std::u8string_view      Name                    (void) const noexcept {return iName;}
    std::thread::id         Run                     (GpRunnable::SP aRunnable);
    inline std::thread::id  ThreadId                (void) const noexcept;

    void                    RequestStop             (void) noexcept;
    void                    Join                    (void) noexcept;

    static void             SSetSysNameForCurrent   (std::u8string_view aName);

private:
    mutable GpRWSpinLock    iRWLock;
    std::u8string           iName;
    ImplT                   iThread;
    GpRunnable::SP          iRunnable;
    std::thread::id         iThreadId;
    std::atomic_flag        iThreadStopRequestF     = false;
    std::atomic_flag        iThreadRunnableDoneF    = true;
};

GpThread::GpThread (std::u8string aName) noexcept:
iName(std::move(aName))
{
}

std::thread::id GpThread::ThreadId (void) const noexcept
{
    std::shared_lock lock(iRWLock);
    return iThreadId;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
