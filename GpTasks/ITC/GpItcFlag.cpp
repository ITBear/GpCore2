#include "GpItcFlag.hpp"
#include "GpTask.hpp"
#include "GpTaskScheduler.hpp"
#include "Fibers/GpTaskFiber.hpp"

#include <iostream>

namespace GPlatform {

void    _Call
(
    GpItcResult::SP&                        aValue,
    std::function<void(GpItcResult::SP&&)>& aOnSetFn
)
{
    aOnSetFn(std::move(aValue));
}

template<typename... ArgsT>
void    _ProcessSet
(
    std::vector<GpUUID>&    aTasks,
    GpConditionVar::SP&     aConditionVar,
    ArgsT&&...              aArgs
)
{
    //Process args
    if constexpr (sizeof...(aArgs) > 0)
    {
        _Call(aArgs...);
    }

    //Wakeup waiting tasks
    {
        for (const GpUUID& taskGuid: aTasks)
        {
            try
            {
                GpTaskScheduler::S().MakeTaskReady(taskGuid);
            } catch (const GpException& e)
            {
                std::cerr << "[_ProcessSet]: " << e.what() << std::endl;
            } catch (const std::exception& e)
            {
                std::cerr << "[_ProcessSet]: " << e.what() << std::endl;
            } catch (...)
            {
                std::cerr << "[_ProcessSet]: unknown" << std::endl;
            }
        }

        aTasks.clear();
    }

    //Wakeup by CV
    if (aConditionVar.IsNotNULL())
    {
        aConditionVar.V().WakeupAll();
    }
}

void    GpItcFlag::ProcessSet (void) noexcept
{
    _ProcessSet
    (
        iTasks,
        iConditionVar
    );
}

void    GpItcFlag::ProcessSet
(
    GpItcResult::SP                         aValue,
    std::function<void(GpItcResult::SP&&)>  aOnSetFn
) noexcept
{
    _ProcessSet<GpItcResult::SP&, std::function<void(GpItcResult::SP&&)>&>
    (
        iTasks,
        iConditionVar,
        aValue,
        aOnSetFn
    );
}

bool    GpItcFlag::TryWait (const milliseconds_t aWaitTimeout)
{
    GpTaskType currentTaskType = GpTaskType::THREAD;

    //Test and subscribe
    {
        std::scoped_lock lock(iLock);

        if (iFlag == true)
        {
            return true;
        }

        //Get current task
        GpTask* currentTask = GpTask::SCurrent();

        if (currentTask != nullptr)
        {
            const GpUUID currentTaskGuid = currentTask->Guid();
            currentTaskType = currentTask->TaskType();

            //Subscribe
            if (currentTaskType == GpTaskType::FIBER)
            {
                iTasks.emplace_back(currentTaskGuid);
            }
        }

        if (currentTaskType == GpTaskType::THREAD)
        {
            if (iConditionVar.IsNULL())
            {
                iConditionVar = MakeSP<GpConditionVar>();
            }
        }

        if (aWaitTimeout == 0.0_si_s)
        {
            return false;
        }
    }

    //Wait for result
    if (aWaitTimeout < 0.0_si_s)//without timeout
    {
        while (!Test())
        {
            if (currentTaskType == GpTaskType::THREAD)
            {
                if (iConditionVar.Vn().WaitForWakeup(aWaitTimeout) == false)
                {
                    THROW_GP("Condition variable WaitForWakeup return error"_sv);
                }
            } else//currentTaskType == GpTaskType::FIBER
            {
                YELD_WAITING();
            }
        }
    } else//with timeout
    {
        milliseconds_t startTs = GpDateTimeOps::SSteadyTS_ms();

        while (!Test())
        {
            if ((GpDateTimeOps::SSteadyTS_ms() - startTs) >= aWaitTimeout)
            {
                return false;
            }

            if (currentTaskType == GpTaskType::THREAD)
            {
                if (iConditionVar.Vn().WaitForWakeup(aWaitTimeout) == false)
                {
                    THROW_GP("Condition variable WaitForWakeup return error"_sv);
                }
            } else//currentTaskType == GpTaskType::FIBER
            {
                //TODO: reimplement with timer
                YELD_READY();
            }
        }
    }

    return true;
}

}//namespace GPlatform
