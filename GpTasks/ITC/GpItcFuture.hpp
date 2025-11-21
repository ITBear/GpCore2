#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpTasks/ITC/GpItcConditionVar.hpp>
#include <GpCore2/GpTasks/ITC/GpItcResult.hpp>
#include <GpCore2/GpUtils/Other/GpMethodAccessGuard.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

template<typename T>
class GpItcPromise;

template<typename T>
class GpItcFuture;

template <typename T, typename... Ts>
concept IsGpItcFutureT = requires()
{
    requires std::is_base_of_v<GpItcFuture<Ts...>, T>;
};

template<typename T>
class GpItcFuture
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcFuture)
    CLASS_DD(GpItcFuture<T>)
    TAG_SET(THREAD_SAFE)

    using value_type    = T;
    using ItcResultT    = GpItcResult<T>;

public:
                        GpItcFuture     (void) noexcept;
                        ~GpItcFuture    (void) noexcept;

    void                Wait            (void);
    bool                WaitFor         (milliseconds_t aTimeout);

    bool                IsReady         (void) const noexcept;
    bool                IsReadyNoLock   (void) const noexcept REQUIRES(iItcCv.SpinLockRW());

    ItcResultT&         ResultNoLock    (void) REQUIRES(iItcCv.SpinLockRW());
    ItcResultT          ExtractResult   (void);

    template<typename R>
    [[nodiscard]] bool  SetResult       (R&& aResult,
                                         GpMethodAccessGuard<GpItcPromise<T>>);

    void                Subscribe       (GpWP<GpTask> aTaskWP);
    bool                Unsubscribe     (GpWP<GpTask> aTaskWP);

    GpSpinLockRW<>&     SpinLockRW      (void) const noexcept RETURN_CAPABILITY(iItcCv.SpinLockRW());

private:
    mutable GpItcConditionVar   iItcCv;
    ItcResultT                  iResult GUARDED_BY(iItcCv.SpinLockRW());
};

template<typename T>
GpItcFuture<T>::GpItcFuture (void) noexcept
{
}

template<typename T>
GpItcFuture<T>::~GpItcFuture (void) noexcept
{
}

template<typename T>
void    GpItcFuture<T>::Wait (void)
{
    iItcCv.Wait
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS
        {
            return !iResult.IsNotSet();
        }
    );
}

template<typename T>
bool    GpItcFuture<T>::WaitFor (const milliseconds_t aTimeout)
{
    return iItcCv.WaitFor
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS
        {
            return !iResult.IsNotSet();
        },
        aTimeout
    );
}

template<typename T>
bool    GpItcFuture<T>::IsReady (void) const noexcept
{
    GpSharedLock sharedLock{iItcCv.SpinLockRW()};

    return !iResult.IsNotSet();
}

template<typename T>
bool    GpItcFuture<T>::IsReadyNoLock (void) const noexcept
{
    return !iResult.IsNotSet();
}

template<typename T>
GpItcFuture<T>::ItcResultT& GpItcFuture<T>::ResultNoLock (void)
{
    return iResult;
}

template<typename T>
auto    GpItcFuture<T>::ExtractResult (void) -> ItcResultT
{
    GpUniqueLock uniqueLock{iItcCv.SpinLockRW()};

    VERIFY
    (
        IsReadyNoLock(),
        "Future has no result yet"
    );

    return std::move(iResult);
}

template<typename T>
template<typename R>
bool    GpItcFuture<T>::SetResult
(
    R&& aResult,
    GpMethodAccessGuard<GpItcPromise<T>>
)
{
    {
        GpUniqueLock uniqueLock{iItcCv.SpinLockRW()};

        if (IsReadyNoLock())
        {
            return false;
        }

        iResult = std::forward<R>(aResult);

        iItcCv.NotifyAll();
    }

    return true;
}

template<typename T>
void    GpItcFuture<T>::Subscribe (GpWP<GpTask> aTaskWP)
{
    iItcCv.Subscribe(aTaskWP);
}

template<typename T>
bool    GpItcFuture<T>::Unsubscribe (GpWP<GpTask> aTaskWP)
{
    return iItcCv.Unsubscribe(aTaskWP);
}

template<typename T>
GpSpinLockRW<>& GpItcFuture<T>::SpinLockRW (void) const noexcept
{
    return iItcCv.SpinLockRW();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
