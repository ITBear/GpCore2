#include <GpCore2/GpTasks/ITC/GpItcCondition.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpUnlockGuard.hpp>

namespace GPlatform {

void    GpItcCondition::SubscribeAsFiber (GpTaskId aGpTaskId)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iFiberSpinLockRW};

    iFiberTaskIDs.insert(aGpTaskId);
}

bool    GpItcCondition::UnsubscribeAsFiber (GpTaskId aGpTaskId)
{
    GpUniqueLock<GpSpinLockRW> uniqueLock{iFiberSpinLockRW};

    return iFiberTaskIDs.erase(aGpTaskId) > 0;
}

bool    GpItcCondition::NotifyOne (void)
{
    bool res = true;

    // Notify fiber task
    if (!iFiberTaskIDs.empty())
    {
        res &= GpTaskScheduler::S().MakeTaskReady
        (
            *std::begin(iFiberTaskIDs)
        );
    }

    // Notify thread
    if (iThreadWatingDataUP) [[unlikely]]
    {
        GpUnlockGuard<GpSpinLockRW> unlockGuard{iFiberSpinLockRW};

        if (iThreadWatingDataUP->iWaiting.load(std::memory_order_acquire) > 0)
        {
            GpUniqueLock<GpMutex> uniqueLock{iThreadWatingDataUP->iCV.Mutex()};
            iThreadWatingDataUP->iCV.NotifyOne();
        }
    }

    return res;
}

bool    GpItcCondition::NotifyAll (void)
{
    bool res = true;

    // Notify fiber tasks
    for (const GpTaskId& taskId: iFiberTaskIDs)
    {
        res &= GpTaskScheduler::S().MakeTaskReady(taskId);
    }

    // Notify threads
    if (iThreadWatingDataUP) [[unlikely]]
    {
        GpUnlockGuard<GpSpinLockRW> unlockGuard{iFiberSpinLockRW};

        if (iThreadWatingDataUP->iWaiting.load(std::memory_order_acquire) > 0)
        {
            GpUniqueLock<GpMutex> uniqueLock{iThreadWatingDataUP->iCV.Mutex()};
            iThreadWatingDataUP->iCV.NotifyAll();
        }
    }

    return res;
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
        GpUniqueLock<GpSpinLockRW> uniqueLock{iFiberSpinLockRW};

        if (isTaskIdRegistered) [[likely]]
        {
            iFiberTaskIDs.erase(aFiberTaskId);
        }

        if (aAtEndFn)
        {
            aAtEndFn(result);
        }
    };

    while (true)
    {
        milliseconds_t passedTime = 0.0_si_ms;

        {
            GpUniqueLock<GpSpinLockRW> uniqueLock{iFiberSpinLockRW};

            // At begin check
            if (!isAtBeginFnCalled) [[unlikely]]
            {
                if (aAtBeginFn)
                {
                    aAtBeginFn();
                }

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
    // Check if iThreadWatingDataUP created
    {
        GpUniqueLock<GpSpinLockRW> uniqueLock{iFiberSpinLockRW};

        if (iThreadWatingDataUP == nullptr)
        {
            iThreadWatingDataUP = std::make_unique<ThreadWatingData>();
        }
    }

    const AtBeginFnT atBeginFn = [&](void)
    {
        iThreadWatingDataUP->iWaiting.fetch_add(1, std::memory_order_acq_rel);

        if (aAtBeginFn)
        {
            GpUniqueLock<GpSpinLockRW> uniqueLock{iFiberSpinLockRW};
            aAtBeginFn();
        }
    };

    const AtEndFnT atEndFn = [&](bool aResult)
    {
        iThreadWatingDataUP->iWaiting.fetch_sub(1, std::memory_order_acq_rel);

        if (aAtEndFn)
        {
            GpUniqueLock<GpSpinLockRW> uniqueLock{iFiberSpinLockRW};
            aAtEndFn(aResult);
        }
    };

    const auto wrapCheckFn = [&]() -> bool
    {
        GpUniqueLock<GpSpinLockRW> uniqueLock{iFiberSpinLockRW};
        return aCheckFn();
    };

    return iThreadWatingDataUP->iCV.WaitFor
    (
        wrapCheckFn,
        aTimeout,
        atBeginFn,
        atEndFn
    );
}

GpItcCondition::TaskInfo    GpItcCondition::SCurrentTaskInfo (void)
{
    // Collect current task info
    std::optional<std::reference_wrapper<GpTask>>   taskOptRef  = GpTask::SCurrentTask();
    GpTaskMode::EnumT                               taskMode    = GpTaskMode::THREAD;
    GpTaskId                                        taskId      = {};

    if (taskOptRef.has_value()) [[likely]]
    {
        const GpTask& task = taskOptRef.value().get();

        taskMode    = task.TaskMode();
        taskId      = task.TaskId();
    }

    return {taskMode, taskId};
}

void    GpItcCondition::SYield (const milliseconds_t aTimeout)
{
    std::ignore = YIELD_WAIT(aTimeout);
}

void    GpItcCondition::SYield (void)
{
    YIELD_WAIT();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
