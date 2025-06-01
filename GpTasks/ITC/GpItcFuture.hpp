#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpTasks/ITC/GpItcCondition.hpp>
#include <GpCore2/GpTasks/ITC/GpItcResult.hpp>
#include <GpCore2/GpUtils/Other/GpMethodAccessGuard.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSharedMutex.hpp>

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
                                            GpItcFuture         (void) noexcept = default;
                                            ~GpItcFuture        (void) noexcept = default;

    void                                    Wait                (void);
    bool                                    WaitFor             (milliseconds_t aTimeout);
    [[nodiscard]] std::optional<ItcResultT> TryGetResult        (void);
    bool                                    IsReady             (void) const noexcept;

    inline void                             SubscribeAsFiber    (GpTaskId aGpTaskId);
    inline bool                             UnsubscribeAsFiber  (GpTaskId aGpTaskId);

    template<typename R>
    [[nodiscard]] bool                      SetResult           (R&& aResult,
                                                                 GpMethodAccessGuard<GpItcPromise<T>>);

private:
    mutable GpItcCondition      iItcCondition;
    std::optional<ItcResultT>   iResultOpt GUARDED_BY(iItcCondition.SpinLock());
};

template<typename T>
void    GpItcFuture<T>::Wait (void)
{
    iItcCondition.Wait
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS
        {
            return iResultOpt.has_value();
        }
    );
}

template<typename T>
bool    GpItcFuture<T>::WaitFor (const milliseconds_t aTimeout)
{
    return iItcCondition.WaitFor
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS
        {
            return iResultOpt.has_value();
        },
        aTimeout
    );
}

template<typename T>
auto    GpItcFuture<T>::TryGetResult (void) -> std::optional<ItcResultT>
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iItcCondition.SpinLock()};

    return iResultOpt;
}

template<typename T>
bool    GpItcFuture<T>::IsReady (void) const noexcept
{   
    GpSharedLock<GpSpinLockRW> sharedLock{iItcCondition.SpinLock()};

    return iResultOpt.has_value();
}

template<typename T>
void    GpItcFuture<T>::SubscribeAsFiber (GpTaskId aGpTaskId)
{
    iItcCondition.SubscribeAsFiber(aGpTaskId);
}

template<typename T>
bool    GpItcFuture<T>::UnsubscribeAsFiber (GpTaskId aGpTaskId)
{
    return iItcCondition.UnsubscribeAsFiber(aGpTaskId);
}

template<typename T>
template<typename R>
bool    GpItcFuture<T>::SetResult
(
    R&& aResult,
    GpMethodAccessGuard<GpItcPromise<T>>
)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iItcCondition.SpinLock()};

    if (iResultOpt.has_value())
    {
        return false;
    }

    iResultOpt = std::forward<R>(aResult);

    iItcCondition.NotifyAll();

    return true;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
