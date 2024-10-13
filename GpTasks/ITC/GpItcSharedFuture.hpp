#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedCondition.hpp>
#include <GpCore2/GpTasks/ITC/GpItcResult.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

template<typename T>
class GpItcSharedPromise;

template<typename T>
class GpItcSharedFuture
{
    friend class GpItcSharedPromise<T>;

public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcSharedFuture)
    CLASS_DD(GpItcSharedFuture<T>)
    TAG_SET(THREAD_SAFE)

    using value_type    = T;
    using ItcResultT    = GpItcResult<T>;

public:
                                            GpItcSharedFuture   (void) noexcept = default;
                                            ~GpItcSharedFuture  (void) noexcept = default;

    bool                                    WaitFor             (milliseconds_t aTimeout);
    [[nodiscard]] std::optional<ItcResultT> TryGetResult        (void);
    bool                                    IsReady             (void) const noexcept;

private:
    bool                                    SetResult           (ItcResultT&& aResult);

private:
    mutable GpItcSharedCondition            iSC;
    std::optional<ItcResultT>               iResultOpt  GUARDED_BY(iSC.Mutex());
};

template<typename T>
bool    GpItcSharedFuture<T>::WaitFor (const milliseconds_t aTimeout)
{
    return iSC.WaitFor
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS
        {
            return iResultOpt.has_value();
        },
        aTimeout
    );
}

template<typename T>
std::optional<typename GpItcSharedFuture<T>::ItcResultT>    GpItcSharedFuture<T>::TryGetResult (void)
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};
    return std::move(iResultOpt);
}

template<typename T>
bool    GpItcSharedFuture<T>::IsReady (void) const noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};

    return iResultOpt.has_value();
}

template<typename T>
bool    GpItcSharedFuture<T>::SetResult (ItcResultT&& aResult)
{
    GpUniqueLock<GpMutex> uniqueLock{iSC.Mutex()};

    if (iResultOpt.has_value())
    {
        return false;
    }

    iResultOpt = std::move(aResult);

    iSC.NotifyAll();

    return true;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
