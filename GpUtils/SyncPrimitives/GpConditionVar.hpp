#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SYNC_PRIMITIVES)

#include "../Types/Containers/GpContainersT.hpp"
#include "../Types/Units/SI/GpUnitsSI_Time.hpp"

#include <mutex>
#include <condition_variable>

namespace GPlatform {

class GpConditionVar
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpConditionVar)
    CLASS_DD(GpConditionVar)
    TAG_SET(THREAD_SAFE)

    using AtomicFnT = std::function<void(std::mutex&)>;
    using CheckFnT  = std::function<bool()>;

                            GpConditionVar  (void) noexcept = default;
                            ~GpConditionVar (void) noexcept = default;

    inline void             DoAtomic        (const AtomicFnT& aFn) const;
    inline void             NotifyAll       (void);
    inline void             NotifyOne       (void);
    inline void             Wait            (const CheckFnT&        aCheckFn);
    inline bool             WaitFor         (const CheckFnT&        aCheckFn,
                                             const milliseconds_t   aTimeout);

private:
    mutable std::mutex      iMutex;
    std::condition_variable iCV;
};

void    GpConditionVar::DoAtomic (const AtomicFnT& aFn) const
{
    std::scoped_lock lock(iMutex);
    aFn(iMutex);
}

void    GpConditionVar::NotifyAll (void)
{
    iCV.notify_all();
}

void    GpConditionVar::NotifyOne (void)
{
    iCV.notify_one();
}

void    GpConditionVar::Wait (const CheckFnT& aCheckFn)
{
    std::unique_lock lock(iMutex);

    iCV.wait
    (
        lock,
        aCheckFn
    );
}

bool    GpConditionVar::WaitFor
(
    const CheckFnT&         aCheckFn,
    const milliseconds_t    aTimeout
)
{
    std::unique_lock lock(iMutex);

    return iCV.wait_for
    (
        lock,
        std::chrono::milliseconds(aTimeout.As<ssize_t>()),
        aCheckFn
    );
}

}//GPlatform

#endif//#if defined(GP_USE_SYNC_PRIMITIVES)
