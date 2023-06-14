#include "GpItcSharedCondition.hpp"
#include "GpTaskScheduler.hpp"
#include "Fibers/GpTaskFiber.hpp"
#include "../../GpUtils/DateTime/GpDateTimeOps.hpp"
#include "../../GpUtils/Other/GpRAIIonDestruct.hpp"

#include <iostream>

namespace GPlatform {

void    GpItcSharedCondition::WakeupOne (std::function<void()> aFn)
{
    iCV.WakeupOne
    (
        [&]()
        {
            aFn();

            //Wakeup waiting task fibers
            for (const GpUUID& taskGuid: iTaskFiberGuids)
            {
                try
                {
                    if (taskGuid.IsNotZero())
                    {
                        GpTaskScheduler::S().MakeTaskReady(taskGuid);
                        break;
                    }
                } catch (const GpException& e)
                {
                    GpStringUtils::SCerr("[GpItcSharedCondition::WakeupOne]: "_sv + e.what() + "\n"_sv);
                } catch (const std::exception& e)
                {
                    GpStringUtils::SCerr("[GpItcSharedCondition::WakeupOne]: "_sv + e.what() + "\n"_sv);
                } catch (const boost::context::detail::forced_unwind&)
                {
                    throw;
                } catch (...)
                {
                    GpStringUtils::SCerr("[GpItcSharedCondition::WakeupOne]: unknown\n"_sv);
                }
            }
        }
    );
}

void    GpItcSharedCondition::WakeupAll (std::function<void()> aFn)
{
    iCV.WakeupAll
    (
        [&]()
        {
            aFn();

            //Wakeup waiting task fibers
            for (const GpUUID& taskGuid: iTaskFiberGuids)
            {
                try
                {
                    if (taskGuid.IsNotZero())
                    {
                        GpTaskScheduler::S().MakeTaskReady(taskGuid);
                    }
                } catch (const GpException& e)
                {
                    GpStringUtils::SCerr("[GpItcSharedCondition::WakeupAll]: "_sv + e.what() + "\n"_sv);
                } catch (const std::exception& e)
                {
                    GpStringUtils::SCerr("[GpItcSharedCondition::WakeupAll]: "_sv + e.what() + "\n"_sv);
                } catch (const boost::context::detail::forced_unwind&)
                {
                    throw;
                } catch (...)
                {
                    GpStringUtils::SCerr("[GpItcSharedCondition::WakeupAll]: unknown\n"_sv);
                }
            }
        }
    );
}

GpItcSharedCondition::WaitForConditionResT  GpItcSharedCondition::_WaitForCondition
(
    const milliseconds_t    aTimeout,
    ConditionFnT            aConditionFn,
    ActionCondMetFnT        aOnConditionMetFn,
    ActionCondNotMetFnT     aOnConditionNotMetFn
)
{
    //Collect current task info
    GpTask*             currentTask     = GpTask::SCurrent();
    const GpTaskType    currentTaskType = (currentTask != nullptr) ? currentTask->TaskType() : GpTaskType::THREAD;
    const GpUUID        currentTaskGuid = (currentTask != nullptr) ? currentTask->Guid() : GpUUID();

    //On destruct
    GpRAIIonDestruct onDestruct
    (
        [&]()
        {
            if (currentTaskType == GpTaskType::FIBER)
            {
                iCV.Do
                (
                    [&]()
                    {
                        _RemoveTaskGuid(currentTaskGuid);
                    }
                );
            }
        }
    );

    //Wait for
    WaitForConditionResT waitForConditionRes = WaitForConditionResT::TIMEOUT;

    if (currentTaskType == GpTaskType::FIBER)
    {
        //Subscribe
        iCV.Do
        (
            [&]()
            {
                _AddTaskGuid(currentTaskGuid);
            }
        );

        const milliseconds_t startTs = GpDateTimeOps::SSteadyTS_ms();

        while (true)
        {
            //Check condition
            iCV.Do
            (
                [&]()
                {
                    const bool isConditionMet = aConditionFn();

                    if (isConditionMet)
                    {
                        aOnConditionMetFn();
                        waitForConditionRes = WaitForConditionResT::OK;
                    } else
                    {
                        if (aOnConditionNotMetFn() == ActionCondNotMetRes::BREAK)
                        {
                            waitForConditionRes = WaitForConditionResT::OK;
                        }
                    }
                }
            );

            //Check
            if (waitForConditionRes == WaitForConditionResT::OK)
            {
                break;
            }

            //Check timeout
            if ((GpDateTimeOps::SSteadyTS_ms() - startTs) >= aTimeout)
            {
                waitForConditionRes = WaitForConditionResT::TIMEOUT;
                break;
            }

            //Wait
            YELD_WAITING();
        }
    } else //currentTaskType == GpTaskType::THREAD
    {
        waitForConditionRes = iCV.WaitFor
        (
            [&]()
            {
                const bool isConditionMet = aConditionFn();

                if (isConditionMet)
                {
                    aOnConditionMetFn();
                    return true;
                } else
                {
                    if (aOnConditionNotMetFn() == ActionCondNotMetRes::BREAK)
                    {
                        return true;
                    } else
                    {
                        return false;
                    }
                }
            },
            aTimeout
        );
    }

    return waitForConditionRes;
}

void    GpItcSharedCondition::_AddTaskGuid (const GpUUID& aTaskGuid)
{
    if (aTaskGuid.IsZero())
    {
        return;
    }

    const size_t size = iTaskFiberGuids.size();

    ssize_t freeSlotId = -1;

    for (size_t id = 0; id < size; id++)
    {
        GpUUID& slotUid = iTaskFiberGuids[id];

        if (slotUid == aTaskGuid)
        {
            return;
        } else if (slotUid.IsZero() && (freeSlotId == -1))
        {
            freeSlotId = ssize_t(id);
        }
    }

    if (freeSlotId >= 0)
    {
        iTaskFiberGuids[size_t(freeSlotId)] = aTaskGuid;
    } else
    {
        iTaskFiberGuids.emplace_back(aTaskGuid);
    }
}

void    GpItcSharedCondition::_RemoveTaskGuid (const GpUUID& aTaskGuid)
{
    const size_t size = iTaskFiberGuids.size();

    for (size_t id = 0; id < size; id++)
    {
        if (iTaskFiberGuids[id] == aTaskGuid)
        {
            iTaskFiberGuids[id].Zero();
            return;
        }
    }
}

}//namespace GPlatform
