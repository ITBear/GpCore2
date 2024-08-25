#pragma once

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Threads/GpRunnable.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>

#include <thread>

namespace GPlatform {

class GP_UTILS_API GpThread
{
public:
    CLASS_DD(GpThread)

#if defined(GP_USE_MULTITHREADING_IMPL_STD_THREAD)
    using ImplT = std::thread;
#endif// #if defined(GP_USE_MULTITHREADING_IMPL_STD_THREAD)

public:
                            GpThread                (void) noexcept = delete;
                            GpThread                (std::string aName) noexcept;
                            GpThread                (const GpThread& aThread) = delete;
                            GpThread                (GpThread&& aThread) noexcept = delete;
                            ~GpThread               (void) noexcept;

    std::string_view        Name                    (void) const noexcept {return iName;}
    std::thread::id         Run                     (GpRunnable::SP aRunnable);
    inline std::thread::id  ThreadId                (void) const noexcept;

    void                    RequestStop             (void) noexcept;
    void                    Join                    (void) noexcept;

    static void             SSetSysNameForCurrent   (std::string_view   aName);

private:
    const std::string       iName;
    std::atomic_flag        iThreadStopRequestF;    // false
    std::atomic_flag        iThreadRunnableDoneF;   // true

    mutable GpMutex         iMutex;
    GpRunnable::SP          iRunnable   GUARDED_BY(iMutex);
    ImplT                   iThread     GUARDED_BY(iMutex);
    std::thread::id         iThreadId   GUARDED_BY(iMutex);
};

std::thread::id GpThread::ThreadId (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};
    return iThreadId;
}

}// GPlatform
