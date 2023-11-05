#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SYNC_PRIMITIVES)

#include "../Types/Containers/GpContainersT.hpp"
#include "../Types/Units/SI/GpUnitsSI_Time.hpp"
#include "../Other/GpRAIIonDestruct.hpp"
#include "GpMutex.hpp"

#include <condition_variable>

namespace GPlatform {

class CAPABILITY("GpConditionVar") GpConditionVar
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpConditionVar)
    CLASS_DD(GpConditionVar)
    TAG_SET(THREAD_SAFE)

    using AtBeginFnT            = std::function<void()>;
    using AtEndFnT              = std::function<void()>;// must be noexcept
    using CheckFnT              = std::function<bool()>;

    template<typename T>
    using ConditionMetFnT       = std::function<std::optional<T>()>;

    template<typename T>
    using ConditionNotMetFnT    = std::function<std::optional<T>()>;



public:
                            GpConditionVar  (void) noexcept = default;
                            ~GpConditionVar (void) noexcept = default;

    inline void             NotifyOne       (void) noexcept REQUIRES(Mutex());
    inline void             NotifyAll       (void) noexcept REQUIRES(Mutex());
    inline GpMutex&         Mutex           (void) noexcept RETURN_CAPABILITY(iMutex);

    inline void             Wait            (CheckFnT               aCheckFn);
    template<typename T>
    std::optional<T>        Wait            (CheckFnT               aCheckFn,
                                             ConditionMetFnT<T>     aConditionMetFn);
    template<typename T>
    std::optional<T>        Wait            (AtBeginFnT             aAtBeginFn,
                                             AtEndFnT               aAtEndFn,
                                             CheckFnT               aCheckFn,
                                             ConditionMetFnT<T>     aConditionMetFn);


    inline bool             WaitFor         (CheckFnT               aCheckFn,
                                             const milliseconds_t   aTimeout);

    template<typename T>
    std::optional<T>        WaitFor         (CheckFnT               aCheckFn,
                                             ConditionMetFnT<T>     aConditionMetFn,
                                             const milliseconds_t   aTimeout);
    template<typename T>
    std::optional<T>        WaitFor         (AtBeginFnT             aAtBeginFn,
                                             AtEndFnT               aAtEndFn,
                                             CheckFnT               aCheckFn,
                                             ConditionMetFnT<T>     aConditionMetFn,
                                             ConditionNotMetFnT<T>  aConditionNotMetFn,
                                             const milliseconds_t   aTimeout);

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

void    GpConditionVar::Wait
(
    CheckFnT aCheckFn
)
{
    GpUniqueLock<GpMutex> lock(iMutex);

    iCV.wait
    (
        lock.internal_lock(),
        std::move(aCheckFn)
    );
}

template<typename T>
std::optional<T>    GpConditionVar::Wait
(
    CheckFnT            aCheckFn,
    ConditionMetFnT<T>  aConditionMetFn
)
{   
    GpUniqueLock<GpMutex> lock(iMutex);

    iCV.wait
    (
        lock.internal_lock(),
        std::move(aCheckFn)
    );

    return aConditionMetFn();
}

template<typename T>
std::optional<T>    GpConditionVar::Wait
(
    AtBeginFnT          aAtBeginFn,
    AtEndFnT            aAtEndFn,
    CheckFnT            aCheckFn,
    ConditionMetFnT<T>  aConditionMetFn
)
{
    GpUniqueLock<GpMutex> lock(iMutex);

    GpRAIIonDestruct callOnDestruct
    (
        [&aAtEndFn]()
        {
            aAtEndFn();
        }
    );

    aAtBeginFn();

    iCV.wait
    (
        lock.internal_lock(),
        std::move(aCheckFn)
    );

    return aConditionMetFn();
}

bool    GpConditionVar::WaitFor
(
    CheckFnT                aCheckFn,
    const milliseconds_t    aTimeout
)
{
    GpUniqueLock<GpMutex> lock(iMutex);

    const bool checkFnRes = iCV.wait_for
    (
        lock.internal_lock(),
        std::chrono::milliseconds(aTimeout.As<ssize_t>()),
        std::move(aCheckFn)
    );

    return checkFnRes;
}

template<typename T>
std::optional<T>    GpConditionVar::WaitFor
(
    CheckFnT                aCheckFn,
    ConditionMetFnT<T>      aConditionMetFn,
    const milliseconds_t    aTimeout
)
{
    GpUniqueLock<GpMutex> lock(iMutex);

    const bool checkFnRes = iCV.wait_for
    (
        lock.internal_lock(),
        std::chrono::milliseconds(aTimeout.As<ssize_t>()),
        std::move(aCheckFn)
    );

    if (checkFnRes) [[likely]]
    {
        return aConditionMetFn();
    }

    return std::nullopt;
}

template<typename T>
std::optional<T>    GpConditionVar::WaitFor
(
    AtBeginFnT              aAtBeginFn,
    AtEndFnT                aAtEndFn,
    CheckFnT                aCheckFn,
    ConditionMetFnT<T>      aConditionMetFn,
    ConditionNotMetFnT<T>   aConditionNotMetFn,
    const milliseconds_t    aTimeout
)
{
    GpUniqueLock<GpMutex> lock(iMutex);

    GpRAIIonDestruct callOnDestruct
    (
        [&aAtEndFn]()
        {
            aAtEndFn();
        }
    );

    aAtBeginFn();

    const bool checkFnRes = iCV.wait_for
    (
        lock.internal_lock(),
        std::chrono::milliseconds(aTimeout.As<ssize_t>()),
        std::move(aCheckFn)
    );

    if (checkFnRes) [[likely]]
    {
        return aConditionMetFn();
    }

    return aConditionNotMetFn();
}

}//GPlatform

#endif//#if defined(GP_USE_SYNC_PRIMITIVES)
