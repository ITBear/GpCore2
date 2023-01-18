#pragma once

#include "../GpMacro.hpp"

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

    CLASS_TAG(THREAD_SAFE)

    using OnWakeupFnT   = std::function<void()>;
    using OnCheckFnT    = std::function<bool()>;

    enum class WaitForResT
    {
        TIMEOUT,
        OK
    };

                                        GpConditionVar  (void) noexcept = default;
                                        ~GpConditionVar (void) noexcept = default;

    inline void                         Do              (std::function<void()> aFn) const;

    inline void                         WakeupAll       (OnWakeupFnT aOnWakeupFn);
    inline void                         WakeupOne       (OnWakeupFnT aOnWakeupFn);
    inline void                         Wait            (OnCheckFnT             aOnCheckFn);
    [[nodiscard]] inline WaitForResT    WaitFor         (OnCheckFnT             aOnCheckFn,
                                                         const milliseconds_t   aTimeout);

private:
    mutable std::mutex                  iMutex;
    std::condition_variable             iCV;
};

void    GpConditionVar::Do (std::function<void()> aFn) const
{
    std::scoped_lock lock(iMutex);

    aFn();
}

void    GpConditionVar::WakeupAll (OnWakeupFnT aOnWakeupFn)
{
    {
        std::scoped_lock lock(iMutex);
        aOnWakeupFn();
    }

    iCV.notify_all();
}

void    GpConditionVar::WakeupOne (OnWakeupFnT aOnWakeupFn)
{
    {
        std::scoped_lock lock(iMutex);
        aOnWakeupFn();
    }

    iCV.notify_one();
}

void    GpConditionVar::Wait (OnCheckFnT aOnCheckFn)
{
    std::unique_lock lock(iMutex);

    iCV.wait
    (
        lock,
        [&]
        {
            return aOnCheckFn();
        }
    );
}

GpConditionVar::WaitForResT GpConditionVar::WaitFor
(
    OnCheckFnT              aOnCheckFn,
    const milliseconds_t    aTimeout
)
{
    std::unique_lock lock(iMutex);

    return iCV.wait_for
    (
        lock,
        std::chrono::milliseconds(aTimeout.As<ssize_t>()),
        [&]
        {
            return aOnCheckFn();
        }
    ) ? WaitForResT::OK : WaitForResT::TIMEOUT;
}

}//GPlatform

#endif//#if defined(GP_USE_SYNC_PRIMITIVES)
