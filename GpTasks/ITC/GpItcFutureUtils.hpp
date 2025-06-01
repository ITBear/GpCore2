#pragma once

#include <GpCore2/GpTasks/ITC/GpItcFuture.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GP_TASKS_API GpItcFutureUtils
{
public:
    template<typename IsGpItcFutureT>
    using OnValueFnT        = std::function<void(typename IsGpItcFutureT::value_type&)>;
    using OnExceptionFnT    = std::function<void(const GpException&)>;

public:
    template<typename IsGpItcFutureT>
    [[nodiscard]] static bool   STryCheck               (IsGpItcFutureT&                    aFuture,
                                                         const OnValueFnT<IsGpItcFutureT>&  aOnValueFn,
                                                         const OnExceptionFnT&              aOnExceptionFn);
    template<typename IsGpItcFutureT>
    [[nodiscard]] static bool   SWaitFor                (IsGpItcFutureT&                    aFuture,
                                                         const OnValueFnT<IsGpItcFutureT>&  aOnValueFn,
                                                         const OnExceptionFnT&              aOnExceptionFn,
                                                         milliseconds_t                     aTimeout,
                                                         const std::function<void()>&       aOnTimeoutFn);
    template<typename IsGpItcFutureT>
    static void                 SWait                   (IsGpItcFutureT&                    aFuture,
                                                         const OnValueFnT<IsGpItcFutureT>&  aOnValueFn,
                                                         const OnExceptionFnT&              aOnExceptionFn);
    template<typename... FuturePacksT>
    static void                 SWaitAny                (FuturePacksT&...   aFuturePacks);

private:
    template <typename T>
    static bool                 SCheckFuturePackAny     (T&                 aFuturePack);

    template <typename T>
    static bool                 SCheckIfIsFulfilledAny  (T&                 aFuturePack);

    template <typename T, typename... FuturePacksT>
    static bool                 SCheckIfIsFulfilledAny  (T&                 aFuturePack,
                                                         FuturePacksT&...   aFuturePacks);

    static void                 SYield                  (milliseconds_t aTimeout);
    static void                 SYield                  (void);
};

template<typename IsGpItcFutureT>
bool    GpItcFutureUtils::STryCheck
(
    IsGpItcFutureT&                     aFuture,
    const OnValueFnT<IsGpItcFutureT>&   aOnValueFn,
    const OnExceptionFnT&               aOnExceptionFn
)
{
    if (aFuture.IsReady() == false)
    {
        return false;
    }

    auto resOpt = aFuture.TryGetResult();

    VERIFY
    (
        resOpt.has_value(),
        "Another task has retrieved the result using TryGetResultMove"
    );

    auto& res = resOpt.value();

    if (res.IsPayload()) [[likely]]
    {
        auto& payload = res.PayloadOrThrow();
        aOnValueFn(payload);
    } else
    {
        aOnExceptionFn(res.Exception());
    }

    return true;
}

template<typename IsGpItcFutureT>
bool    GpItcFutureUtils::SWaitFor
(
    IsGpItcFutureT&                     aFuture,
    const OnValueFnT<IsGpItcFutureT>&   aOnValueFn,
    const OnExceptionFnT&               aOnExceptionFn,
    const milliseconds_t                aTimeout,
    const std::function<void()>&        aOnTimeoutFn
)
{
    if (aFuture.WaitFor(aTimeout) == false)
    {
        if (aOnTimeoutFn)
        {
            aOnTimeoutFn();
        }

        return false;
    }

    auto resOpt = aFuture.TryGetResult();

    VERIFY
    (
        resOpt.has_value(),
        "Another task has retrieved the result using TryGetResultMove"
    );

    auto& res = resOpt.value();

    if (res.IsPayload()) [[likely]]
    {
        auto& payload = res.PayloadOrThrow();
        aOnValueFn(payload);
    } else
    {
        aOnExceptionFn(res.Exception());
    }

    return true;
}

template<typename IsGpItcFutureT>
void    GpItcFutureUtils::SWait
(
    IsGpItcFutureT&                     aFuture,
    const OnValueFnT<IsGpItcFutureT>&   aOnValueFn,
    const OnExceptionFnT&               aOnExceptionFn
)
{
    // Wait until get result
    aFuture.Wait();

    auto resOpt = aFuture.TryGetResult();

    VERIFY
    (
        resOpt.has_value(),
        "Another task has retrieved the result using TryGetResultMove"
    );

    auto& res = resOpt.value();

    if (res.IsPayload()) [[likely]]
    {
        auto& payload = res.PayloadOrThrow();
        aOnValueFn(payload);
    } else
    {
        aOnExceptionFn(res.Exception());
    }
}

template<typename... FuturePacksT>
void    GpItcFutureUtils::SWaitAny (FuturePacksT&... aFuturePacks)
{
    while (!SCheckIfIsFulfilledAny<FuturePacksT...>(aFuturePacks...))
    {
        SYield();
    }
}

template <typename T>
bool    GpItcFutureUtils::SCheckFuturePackAny (T& aFuturePack)
{
    auto& future = std::get<0>(aFuturePack);

    return STryCheck
    (
        future,
        std::get<1>(aFuturePack),   // OnSuccessFnT
        std::get<2>(aFuturePack)    // OnExceptionFnT
    );
}

template <typename T>
bool    GpItcFutureUtils::SCheckIfIsFulfilledAny (T& aFuturePack)
{
    return SCheckFuturePackAny<T>(aFuturePack);
}

template <typename T, typename... FuturePacksT>
bool    GpItcFutureUtils::SCheckIfIsFulfilledAny
(
    T&                      aFuturePack,
    FuturePacksT&...        aFuturePacks
)
{
    if (SCheckFuturePackAny<T>(aFuturePack))
    {
        return true;
    }

    return SCheckIfIsFulfilledAny(aFuturePacks...);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
