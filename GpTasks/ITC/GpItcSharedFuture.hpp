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

    using value_type        = T;
    using ItcResultT        = GpItcResult<T>;
    using ItcResultOptValT  = typename ItcResultT::C::Opt::Val;
    using ItcResultOptRefT  = typename ItcResultT::C::Opt::Ref;

    using OnSuccessFnT      = typename ItcResultT::OnSuccessFnT;
    using OnEmptyFnT        = typename ItcResultT::OnEmptyFnT;
    using OnExceptionFnT    = typename ItcResultT::OnExceptionFnT;

public:
                            GpItcSharedFuture   (void) noexcept = default;
                            ~GpItcSharedFuture  (void) noexcept = default;

    bool                    IsReady             (void) const noexcept;
    bool                    WaitFor             (const milliseconds_t aWaitTimeout);
    ItcResultOptRefT        Result              (void) noexcept;

    static bool             SCheckIfReady       (this_type&             aFuture,
                                                 const OnSuccessFnT&    aOnSuccessFn,
                                                 const OnEmptyFnT&      aOnEmptyFn,
                                                 const OnExceptionFnT&  aOnExceptionFn);

private:
    bool                    SetResult           (ItcResultT&& aResult);
    bool                    SetEmptyResult      (void);

private:
    GpItcSharedCondition    iSharedCondition;
    ItcResultOptValT        iResultOpt;
};

template<typename T>
bool    GpItcSharedFuture<T>::IsReady (void) const noexcept
{
    bool isReady = false;

    iSharedCondition.DoAtomic
    (
        [&](std::mutex&)
        {
            isReady = iResultOpt.has_value();
        }
    );

    return isReady;
}

template<typename T>
bool    GpItcSharedFuture<T>::WaitFor (const milliseconds_t aWaitTimeout)
{
    return iSharedCondition.WaitFor
    (
        aWaitTimeout,
        [&](std::mutex&)// Condition
        {
            return iResultOpt.has_value();
        }
    );
}

template<typename T>
typename GpItcSharedFuture<T>::ItcResultOptRefT GpItcSharedFuture<T>::Result (void) noexcept
{
    ItcResultOptRefT res;

    iSharedCondition.DoAtomic
    (
        [&](std::mutex&)
        {
            if (iResultOpt.has_value())
            {
                res = ItcResultOptRefT(iResultOpt.value());
            } else
            {
                res = std::nullopt;
            }
        }
    );

    return res;
}

template<typename T>
bool    GpItcSharedFuture<T>::SCheckIfReady
(
    this_type&              aFuture,
    const OnSuccessFnT&     aOnSuccessFn,
    const OnEmptyFnT&       aOnEmptyFn,
    const OnExceptionFnT&   aOnExceptionFn
)
{
    bool isReady = false;

    aFuture.iSharedCondition.DoAtomic
    (
        [&aFuture, &isReady, &aOnSuccessFn, &aOnEmptyFn, &aOnExceptionFn](std::mutex&)
        {
            isReady = aFuture.iResultOpt.has_value();
            if (isReady == false)
            {
                return;
            }

            ItcResultT& result = aFuture.iResultOpt.value();

            ItcResultT::SExtract
            (
                result,
                aOnSuccessFn,
                aOnEmptyFn,
                aOnExceptionFn
            );
        }
    );

    return isReady;
}

template<typename T>
bool    GpItcSharedFuture<T>::SetResult (ItcResultT&& aResult)
{
    bool isSet = false;

    GpItcSharedCondition::AtomicFnT fn = [&, resultSrc = std::move(aResult)](std::mutex&)
    {
        if (iResultOpt.has_value() == false)
        {
            iResultOpt  = std::move(resultSrc);
            isSet       = true;
        }
    };

    iSharedCondition.Notify
    (
        GpItcSharedCondition::NotifyMode::ALL,
        fn
    );

    return isSet;
}

template<typename T>
bool    GpItcSharedFuture<T>::SetEmptyResult (void)
{
    bool isSet = false;

    GpItcSharedCondition::AtomicFnT fn = [&](std::mutex&)
    {
        if (iResultOpt.has_value() == false)
        {
            iResultOpt  = ItcResultT();
            isSet       = true;
        }
    };

    iSharedCondition.Notify
    (
        GpItcSharedCondition::NotifyMode::ALL,
        fn
    );

    return isSet;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
