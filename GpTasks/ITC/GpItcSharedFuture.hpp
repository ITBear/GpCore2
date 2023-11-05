#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpItcSharedCondition.hpp"
#include "GpItcResult.hpp"

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

    using value_type        = T;
    using ItcResultT        = GpItcResult<T>;

public:
                                            GpItcSharedFuture   (void) noexcept = default;
                                            ~GpItcSharedFuture  (void) noexcept = default;

    bool                                    WaitFor             (const milliseconds_t   aTimeout);
    [[nodiscard]] std::optional<ItcResultT> TryGetResult        (void);
    bool                                    IsReady             (void) const noexcept;

    static bool                             SCheckIfReady       (GpItcSharedFuture&                             aFuture,
                                                                 const std::function<void(T&)>&                 aOnValueFn,
                                                                 const std::function<void(const GpException&)>& aOnExceptionFn);

private:
    bool                                    SetResult           (ItcResultT&& aResult);

private:
    mutable GpItcSharedCondition            iSC;
    std::optional<ItcResultT>               iResultOpt  GUARDED_BY(iSC.Mutex());
    bool                                    iIsSet      GUARDED_BY(iSC.Mutex()) = false;
};

template<typename T>
bool    GpItcSharedFuture<T>::WaitFor (const milliseconds_t aTimeout)
{
    return iSC.WaitFor
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS
        {
            return iIsSet;
        },
        aTimeout
    );
}

template<typename T>
std::optional<typename GpItcSharedFuture<T>::ItcResultT>    GpItcSharedFuture<T>::TryGetResult (void)
{
    GpUniqueLock<GpMutex> lock(iSC.Mutex());
    return std::move(iResultOpt);
}

template<typename T>
bool    GpItcSharedFuture<T>::IsReady (void) const noexcept
{
    GpUniqueLock<GpMutex> lock(iSC.Mutex());
    return iIsSet;
}

template<typename T>
bool    GpItcSharedFuture<T>::SCheckIfReady
(
    GpItcSharedFuture&                              aFuture,
    const std::function<void(T&)>&                  aOnValueFn,
    const std::function<void(const GpException&)>&  aOnExceptionFn
)
{
    std::optional<ItcResultT> resOpt = aFuture.TryGetResult();

    if (!resOpt.has_value()) [[likely]]
    {
        return false;
    }

    ItcResultT& res = resOpt.value();

    if (res.IsPayload()) [[likely]]
    {
        T& payload = res.PayloadOrThrow();
        aOnValueFn(payload);
    } else
    {
        aOnExceptionFn(res.Exception());
    }

    return true;
}

template<typename T>
bool    GpItcSharedFuture<T>::SetResult (ItcResultT&& aResult)
{
    GpUniqueLock<GpMutex> lock(iSC.Mutex());

    if (iIsSet)
    {
        return false;
    }

    iResultOpt  = std::move(aResult);
    iIsSet      = true;

    iSC.NotifyAll();

    return true;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
