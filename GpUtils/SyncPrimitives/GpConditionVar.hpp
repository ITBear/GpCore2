#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)

#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpUtils/Other/GpRAIIonDestruct.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpMutex.hpp>

#include <condition_variable>

namespace GPlatform {

class CAPABILITY("GpConditionVar") GpConditionVar
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpConditionVar)
    CLASS_DD(GpConditionVar)
    TAG_SET(THREAD_SAFE)

    using AtBeginFnT    = std::function<void()>;
    using AtEndFnT      = std::function<void(bool)>;// must be noexcept
    using CheckFnT      = std::function<bool()>;

public:
                    GpConditionVar  (void) noexcept = default;
                    ~GpConditionVar (void) noexcept = default;

    inline void     NotifyOne       (void) noexcept REQUIRES(Mutex());
    inline void     NotifyAll       (void) noexcept REQUIRES(Mutex());
    inline GpMutex& Mutex           (void) noexcept RETURN_CAPABILITY(iMutex);

    inline void     Wait            (const CheckFnT&    aCheckFn);
    inline void     Wait            (const CheckFnT&    aCheckFn,
                                     const AtBeginFnT&  aAtBeginFn,
                                     const AtEndFnT&    aAtEndFn);

    inline bool     WaitFor         (const CheckFnT&    aCheckFn,
                                     milliseconds_t     aTimeout);
    inline bool     WaitFor         (const CheckFnT&    aCheckFn,
                                     milliseconds_t     aTimeout,
                                     const AtBeginFnT&  aAtBeginFn,
                                     const AtEndFnT&    aAtEndFn);

private:
    mutable GpMutex         iMutex;
    std::condition_variable iCV GUARDED_BY(iMutex);
};

void    GpConditionVar::NotifyOne (void) noexcept
{
    iCV.notify_one();
}

void    GpConditionVar::NotifyAll (void) noexcept
{
    iCV.notify_all();
}

GpMutex&    GpConditionVar::Mutex (void) noexcept
{
    return iMutex;
}

void    GpConditionVar::Wait (const CheckFnT& aCheckFn)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    iCV.wait
    (
        uniqueLock.internal_lock(),
        aCheckFn
    );
}

void    GpConditionVar::Wait
(
    const CheckFnT&     aCheckFn,
    const AtBeginFnT&   aAtBeginFn,
    const AtEndFnT&     aAtEndFn
)
{
    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    GpRAIIonDestruct callOnDestruct = [&aAtEndFn]()
    {
        aAtEndFn(true);
    };

    aAtBeginFn();

    iCV.wait
    (
        uniqueLock.internal_lock(),
        aCheckFn
    );
}

bool    GpConditionVar::WaitFor
(
    const CheckFnT&         aCheckFn,
    const milliseconds_t    aTimeout
)
{
    if (aTimeout <= 0.0_si_ms)
    {
        Wait(aCheckFn);
        return true;
    }

    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    const bool checkFnRes = iCV.wait_for
    (
        uniqueLock.internal_lock(),
        std::chrono::milliseconds(aTimeout.As<ssize_t>()),
        aCheckFn
    );

    return checkFnRes;
}

bool    GpConditionVar::WaitFor
(
    const CheckFnT&         aCheckFn,
    const milliseconds_t    aTimeout,
    const AtBeginFnT&       aAtBeginFn,
    const AtEndFnT&         aAtEndFn
)
{
    if (aTimeout <= 0.0_si_ms)
    {
        Wait(aCheckFn, aAtBeginFn, aAtEndFn);
        return true;
    }

    GpUniqueLock<GpMutex> uniqueLock{iMutex};

    bool result = false;

    GpRAIIonDestruct callOnDestruct = [&aAtEndFn, &result]()
    {
        aAtEndFn(result);
    };

    aAtBeginFn();

    result = iCV.wait_for
    (
        uniqueLock.internal_lock(),
        std::chrono::milliseconds(aTimeout.As<ssize_t>()),
        aCheckFn
    );

    return result;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
