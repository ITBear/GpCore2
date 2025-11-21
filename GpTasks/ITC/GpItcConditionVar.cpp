#include <GpCore2/GpTasks/ITC/GpItcConditionVar.hpp>

#if defined(GP_USE_MULTITHREADING)

#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>

namespace GPlatform {

bool operator<(const GpItcConditionVar::TaskInfo& a, const GpItcConditionVar::TaskInfo& b) noexcept
{
    return a.iTaskWP < b.iTaskWP;
}

bool operator<(const GpItcConditionVar::TaskInfo& a, const GpWP<GpTask>& b) noexcept
{
    return a.iTaskWP < b;
}

bool operator<(const GpWP<GpTask>& a, const GpItcConditionVar::TaskInfo& b) noexcept
{
    return a < b.iTaskWP;
}

GpItcConditionVar::GpItcConditionVar (void) noexcept
{
}

GpItcConditionVar::GpItcConditionVar (GpItcConditionVar&& aConditionVar) noexcept:
iTasks{std::move(aConditionVar.iTasks)}
{
}

GpItcConditionVar::~GpItcConditionVar (void) noexcept
{
}

void    GpItcConditionVar::NotifyOne (void)
{
    // Notify ONE (any) fiber task
    while (!iTasks.empty())
    {
        const TaskInfo& taskInfo    = *std::begin(iTasks);
        GpTask::SP      taskSP      = taskInfo.iTaskWP.Lock();

        if (taskSP.IsNULL()) [[unlikely]]
        {
            iTasks.erase(std::begin(iTasks));
            continue;
        }

        WAKEUP_TASK(taskSP.Vn());
        break;
    }
}

void    GpItcConditionVar::NotifyAll (void)
{
    // Notify ALL fiber tasks
    for (auto iter = std::begin(iTasks); iter != std::end(iTasks); /*NOP*/)
    {
        const TaskInfo& taskInfo    = *iter;
        GpTask::SP      taskSP      = taskInfo.iTaskWP.Lock();

        if (taskSP.IsNULL()) [[unlikely]]
        {
            iter = iTasks.erase(iter);
            continue;
        }
        iter++;

        WAKEUP_TASK(taskSP.Vn());
    }
}

void    GpItcConditionVar::Subscribe (GpWP<GpTask> aTaskWP)
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    iTasks.emplace
    (
        TaskInfo
        {
            .iTaskWP        = std::move(aTaskWP),
            .iIsSubscriber  = true
        }
    );
}

bool    GpItcConditionVar::Unsubscribe (GpWP<GpTask> aTaskWP)
{
    GpUniqueLock uniqueLock{iSpinLockRW};

    return iTasks.erase(aTaskWP) > 0;
}

void    GpItcConditionVar::Wait (const CheckFnT& aCheckFn)
{
    WaitFor(aCheckFn, 0.0_si_ms);
}

void    GpItcConditionVar::Wait
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

bool    GpItcConditionVar::WaitFor
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
        {},
        {}
    );
}

bool    GpItcConditionVar::WaitFor
(
    const CheckFnT&         aCheckFn,
    const milliseconds_t    aTimeout,
    const AtBeginFnT&       aAtBeginFn,
    const AtEndFnT&         aAtEndFn
)
{
    GpTask::WP taskWP = GpTask::SCurrentTask();
    GpTask::SP taskSP = taskWP.Lock();

    VERIFY
    (
        taskSP.IsNotNULL(),
        "[GpItcConditionVar::WaitFor]: Function invoked outside a fiber context (current task is null)"
    );

    VERIFY
    (
        taskSP.Vn().TaskMode() == GpTaskMode::FIBER,
        "[GpItcConditionVar::WaitFor]: Function invoked outside a fiber context (task mode == GpTaskMode::THREAD)"
    );

    const milliseconds_t    startTs                 = GpDateTimeOps::SSteadyTS_ms();
    bool                    isTaskIdRegistered      = false;
    bool                    isAtBeginFnCalled       = false;
    const bool              isNeedToCheckTimeout    = aTimeout > 0.0_si_ms;
    bool                    result                  = false;

    TaskInfo taskInfo
    {
        .iTaskWP        = taskWP,
        .iIsSubscriber  = false,
    };

    GpDefer callOnDestruct = [&]()
    {
        GpUniqueLock uniqueLock{iSpinLockRW};

        if (isTaskIdRegistered) [[likely]]
        {
            if (taskInfo.iIsSubscriber == false)
            {
                iTasks.erase(taskWP);
            }
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
            GpUniqueLock uniqueLock{iSpinLockRW};

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
                auto iter = iTasks.find(taskWP);

                if (iter != std::end(iTasks))
                {
                    taskInfo = *iter;
                } else
                {
                    iTasks.emplace(taskInfo);
                }

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
            std::ignore = YIELD_WAIT(aTimeout - passedTime);
        } else
        {
            YIELD_WAIT();
        }
    }

    // Never reaches here
    result = false;
    return result;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
