#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_set.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpConditionVar.hpp>
#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtx.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GP_TASKS_API CAPABILITY("GpItcCondition") GpItcCondition
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcCondition)
    CLASS_DD(GpItcCondition)
    TAG_SET(THREAD_SAFE)

    using FiberTaskIDsT = boost::container::small_flat_set<GpTaskId, 8>;
    using TaskInfo      = std::tuple<GpTaskMode::EnumT, GpTaskId>;
    using AtBeginFnT    = std::function<void()>;
    using AtEndFnT      = std::function<void(bool)>;// must be noexcept
    using CheckFnT      = std::function<bool()>;

public:
                            GpItcCondition      (void) noexcept = default;
                            ~GpItcCondition     (void) noexcept = default;

    GpMutex&                Mutex               (void) noexcept RETURN_CAPABILITY(iThreadsCV.Mutex()) {return iThreadsCV.Mutex();}
    bool                    NotifyOne           (void) REQUIRES(iThreadsCV.Mutex());
    bool                    NotifyAll           (void) REQUIRES(iThreadsCV.Mutex());

    inline void             SubscribeAsFiber    (GpTaskId aGpTaskId);
    inline bool             UnsubscribeAsFiber  (GpTaskId aGpTaskId);

    inline void             Wait                (const CheckFnT&    aCheckFn);  
    inline void             Wait                (const CheckFnT&    aCheckFn,
                                                 const AtBeginFnT&  aAtBeginFn,
                                                 const AtEndFnT&    aAtEndFn);

    inline bool             WaitFor             (const CheckFnT&    aCheckFn,
                                                 milliseconds_t     aTimeout);
    inline bool             WaitFor             (const CheckFnT&    aCheckFn,
                                                 milliseconds_t     aTimeout,
                                                 const AtBeginFnT&  aAtBeginFn,
                                                 const AtEndFnT&    aAtEndFn);

private:
    inline bool             WaitForFiber        (const CheckFnT&    aCheckFn,
                                                 milliseconds_t     aTimeout,
                                                 const AtBeginFnT&  aAtBeginFn,
                                                 const AtEndFnT&    aAtEndFn,
                                                 GpTaskId           aFiberTaskId);
    inline bool             WaitForThread       (const CheckFnT&    aCheckFn,
                                                 milliseconds_t     aTimeout,
                                                 const AtBeginFnT&  aAtBeginFn,
                                                 const AtEndFnT&    aAtEndFn);

    static TaskInfo         SCurrentTaskInfo    (void);
    static void             SYield              (milliseconds_t aTimeout);
    static void             SYield              (void);

private:
    // For waiting threads
    mutable GpConditionVar  iThreadsCV;
    size_t                  iThreadsWaiting GUARDED_BY(iThreadsCV.Mutex());

    // For waiting fiber tasks
    FiberTaskIDsT           iFiberTaskIDs GUARDED_BY(iThreadsCV.Mutex());
};

void    GpItcCondition::SubscribeAsFiber (GpTaskId aGpTaskId)
{
    GpUniqueLock<GpMutex> uniqueLock{iThreadsCV.Mutex()};

    iFiberTaskIDs.insert(aGpTaskId);
}

bool    GpItcCondition::UnsubscribeAsFiber (GpTaskId aGpTaskId)
{
    GpUniqueLock<GpMutex> uniqueLock{iThreadsCV.Mutex()};

    return iFiberTaskIDs.erase(aGpTaskId) > 0;
}

void    GpItcCondition::Wait (const CheckFnT& aCheckFn)
{
    WaitFor(aCheckFn, 0.0_si_ms);
}

void    GpItcCondition::Wait
(
    const CheckFnT&     aCheckFn,
    const AtBeginFnT&   aAtBeginFn,
    const AtEndFnT&     aAtEndFn
)
{
    WaitFor
    (
        aCheckFn,
        0.0_si_ms,
        aAtBeginFn,
        aAtEndFn
    );
}

bool    GpItcCondition::WaitFor
(
    const CheckFnT&         aCheckFn,
    const milliseconds_t    aTimeout
)
{
    static const AtBeginFnT atBeginFn   = [](){};
    static const AtEndFnT   atEndFn     = [](bool){};

    return WaitFor
    (
        aCheckFn,
        aTimeout,
        atBeginFn,
        atEndFn
    );
}

bool    GpItcCondition::WaitFor
(
    const CheckFnT&         aCheckFn,
    const milliseconds_t    aTimeout,
    const AtBeginFnT&       aAtBeginFn,
    const AtEndFnT&         aAtEndFn
)
{
    // Collect current task info
    const auto[taskMode, taskId] = SCurrentTaskInfo();

    if (taskMode == GpTaskMode::FIBER) [[likely]]
    {
        return WaitForFiber
        (
            aCheckFn,
            aTimeout,
            aAtBeginFn,
            aAtEndFn,
            taskId
        );
    } else // taskMode == GpTaskMode::THREAD
    {
        return WaitForThread
        (
            aCheckFn,
            aTimeout,
            aAtBeginFn,
            aAtEndFn
        );
    }
}

bool    GpItcCondition::WaitForFiber
(
    const CheckFnT&         aCheckFn,
    const milliseconds_t    aTimeout,
    const AtBeginFnT&       aAtBeginFn,
    const AtEndFnT&         aAtEndFn,
    const GpTaskId          aFiberTaskId
)
{
    const milliseconds_t    startTs                 = GpDateTimeOps::SSteadyTS_ms();
    bool                    isTaskIdRegistered      = false;
    bool                    isAtBeginFnCalled       = false;
    const bool              isNeedToCheckTimeout    = aTimeout > 0.0_si_ms;
    bool                    result                  = false;

    GpRAIIonDestruct callOnDestruct = [&]()
    {
        GpUniqueLock<GpMutex> uniqueLock{iThreadsCV.Mutex()};

        if (isTaskIdRegistered) [[likely]]
        {
            iFiberTaskIDs.erase(aFiberTaskId);
        }

        aAtEndFn(result);
    };

    while (true)
    {
        milliseconds_t passedTime = 0.0_si_ms;

        {
            GpUniqueLock<GpMutex> uniqueLock{iThreadsCV.Mutex()};

            // At begin check
            if (!isAtBeginFnCalled) [[unlikely]]
            {
                aAtBeginFn();
                isAtBeginFnCalled = true;
            }

            // Check condition
            if (aCheckFn()) [[unlikely]]
            {
                result = true;
                return result;
            }

            // Check if fiber task id is registered into iFiberTaskIDs
            if (!isTaskIdRegistered) [[unlikely]]
            {
                iFiberTaskIDs.emplace(aFiberTaskId);
                isTaskIdRegistered = true;
            }

            // Check timeout
            if (isNeedToCheckTimeout)
            {
                passedTime = GpDateTimeOps::SSteadyTS_ms() - startTs;
                if (passedTime >= aTimeout) [[unlikely]]
                {
                    result = false;
                    return result;
                }
            }
        }

        // Wait for
        if (isNeedToCheckTimeout)
        {
            SYield(aTimeout - passedTime);
        } else
        {
            SYield();
        }
    }

    // Never reaches here
    result = false;
    return result;
}

bool    GpItcCondition::WaitForThread
(
    const CheckFnT&         aCheckFn,
    const milliseconds_t    aTimeout,
    const AtBeginFnT&       aAtBeginFn,
    const AtEndFnT&         aAtEndFn
)
{
    const AtBeginFnT atBeginFn = [&](void) NO_THREAD_SAFETY_ANALYSIS
    {
        iThreadsWaiting++;
        aAtBeginFn();
    };

    const AtEndFnT atEndFn = [&](bool aResult) NO_THREAD_SAFETY_ANALYSIS
    {
        iThreadsWaiting--;
        aAtEndFn(aResult);
    };

    return iThreadsCV.WaitFor
    (
        aCheckFn,
        aTimeout,
        atBeginFn,
        atEndFn
    );
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
