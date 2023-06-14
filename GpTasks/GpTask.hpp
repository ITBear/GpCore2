#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskState.hpp"
#include "ITC/GpItcPromise.hpp"
#include "../GpUtils/EventBus/GpEventSubscriber.hpp"
#include "../GpUtils/Types/Containers/GpFlatMap.hpp"
#include "../GpUtils/Types/Containers/GpDictionary.hpp"
#include "../GpUtils/Types/Containers/GpAny.hpp"
#include "../GpUtils/Threads/GpThreadStopToken.hpp"

namespace GPlatform {

class GpTaskAccessor;
class GpTaskScheduler;

enum class GpTaskDoRes
{
    READY_TO_EXEC,
    WAITING,
    DONE
};

enum class GpTaskType
{
    THREAD,
    FIBER
};

class GP_TASKS_API GpTask: public GpEventSubscriber
{
    friend class GpTaskAccessor;

public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTask)
    CLASS_DD(GpTask)

    using StateT                = GpTaskState;
    using StateTE               = StateT::EnumT;
    using SchedulerRefT         = std::optional<std::reference_wrapper<GpTaskScheduler>>;
    using TasksByThreadIdT      = GpFlatMap<std::thread::id, GpTask*, GpTasksSettings::SMaxCoresCount()>;
    using CompleteItcPromiseT   = GpItcPromise<size_t>;
    using CompleteItcFutureT    = GpItcFuture<size_t>;
    using CompleteItcResultT    = GpItcResult<size_t>;
    using VarsT                 = GpDictionary<std::u8string, GpAny>;

protected:
    inline                          GpTask                  (std::u8string      aName,
                                                             const GpTaskType   aTaskType) noexcept;
public:
    virtual                         ~GpTask                 (void) noexcept override;

    std::u8string_view              Name                    (void) const noexcept {return iName;}
    GpUUID                          Guid                    (void) const noexcept {return iGuid;}
    GpTaskType                      TaskType                (void) const noexcept {return iTaskType;}
    bool                            IsStopRequested         (void) const noexcept {return iIsStopRequested.load(std::memory_order_acquire);}
    const GpTask*                   ParentTask              (void) const noexcept {return iParentTask;}
    GpTask*                         ParentTask              (void) noexcept {return iParentTask;}
    size_t                          ChildTasksCount         (void) const noexcept {return iChildTasksCount.load(std::memory_order_acquire);}
    size_t                          ChildTasksInc           (void) noexcept {return iChildTasksCount.fetch_add(1, std::memory_order_release)+1;}
    size_t                          ChildTasksDec           (void) noexcept {return iChildTasksCount.fetch_sub(1, std::memory_order_release)-1;}

    inline CompleteItcFutureT::SP   Future                  (void);

    inline static GpTask*           SCurrent                (void) noexcept;
    inline static GpUUID            SCurrentUID             (void) noexcept;
    inline static void              SSetCurrent             (GpTask* aTask);
    inline static void              SClearCurrent           (void);

    bool                            RunRequestFlag          (void) const noexcept {return iRunRequestFlag.load(std::memory_order_acquire);}
    void                            UpRunRequestFlag        (void) noexcept {iRunRequestFlag.store(true, std::memory_order_release);}
    void                            DownRunRequestFlag      (void) noexcept {iRunRequestFlag.store(false, std::memory_order_release);}

    const VarsT&                    Vars                    (void) const noexcept {return iVars;}
    VarsT&                          Vars                    (void) noexcept {return iVars;}

protected:
    virtual void                    OnPushEvent             (void) override final;
    inline void                     CompletePromise         (CompleteItcResultT::SP aResult) noexcept;

    GpTaskDoRes                     Run                     (GpThreadStopToken aStopToken) noexcept;
    virtual GpTaskDoRes             _Run                    (GpThreadStopToken aStopToken) noexcept = 0;

    void                            RequestStop             (void) noexcept {iIsStopRequested.store(true, std::memory_order_release);}

private:
    StateTE                         State                   (void) const noexcept {return iState;}
    void                            UpdateState             (StateTE aNewState) noexcept {iState = aNewState;}
    void                            SetParentTask           (GpTask* aTask) noexcept {iParentTask = aTask;}

    static void                     SAddExecutorThreadId    (const std::thread::id& aThreadId);

private:
    const std::u8string             iName;
    const GpUUID                    iGuid;
    const GpTaskType                iTaskType;
    StateTE                         iState              = StateTE::NOT_ASSIGNED_TO_SCHEDULER;
    CompleteItcPromiseT             iCompletePromise;
    std::atomic_bool                iIsStopRequested    = false;
    GpTask*                         iParentTask         = nullptr;
    std::atomic_size_t              iChildTasksCount    = 0;
    std::atomic_bool                iRunRequestFlag     = false;
    VarsT                           iVars;

    static TasksByThreadIdT         sTasksByThreadId;
};

GpTask::GpTask
(
    std::u8string       aName,
    const GpTaskType    aTaskType
) noexcept:
iName    (std::move(aName)),
iGuid    (GpUUID::SGenRandomV4()),
iTaskType(aTaskType)
{
}

GpTask::CompleteItcFutureT::SP  GpTask::Future (void)
{
    if (iParentTask != nullptr)
    {
        return iCompletePromise.Future(iParentTask->Guid());
    } else
    {
        return iCompletePromise.Future(GpUUID());
    }
}

GpTask* GpTask::SCurrent (void) noexcept
{
    const std::thread::id threadId = std::this_thread::get_id();
    return sTasksByThreadId.FindCopyOrDefault(threadId);
}

GpUUID  GpTask::SCurrentUID (void) noexcept
{
    const GpTask* task = GpTask::SCurrent();

    if (task != nullptr)
    {
        return task->Guid();
    } else
    {
        return GpUUID();
    }
}

void    GpTask::SSetCurrent (GpTask* aTask)
{
    const std::thread::id threadId = std::this_thread::get_id();
    sTasksByThreadId.Update(threadId, aTask);
}

void    GpTask::SClearCurrent (void)
{
    const std::thread::id threadId = std::this_thread::get_id();

    GpTask* task = nullptr;
    sTasksByThreadId.Update(threadId, task);
}

void    GpTask::CompletePromise (CompleteItcResultT::SP aResult) noexcept
{
    iCompletePromise.Complete(std::move(aResult));
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
