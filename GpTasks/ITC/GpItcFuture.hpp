#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpItcSharedCondition.hpp"
#include "GpItcResult.hpp"

namespace GPlatform {

class GpItcPromise;

class GpItcFuture
{
    friend class GpItcPromise;

public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcFuture)
    CLASS_DD(GpItcFuture)

    using ResultT       = std::optional<GpItcResult::SP>;
    using WaitForResT   = GpItcSharedCondition::WaitForConditionResT;

public:
                                        GpItcFuture     (void) noexcept = default;
                                        ~GpItcFuture    (void) noexcept = default;

    inline bool                         IsReady         (void) const noexcept;
    inline void                         Wait            (void);
    [[nodiscard]] inline WaitForResT    WaitFor         (const milliseconds_t aWaitTimeout);
    inline ResultT                      Result          (void) noexcept;

    template<typename T>
    static bool                         SCheckIfReady   (GpItcFuture::SP&                       aFuture,
                                                         std::function<void(T&&)>               aOnSuccessFn,
                                                         std::function<void(std::string_view)>  aOnErrorFn,
                                                         std::function<void()>                  aOnNullResFn);

private:
    inline void                         AddTaskGuid     (const GpUUID& aCurrentTaskGuid);
    inline void                         SetResultOnce   (GpItcResult::SP aResult);

private:
    GpItcSharedCondition                iItcSharedCondition;
    ResultT                             iResult;
};

bool    GpItcFuture::IsReady (void) const noexcept
{
    bool isReady = false;

    iItcSharedCondition.Do
    (
        [&]()
        {
            isReady = iResult.has_value();
        }
    );

    return isReady;
}

void    GpItcFuture::Wait (void)
{
    while (WaitFor(1.0_si_s) == WaitForResT::TIMEOUT)
    {
        //NOP
    }
}

[[nodiscard]] GpItcFuture::WaitForResT  GpItcFuture::WaitFor (const milliseconds_t aWaitTimeout)
{
    return iItcSharedCondition.WaitForCondition
    (
        aWaitTimeout,
        [&]()//Condition
        {
            return iResult.has_value();
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

GpItcFuture::ResultT    GpItcFuture::Result (void) noexcept
{
    ResultT res;

    iItcSharedCondition.Do
    (
        [&]()
        {
            res = iResult;
        }
    );

    return res;
}

template<typename T>
bool    GpItcFuture::SCheckIfReady
(
    GpItcFuture::SP&                        aFuture,
    std::function<void(T&&)>                aOnSuccessFn,
    std::function<void(std::string_view)>   aOnErrorFn,
    std::function<void()>                   aOnNullResFn
)
{
    if (aFuture.IsNULL())
    {
        return true;
    }

    GpItcFuture& future = aFuture.V();

    if (future.IsReady() == false)
    {
        return false;
    }

    GpItcFuture::ResultT futureRes = future.Result();
    GpItcResult::SExtract<T>
    (
        futureRes,
        aOnSuccessFn,
        aOnErrorFn,
        aOnNullResFn
    );

    return true;
}

void    GpItcFuture::AddTaskGuid (const GpUUID& aCurrentTaskGuid)
{
    iItcSharedCondition.AddTaskGuid(aCurrentTaskGuid);
}

void    GpItcFuture::SetResultOnce (GpItcResult::SP aResult)
{
    iItcSharedCondition.WakeupAll
    (
        [&, resultSrc = std::move(aResult)]()
        {
            if (   (iResult.has_value() == false)
                || (iResult.value().IsNULL()))
            {
                iResult = std::move(resultSrc);
            }
        }
    );
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
