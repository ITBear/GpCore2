#pragma once

#include "../GpUtils_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpRunnable.hpp"
#include "../SyncPrimitives/GpRWLock.hpp"
#include <shared_mutex>

namespace GPlatform {

class GP_UTILS_API GpThread
{
public:
#if defined(GP_USE_MULTITHREADING_IMPL_JTHREAD)
    using ImplT = std::jthread;
#endif//#if defined(GP_USE_MULTITHREADING_IMPL_JTHREAD)

    CLASS_DD(GpThread)

public:
                            GpThread    (void) noexcept;
                            GpThread    (std::string aName) noexcept;
                            GpThread    (const GpThread& aThread) = delete;
                            GpThread    (GpThread&& aThread) noexcept;
                            ~GpThread   (void) noexcept;

    std::string_view        Name        (void) const noexcept {return iName;}
    std::thread::id         Run         (GpRunnable::SP aRunnable);
    inline std::thread::id  ThreadId    (void) const noexcept;

    bool                    Joinable    (void) const noexcept {return iThread.joinable();}
    void                    Join        (void) noexcept;
    bool                    RequestStop (void) noexcept;

private:
    mutable GpRWLock        iRWLock;
    std::string             iName;
    ImplT                   iThread;
    GpRunnable::SP          iRunnable;
    std::thread::id         iThreadId;
};

std::thread::id GpThread::ThreadId (void) const noexcept
{
    std::shared_lock lock(iRWLock);
    return iThreadId;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
