#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../GpTasks_global.hpp"

#include "../../GpUtils/Macro/GpMacroClass.hpp"
#include "../../GpUtils/Types/Containers/GpContainersT.hpp"
#include "../../GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp"
#include "../../GpUtils/SyncPrimitives/GpConditionVar.hpp"
#include "../../GpUtils/DateTime/GpDateTimeOps.hpp"
#include "../GpTaskEnums.hpp"

#include <boost/container/flat_set.hpp>
//#include <boost/container/small_vector.hpp>

namespace GPlatform {

class GP_TASKS_API CAPABILITY("GpItcSharedCondition") GpItcSharedCondition
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcSharedCondition)
    CLASS_DD(GpItcSharedCondition)
    TAG_SET(THREAD_SAFE)

    using FiberTaskIDsT         = boost::container::flat_set<u_int_64/*, std::less<u_int_64>, boost::container::small_vector<u_int_64, 8>*/>;
    using TaskInfo              = std::tuple<GpTaskMode::EnumT, GpTaskId>;
    using AtBeginFnT            = std::function<void()>;
    using AtEndFnT              = std::function<void()>;// must be noexcept 
    using CheckFnT              = std::function<bool()>;

    template<typename T>
    using ConditionMetFnT       = std::function<std::optional<T>()>;

    template<typename T>
    using ConditionNotMetFnT    = std::function<std::optional<T>()>;

public:
                            GpItcSharedCondition    (void) noexcept = default;
                            ~GpItcSharedCondition   (void) noexcept = default;

    GpMutex&                Mutex                   (void) noexcept RETURN_CAPABILITY(iThreadsCV.Mutex()) {return iThreadsCV.Mutex();}
    void                    NotifyOne               (void) REQUIRES(iThreadsCV.Mutex());
    void                    NotifyAll               (void) REQUIRES(iThreadsCV.Mutex());

    inline bool             WaitFor                 (CheckFnT               aCheckFn,
                                                     const milliseconds_t   aTimeout);
    template<typename T>
    std::optional<T>        WaitFor                 (CheckFnT               aCheckFn,
                                                     ConditionMetFnT<T>     aConditionMetFn,
                                                     const milliseconds_t   aTimeout);
    template<typename T>
    std::optional<T>        WaitFor                 (AtBeginFnT             aAtBeginFn,
                                                     AtEndFnT               aAtEndFn,
                                                     CheckFnT               aCheckFn,
                                                     ConditionMetFnT<T>     aConditionMetFn,
                                                     ConditionNotMetFnT<T>  aConditionNotMetFn,
                                                     const milliseconds_t   aTimeout);

private:
    inline bool             WaitForFiber            (CheckFnT               aCheckFn,
                                                     const milliseconds_t   aTimeout,
                                                     const u_int_64         aFiberTaskId);
    template<typename T>
    std::optional<T>        WaitForFiber            (CheckFnT               aCheckFn,
                                                     ConditionMetFnT<T>     aConditionMetFn,
                                                     const milliseconds_t   aTimeout,
                                                     const u_int_64         aFiberTaskId);
    template<typename T>
    std::optional<T>        WaitForFiber            (AtBeginFnT             aAtBeginFn,
                                                     AtEndFnT               aAtEndFn,
                                                     CheckFnT               aCheckFn,
                                                     ConditionMetFnT<T>     aConditionMetFn,
                                                     ConditionNotMetFnT<T>  aConditionNotMetFn,
                                                     const milliseconds_t   aTimeout,
                                                     const u_int_64         aFiberTaskId);

    inline bool             WaitForThread           (CheckFnT               aCheckFn,
                                                     const milliseconds_t   aTimeout);
    template<typename T>
    std::optional<T>        WaitForThread           (CheckFnT               aCheckFn,
                                                     ConditionMetFnT<T>     aConditionMetFn,
                                                     const milliseconds_t   aTimeout);
    template<typename T>
    std::optional<T>        WaitForThread           (AtBeginFnT             aAtBeginFn,
                                                     AtEndFnT               aAtEndFn,
                                                     CheckFnT               aCheckFn,
                                                     ConditionMetFnT<T>     aConditionMetFn,
                                                     ConditionNotMetFnT<T>  aConditionNotMetFn,
                                                     const milliseconds_t   aTimeout);

    static TaskInfo         SCurrentTaskInfo        (void);
    static void             SYeld                   (const milliseconds_t aTimeout);

private:
    // For waiting threads
    mutable GpConditionVar  iThreadsCV;
    size_t                  iThreadsWaiting GUARDED_BY(iThreadsCV.Mutex());

    // For waiting fiber tasks
    FiberTaskIDsT           iFiberTaskIDs GUARDED_BY(iThreadsCV.Mutex());
};

bool    GpItcSharedCondition::WaitFor
(
    CheckFnT                aCheckFn,
    const milliseconds_t    aTimeout
)
{
    // Collect current task info
    const auto[taskMode, taskId] = SCurrentTaskInfo();

    if (taskMode == GpTaskMode::FIBER) [[likely]]
    {
        return WaitForFiber
        (
            std::move(aCheckFn),
            aTimeout,
            taskId
        );
    } else // taskMode == GpTaskMode::THREAD
    {
        return WaitForThread
        (
            std::move(aCheckFn),
            aTimeout
        );
    }
}

template<typename T>
std::optional<T>    GpItcSharedCondition::WaitFor
(
    CheckFnT                aCheckFn,
    ConditionMetFnT<T>      aConditionMetFn,
    const milliseconds_t    aTimeout
)
{
    // Collect current task info
    const auto[taskMode, taskId] = SCurrentTaskInfo();

    if (taskMode == GpTaskMode::FIBER) [[likely]]
    {
        return WaitForFiber<T>
        (
            std::move(aCheckFn),
            std::move(aConditionMetFn),
            aTimeout,
            taskId
        );
    } else // taskMode == GpTaskMode::THREAD
    {
        return WaitForThread<T>
        (
            std::move(aCheckFn),
            std::move(aConditionMetFn),
            aTimeout
        );
    }
}

template<typename T>
std::optional<T>    GpItcSharedCondition::WaitFor
(
    AtBeginFnT              aAtBeginFn,
    AtEndFnT                aAtEndFn,
    CheckFnT                aCheckFn,
    ConditionMetFnT<T>      aConditionMetFn,
    ConditionNotMetFnT<T>   aConditionNotMetFn,
    const milliseconds_t    aTimeout
)
{
    // Collect current task info
    const auto[taskMode, taskId] = SCurrentTaskInfo();

    if (taskMode == GpTaskMode::FIBER) [[likely]]
    {
        return WaitForFiber<T>
        (
            std::move(aAtBeginFn),
            std::move(aAtEndFn),
            std::move(aCheckFn),
            std::move(aConditionMetFn),
            std::move(aConditionNotMetFn),
            aTimeout,
            taskId
        );
    } else // taskMode == GpTaskMode::THREAD
    {
        return WaitForThread<T>
        (
            std::move(aAtBeginFn),
            std::move(aAtEndFn),
            std::move(aCheckFn),
            std::move(aConditionMetFn),
            std::move(aConditionNotMetFn),
            aTimeout
        );
    }
}

bool    GpItcSharedCondition::WaitForFiber
(
    const CheckFnT          aCheckFn,
    const milliseconds_t    aTimeout,
    const u_int_64          aFiberTaskId
)
{
    const milliseconds_t    startTs             = GpDateTimeOps::SSteadyTS_ms();
    bool                    isTaskIdRegistered  = false;

    GpRAIIonDestruct callOnDestruct
    (
        [&]()
        {
            GpUniqueLock<GpMutex> uniqueLock(iThreadsCV.Mutex());

            if (isTaskIdRegistered) [[likely]]
            {
                iFiberTaskIDs.erase(aFiberTaskId);
            }
        }
    );

    while (true)
    {
        milliseconds_t passedTime = 0.0_si_ms;

        {
            GpUniqueLock<GpMutex> uniqueLock(iThreadsCV.Mutex());

            // Check condition
            if (aCheckFn()) [[unlikely]]
            {
                return true;
            }

            // Check if fiber task id is registered into iFiberTaskIDs
            if (!isTaskIdRegistered) [[unlikely]]
            {
                iFiberTaskIDs.emplace(aFiberTaskId);
                isTaskIdRegistered = true;
            }

            // Check timeout
            passedTime = GpDateTimeOps::SSteadyTS_ms() - startTs;
            if (passedTime >= aTimeout) [[unlikely]]
            {
                return false;
            }
        }

        // Wait for
        SYeld(aTimeout - passedTime);
    }

    // Never reaches here
    return false;
}

template<typename T>
std::optional<T>    GpItcSharedCondition::WaitForFiber
(
    CheckFnT                aCheckFn,
    ConditionMetFnT<T>      aConditionMetFn,
    const milliseconds_t    aTimeout,
    const u_int_64          aFiberTaskId
)
{
    const milliseconds_t    startTs             = GpDateTimeOps::SSteadyTS_ms();
    bool                    isTaskIdRegistered  = false;

    GpRAIIonDestruct callOnDestruct
    (
        [&]()
        {
            GpUniqueLock<GpMutex> uniqueLock(iThreadsCV.Mutex());

            if (isTaskIdRegistered) [[likely]]
            {
                iFiberTaskIDs.erase(aFiberTaskId);
            }
        }
    );

    while (true)
    {
        milliseconds_t passedTime = 0.0_si_ms;

        {
            GpUniqueLock<GpMutex> uniqueLock(iThreadsCV.Mutex());

            // Check condition
            if (aCheckFn()) [[unlikely]]
            {
                return aConditionMetFn();
            }

            // Check if fiber task id is registered into iFiberTaskIDs
            if (!isTaskIdRegistered) [[unlikely]]
            {
                iFiberTaskIDs.emplace(aFiberTaskId);
                isTaskIdRegistered = true;
            }

            // Check timeout
            passedTime = GpDateTimeOps::SSteadyTS_ms() - startTs;
            if (passedTime >= aTimeout) [[unlikely]]
            {
                return std::nullopt;
            }
        }

        // Wait for
        SYeld(aTimeout - passedTime);
    }

    // Never reaches here
    return std::nullopt;
}

template<typename T>
std::optional<T>    GpItcSharedCondition::WaitForFiber
(
    AtBeginFnT              aAtBeginFn,
    AtEndFnT                aAtEndFn,
    CheckFnT                aCheckFn,
    ConditionMetFnT<T>      aConditionMetFn,
    ConditionNotMetFnT<T>   aConditionNotMetFn,
    const milliseconds_t    aTimeout,
    const u_int_64          aFiberTaskId
)
{
    const milliseconds_t    startTs             = GpDateTimeOps::SSteadyTS_ms();
    bool                    isTaskIdRegistered  = false;
    bool                    isAtBeginFnCalled   = false;

    GpRAIIonDestruct callOnDestruct
    (
        [&]()
        {
            GpUniqueLock<GpMutex> uniqueLock(iThreadsCV.Mutex());

            if (isTaskIdRegistered) [[likely]]
            {
                iFiberTaskIDs.erase(aFiberTaskId);
            }

            aAtEndFn();
        }
    );

    while (true)
    {
        milliseconds_t passedTime = 0.0_si_ms;

        {
            GpUniqueLock<GpMutex> uniqueLock(iThreadsCV.Mutex());

            // At begin check
            if (!isAtBeginFnCalled) [[unlikely]]
            {
                aAtBeginFn();
                isAtBeginFnCalled = true;
            }

            // Check condition
            if (aCheckFn()) [[unlikely]]
            {
                return aConditionMetFn();
            }

            // Check if fiber task id is registered into iFiberTaskIDs
            if (!isTaskIdRegistered) [[unlikely]]
            {
                iFiberTaskIDs.emplace(aFiberTaskId);
                isTaskIdRegistered = true;
            }

            // Check timeout
            passedTime = GpDateTimeOps::SSteadyTS_ms() - startTs;
            if (passedTime >= aTimeout) [[unlikely]]
            {
                return aConditionNotMetFn();
            }
        }

        // Wait for
        SYeld(aTimeout - passedTime);
    }

    // Never reaches here
    return std::nullopt;
}

bool    GpItcSharedCondition::WaitForThread
(
    CheckFnT                aCheckFn,
    const milliseconds_t    aTimeout
)
{
    return iThreadsCV.WaitFor<bool>
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS {iThreadsWaiting++;},// At begin
        [&]() NO_THREAD_SAFETY_ANALYSIS {iThreadsWaiting--;},// At end
        std::move(aCheckFn),// Check condition
        []() {return true;},// condition met
        []() {return false;},// condition not met
        aTimeout
    ).value();
}

template<typename T>
std::optional<T>    GpItcSharedCondition::WaitForThread
(
    CheckFnT                aCheckFn,
    ConditionMetFnT<T>      aConditionMetFn,
    const milliseconds_t    aTimeout
)
{
    return iThreadsCV.WaitFor<T>
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS {iThreadsWaiting++;},// At begin
        [&]() NO_THREAD_SAFETY_ANALYSIS {iThreadsWaiting--;},// At end
        std::move(aCheckFn),// Check condition
        std::move(aConditionMetFn),// condition met
        []() -> std::optional<T> {return std::nullopt;},// condition not met
        aTimeout
    );
}

template<typename T>
std::optional<T>    GpItcSharedCondition::WaitForThread
(
    AtBeginFnT              aAtBeginFn,
    AtEndFnT                aAtEndFn,
    CheckFnT                aCheckFn,
    ConditionMetFnT<T>      aConditionMetFn,
    ConditionNotMetFnT<T>   aConditionNotMetFn,
    const milliseconds_t    aTimeout
)
{
    return iThreadsCV.WaitFor<bool>
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS {iThreadsWaiting++; aAtBeginFn();},// At begin
        [&]() NO_THREAD_SAFETY_ANALYSIS {iThreadsWaiting--; aAtEndFn();},// At end
        std::move(aCheckFn),// Check condition
        std::move(aConditionMetFn),// condition met
        std::move(aConditionNotMetFn),// condition not met
        aTimeout
    );
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
