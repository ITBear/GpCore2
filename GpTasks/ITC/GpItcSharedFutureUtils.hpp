#pragma once

#include <GpCore2/GpTasks/ITC/GpItcSharedFuture.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpItcSharedFutureUtils
{
public:
    template<typename SharedFutureT>
    static void                 SCheck                  (SharedFutureT&                                                     aFuture,
                                                         const std::function<void(typename SharedFutureT::value_type&)>&    aOnValueFn,
                                                         const std::function<void(const GpException&)>&                     aOnExceptionFn);
    template<typename SharedFutureT>
    [[nodiscard]] static bool   STryCheck               (SharedFutureT&                                                     aFuture,
                                                         const std::function<void(typename SharedFutureT::value_type&)>&    aOnValueFn,
                                                         const std::function<void(const GpException&)>&                     aOnExceptionFn);
    template<typename SharedFutureT>
    [[nodiscard]] static bool   SWaitFor                (SharedFutureT&                                                     aFuture,
                                                         const std::function<void(typename SharedFutureT::value_type&)>&    aOnValueFn,
                                                         const std::function<void(const GpException&)>&                     aOnExceptionFn,
                                                         milliseconds_t                                                     aTimeout,
                                                         const std::function<void()>&                                       aOnTimeoutFn);
    template<typename SharedFutureT>
    static void                 SWaitForInf             (SharedFutureT&                                                     aFuture,
                                                         const std::function<void(typename SharedFutureT::value_type&)>&    aOnValueFn,
                                                         const std::function<void(const GpException&)>&                     aOnExceptionFn,
                                                         milliseconds_t                                                     aCheckPeriod);
    template<typename... FuturePacksT>
    static void                 SWaitForInfAny          (milliseconds_t     aCheckPeriod,
                                                         FuturePacksT&...   aFuturePacks);

private:
    template <typename T>
    static bool                 SCheckFuturePackAny     (milliseconds_t     aTimeout,
                                                         T&                 aFuturePack);

    template <typename T>
    static bool                 SCheckIfIsFulfilledAny  (milliseconds_t     aCheckPeriod,
                                                         T&                 aFuturePack);

    template <typename T, typename... FuturePacksT>
    static bool                 SCheckIfIsFulfilledAny  (milliseconds_t     aCheckPeriod,
                                                         T&                 aFuturePack,
                                                         FuturePacksT&...   aFuturePacks);
};

template<typename SharedFutureT>
void    GpItcSharedFutureUtils::SCheck
(
    SharedFutureT&                                                  aFuture,
    const std::function<void(typename SharedFutureT::value_type&)>& aOnValueFn,
    const std::function<void(const GpException&)>&                  aOnExceptionFn
)
{
    auto resOpt = aFuture.TryGetResult();

    THROW_COND_GP
    (
        resOpt.has_value(),
        "The future is not fulfilled"
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

template<typename SharedFutureT>
bool    GpItcSharedFutureUtils::STryCheck
(
    SharedFutureT&                                                  aFuture,
    const std::function<void(typename SharedFutureT::value_type&)>& aOnValueFn,
    const std::function<void(const GpException&)>&                  aOnExceptionFn
)
{
    auto resOpt = aFuture.TryGetResult();

    if (!resOpt.has_value())
    {
        return false;
    }

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

template<typename SharedFutureT>
bool    GpItcSharedFutureUtils::SWaitFor
(
    SharedFutureT&                                                  aFuture,
    const std::function<void(typename SharedFutureT::value_type&)>& aOnValueFn,
    const std::function<void(const GpException&)>&                  aOnExceptionFn,
    const milliseconds_t                                            aTimeout,
    const std::function<void()>&                                    aOnTimeoutFn
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

    THROW_COND_GP
    (
        resOpt.has_value(),
        "The future is not fulfilled"
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

template<typename SharedFutureT>
void    GpItcSharedFutureUtils::SWaitForInf
(
    SharedFutureT&                                                  aFuture,
    const std::function<void(typename SharedFutureT::value_type&)>& aOnValueFn,
    const std::function<void(const GpException&)>&                  aOnExceptionFn,
    const milliseconds_t                                            aCheckPeriod
)
{
    // Wait until get result
    while (!aFuture.WaitFor(aCheckPeriod))
    {
        // NOP
    }

    auto resOpt = aFuture.TryGetResult();

    THROW_COND_GP
    (
        resOpt.has_value(),
        "The future is not fulfilled"
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
void    GpItcSharedFutureUtils::SWaitForInfAny
(
    const milliseconds_t    aCheckPeriod,
    FuturePacksT&...        aFuturePacks
)
{
    while (!SCheckIfIsFulfilledAny<FuturePacksT...>(aCheckPeriod, aFuturePacks...))
    {
        // NOP
    }
}

template <typename T>
bool    GpItcSharedFutureUtils::SCheckFuturePackAny
(
    const milliseconds_t    aTimeout,
    T&                      aFuturePack
)
{
    auto& future = std::get<0>(aFuturePack);

    return SWaitFor
    (
        future,
        std::get<1>(aFuturePack),   // OnSuccessFnT
        std::get<2>(aFuturePack),   // OnExceptionFnT
        aTimeout,
        {}
    );
}

template <typename T>
bool    GpItcSharedFutureUtils::SCheckIfIsFulfilledAny
(
    const milliseconds_t    aCheckPeriod,
    T&                      aFuturePack
)
{
    return SCheckFuturePackAny<T>(aCheckPeriod, aFuturePack);
}

template <typename T, typename... FuturePacksT>
bool    GpItcSharedFutureUtils::SCheckIfIsFulfilledAny
(
    const milliseconds_t    aCheckPeriod,
    T&                      aFuturePack,
    FuturePacksT&...        aFuturePacks
)
{
    if (SCheckFuturePackAny<T>(aCheckPeriod, aFuturePack))
    {
        return true;
    }

    return SCheckIfIsFulfilledAny(aCheckPeriod, aFuturePacks...);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
