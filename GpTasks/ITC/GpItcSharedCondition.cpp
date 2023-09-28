#include "GpItcSharedCondition.hpp"
#include "../../GpUtils/Types/Strings/GpStringUtils.hpp"
#include "../../GpUtils/Types/Strings/GpStringOps.hpp"
#include "../../GpUtils/Other/GpRAIIonDestruct.hpp"
#include "../../GpUtils/DateTime/GpDateTimeOps.hpp"
#include "../../GpUtils/SyncPrimitives/GpUnlockGuard.hpp"
#include "../Scheduler/GpTaskScheduler.hpp"
#include "../GpTask.hpp"
#include "../Fibers/GpTaskFiber.hpp"

GP_WARNING_PUSH()
GP_WARNING_DISABLE(shadow)

#include <boost/context/fiber.hpp>

GP_WARNING_POP()

namespace GPlatform {

void    GpItcSharedCondition::Notify
(
    const NotifyMode    aNotifyMode,
    AtomicFnOptT        aBeforeSendNotifyFn
)
{
    iThreadsFlagCV.DoAtomic
    (
        [&](std::mutex& aLock)
        {
            // Call 'aBeforeSendNotifyFn'
            if (aBeforeSendNotifyFn.has_value())
            {
                aBeforeSendNotifyFn.value().get()(aLock);
            }

            // Notify fiber tasks
            for (const GpTask::IdT taskId: iFiberTaskIds)
            {
                try
                {
                    GpTaskScheduler::S().MakeTaskReady(taskId);

                    if (aNotifyMode == NotifyMode::ONE)
                    {
                        break;
                    }
                } catch (const GpException& e)
                {
                    GpStringUtils::SCerr(u8"[GpItcSharedCondition::Notify]: "_sv + GpUTF::S_STR_To_UTF8(e.what()));
                } catch (const std::exception& e)
                {
                    GpStringUtils::SCerr(u8"[GpItcSharedCondition::Notify]: "_sv + GpUTF::S_STR_To_UTF8(e.what()));
                } catch (const boost::context::detail::forced_unwind&)
                {
                    throw;
                } catch (...)
                {
                    GpStringUtils::SCerr(u8"[GpItcSharedCondition::Notify]: unknown"_sv);
                }
            }

            // Notify threads
            if (iThreadsWaiting.load(std::memory_order_acquire) > 0)
            {
                GpUnlockGuard unlock(aLock);

                if (aNotifyMode == NotifyMode::ALL)
                {
                    iThreadsFlagCV.NotifyAll();
                } else// if (aNotifyMode == NotifyMode::ONE)
                {
                    iThreadsFlagCV.NotifyOne();
                }
            }
        }
    );
}

bool    GpItcSharedCondition::WaitFor
(
    const milliseconds_t    aTimeout,
    const CondFnT&          aCondFn
)
{
    // Collect current task info
    GpTaskScheduler::TaskOptRefT    taskOptRef  = GpTask::SCurrentTask();
    GpTaskMode::EnumT               taskMode    = GpTaskMode::THREAD;
    GpTask::IdT                     taskId      = {};

    if (taskOptRef.has_value())
    {
        const GpTask& task = taskOptRef.value().get();

        taskMode    = task.Mode();
        taskId      = task.Id();
    }

    if (taskMode == GpTaskMode::FIBER)
    {
        return WaitForFiber
        (
            taskId,
            aTimeout,
            aCondFn
        );
    } else if (taskMode == GpTaskMode::THREAD)
    {
        return WaitForThread
        (
            aTimeout,
            aCondFn
        );
    }

    return false;
}

bool    GpItcSharedCondition::WaitForFiber
(
    const GpTask::IdT       aTaskId,
    const milliseconds_t    aTimeout,
    const CondFnT&          aCondFn
)
{
    bool isFiberTaskAdded = false;

    //Remove aTaskGuid from iFiberTaskGuids
    GpRAIIonDestruct onDestruct
    (
        [&]()
        {
            if (isFiberTaskAdded)
            {
                iThreadsFlagCV.DoAtomic
                (
                    [&](std::mutex&)
                    {
                        iFiberTaskIds.erase(aTaskId);
                    }
                );
            }
        }
    );

    const milliseconds_t startTs = GpDateTimeOps::SSteadyTS_ms();
    bool isContinue     = true;
    bool isConditionMet = true;

    while (isContinue)
    {
        iThreadsFlagCV.DoAtomic
        (
            [&](std::mutex& aLock)
            {
                if (aCondFn(aLock))
                {
                    isConditionMet  = true;
                    isContinue      = false;
                    return;
                }

                // Check for timeout
                const milliseconds_t nowTs      = GpDateTimeOps::SSteadyTS_ms();
                const milliseconds_t passedTime = nowTs - startTs;
                if (aTimeout <= passedTime)
                {
                    isConditionMet  = false;
                    isContinue      = false;
                    return;
                }

                // Add fiber taskId to waiting list
                if (!isFiberTaskAdded)
                {
                    iFiberTaskIds.emplace(aTaskId);
                    isFiberTaskAdded = true;
                }

                // Wait for
                {
                    GpUnlockGuard unlock(aLock);
                    YELD_WAIT(aTimeout - passedTime);
                }
            }
        );
    }

    return isConditionMet;
}

bool    GpItcSharedCondition::WaitForThread
(
    const milliseconds_t    aTimeout,
    const CondFnT&          aCondFn
)
{
    GpRAIIonDestruct onDestruct
    (
        [&]()
        {
            iThreadsWaiting.fetch_sub(1, std::memory_order_acq_rel);
        }
    );

    iThreadsWaiting.fetch_add(1, std::memory_order_acq_rel);

    const milliseconds_t startTs = GpDateTimeOps::SSteadyTS_ms();
    bool isContinue     = true;
    bool isConditionMet = true;

    while (isContinue)
    {
        iThreadsFlagCV.DoAtomic
        (
            [&](std::mutex& aLock)
            {
                if (aCondFn(aLock))
                {
                    isConditionMet  = true;
                    isContinue      = false;
                    return;
                }

                //Check for timeout
                const milliseconds_t nowTs      = GpDateTimeOps::SSteadyTS_ms();
                const milliseconds_t passedTime = nowTs - startTs;
                if (aTimeout <= passedTime)
                {
                    isConditionMet  = false;
                    isContinue      = false;
                    return;
                }

                //Wait for
                {
                    GpUnlockGuard unlock(aLock);
                    iThreadsFlagCV.WaitFor(aTimeout - passedTime);
                }
            }
        );
    }

    return isConditionMet;
}

}//namespace GPlatform
