#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpTasks/ITC/GpItcCondition.hpp>
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
                                            GpItcFuture         (void) noexcept = default;
                                            ~GpItcFuture        (void) noexcept = default;

    void                                    Wait                (void);
    bool                                    WaitFor             (milliseconds_t aTimeout);
    [[nodiscard]] std::optional<ItcResultT> TryGetResultCopy    (void);
    [[nodiscard]] std::optional<ItcResultT> TryGetResultMove    (void);
    bool                                    IsReady             (void) const noexcept;

    inline void                             SubscribeAsFiber    (GpTaskId aGpTaskId);
    inline bool                             UnsubscribeAsFiber  (GpTaskId aGpTaskId);

    template<typename R>
    [[nodiscard]] bool                      SetResult           (R&& aResult,
                                                                 GpMethodAccessGuard<GpItcPromise<T>>);

private:
    mutable GpItcCondition      iItcCondition;
    std::optional<ItcResultT>   iResultOpt  GUARDED_BY(iItcCondition.Mutex());
    bool                        iIsReady    GUARDED_BY(iItcCondition.Mutex())  = false;
};

template<typename T>
void    GpItcFuture<T>::Wait (void)
{
    iItcCondition.Wait
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS
        {
            return iIsReady;
        }
    );
}

template<typename T>
bool    GpItcFuture<T>::WaitFor (const milliseconds_t   aTimeout)
{
    return iItcCondition.WaitFor
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS
        {
            return iIsReady;
        },
        aTimeout
    );
}

template<typename T>
auto    GpItcFuture<T>::TryGetResultCopy (void) -> std::optional<ItcResultT>
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    return iResultOpt;
}

template<typename T>
auto    GpItcFuture<T>::TryGetResultMove (void) -> std::optional<ItcResultT>
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    return std::move(iResultOpt);
}

template<typename T>
bool    GpItcFuture<T>::IsReady (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    return iIsReady;
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
    GpUniqueLock<GpMutex> uniqueLock{iItcCondition.Mutex()};

    if (iIsReady)
    {
        return false;
    }

    iResultOpt  = std::forward<R>(aResult);
    iIsReady    = true;

    iItcCondition.NotifyAll();

    return true;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
