#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpItcSharedCondition.hpp"
#include "GpItcResult.hpp"

namespace GPlatform {

template<typename T>
class GpItcPromise;

template<typename T>
class GpItcFuture
{
    friend class GpItcPromise<T>;

public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcFuture)
    CLASS_DD(GpItcFuture<T>)

    using ItcResultT    = GpItcResult<T>;
    using ItcResultOptT = std::optional<typename ItcResultT::SP>;
    using WaitForResT   = GpItcSharedCondition::WaitForConditionResT;

public:
                                        GpItcFuture     (void) noexcept = default;
                                        ~GpItcFuture    (void) noexcept = default;

    inline bool                         IsReady         (void) const noexcept;
    inline void                         Wait            (void);
    [[nodiscard]] inline WaitForResT    WaitFor         (const milliseconds_t aWaitTimeout);
    inline ItcResultOptT                Result          (void) noexcept;

    static bool                         SCheckIfReady   (this_type::SP&                                 aFuture,
                                                         typename ItcResultT::ExtractFnT_OnSuccess      aOnSuccessFn,
                                                         typename ItcResultT::ExtractFnT_OnException    aOnExceptionFn);

private:
    inline void                         AddTaskGuid     (const GpUUID& aCurrentTaskGuid);
    inline void                         SetResultOnce   (typename ItcResultT::SP aResult);

private:
    GpItcSharedCondition                iItcSharedCondition;
    ItcResultOptT                       iResultOpt;
};

template<typename T>
bool    GpItcFuture<T>::IsReady (void) const noexcept
{
    bool isReady = false;

    iItcSharedCondition.Do
    (
        [&]()
        {
            isReady = iResultOpt.has_value();
        }
    );

    return isReady;
}

template<typename T>
void    GpItcFuture<T>::Wait (void)
{
    while (WaitFor(1.0_si_s) == WaitForResT::TIMEOUT)
    {
        //NOP
    }
}

template<typename T>
[[nodiscard]] typename GpItcFuture<T>::WaitForResT  GpItcFuture<T>::WaitFor (const milliseconds_t aWaitTimeout)
{
    return iItcSharedCondition.WaitForCondition
    (
        aWaitTimeout,
        [&]()//Condition
        {
            return iResultOpt.has_value();
        },
        []()//Action on condition met
        {
        },
        []()//Action on condition not met
        {
            return GpItcSharedCondition::ActionCondNotMetRes::CONTINUE;
        }
    );
}

template<typename T>
typename GpItcFuture<T>::ItcResultOptT  GpItcFuture<T>::Result (void) noexcept
{
    ItcResultOptT res;

    iItcSharedCondition.Do
    (
        [&]()
        {
            res = iResultOpt;
        }
    );

    return res;
}

template<typename T>
bool    GpItcFuture<T>::SCheckIfReady
(
    this_type::SP&                              aFuture,
    typename ItcResultT::ExtractFnT_OnSuccess   aOnSuccessFn,
    typename ItcResultT::ExtractFnT_OnException aOnExceptionFn
)
{
    if (aFuture.IsNULL())
    {
        return true;
    }

    this_type& future = aFuture.V();

    if (future.IsReady() == false)
    {
        return false;
    }

    ItcResultOptT futureRes = future.Result();
    ItcResultT::SExtract
    (
        futureRes,
        aOnSuccessFn,
        aOnExceptionFn
    );

    return true;
}

template<typename T>
void    GpItcFuture<T>::AddTaskGuid (const GpUUID& aCurrentTaskGuid)
{
    iItcSharedCondition.AddTaskGuid(aCurrentTaskGuid);
}

template<typename T>
void    GpItcFuture<T>::SetResultOnce (typename ItcResultT::SP aResult)
{
    iItcSharedCondition.WakeupAll
    (
        [&, resultSrc = std::move(aResult)]()
        {
            if (   (iResultOpt.has_value() == false)
                || (iResultOpt.value().IsNULL()))
            {
                iResultOpt = std::move(resultSrc);
            }
        }
    );
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
