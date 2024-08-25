#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include <GpCore2/GpTasks/GpTaskEnums.hpp>
#include <GpCore2/GpTasks/GpTaskVarStorage.hpp>
#include <GpCore2/GpTasks/ITC/GpItcSharedPromise.hpp>

#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpSharedQueueSimple.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpDictionary.hpp>
#include <GpCore2/GpUtils/Types/UIDs/GpUUID.hpp>
#include <GpCore2/GpUtils/Other/GpMethodAccessGuard.hpp>

namespace GPlatform {

class GpTaskScheduler;
class GpTaskExecutor;

class GP_TASKS_API GpTask
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTask)
    CLASS_DD(GpTask)
    TAG_SET(THREAD_SAFE)

    class StartPromiseShellType;
    using StartPromiseRes       = GpTypeShell<GpAny, StartPromiseShellType>;
    using StartPromiseT         = GpItcSharedPromise<StartPromiseRes>;
    using StartFutureT          = typename StartPromiseT::FutureT;

    class DonePromiseShellType;
    using DonePromiseRes        = GpTypeShell<GpAny, DonePromiseShellType>;
    using DonePromiseT          = GpItcSharedPromise<DonePromiseRes>;
    using DoneFutureT           = typename DonePromiseT::FutureT;

    using MessageQueueT         = GpSharedQueueSimple<GpAny>;
    using AllTasksDictionaryT   = GpDictionary<std::unordered_map<GpTaskId, GpTask*>>;

protected:
                                    GpTask                  (std::string        aName,
                                                             GpTaskMode::EnumT  aTaskMode) noexcept;
                                    GpTask                  (GpTaskMode::EnumT  aTaskMode) noexcept;
                                    GpTask                  (std::string        aName,
                                                             GpTaskMode::EnumT  aTaskMode,
                                                             GpTaskId           aId) noexcept;
                                    GpTask                  (GpTaskMode::EnumT  aTaskMode,
                                                             GpTaskId           aId) noexcept;

public:
    virtual                         ~GpTask                 (void) noexcept;

    static GpTask::C::Opt::Ref      SCurrentTask            (void) noexcept;
    static std::optional<GpTask*>   STaskById               (GpTaskId aTaskId) noexcept;

    inline std::string_view         TaskName                (void) const noexcept;
    inline GpTaskId                 TaskId                  (void) const noexcept;
    GpUUID                          TaskIdAsUUID            (void) const noexcept;
    inline GpTaskMode::EnumT        TaskMode                (void) const noexcept;

    GpTask::DoneFutureT::SP         RequestTaskStop         (void);
    void                            RequestAndWaitForStop   (void);

    // Messages queue
    void                            PushMessage             (GpAny aMessage);

protected:
    GpAny::C::Opt::Val              PopMessage              (void);

public:
    // Vars
    void                            SetVar                  (std::string    aKey,
                                                             GpAny          aValue);
    GpTaskVarStorage::AnyOptT       GetVarCopy              (std::string_view   aKey) const;
    GpTaskVarStorage::AnyOptCRefT   GetVarRef               (std::string_view   aKey) const;

    // Use only from Task Executor or Task Scheduler
    GpTaskRunRes::EnumT             Execute                 (GpMethodAccessGuard<GpTaskScheduler, GpTaskExecutor>) noexcept;
    inline void                     UpStartRequestFlag      (GpMethodAccessGuard<GpTaskScheduler>) noexcept;
    inline bool                     IsStartRequested        (void) const noexcept;
    inline void                     UpStopRequestFlag       (GpMethodAccessGuard<GpTaskScheduler>) noexcept;
    inline bool                     IsStopRequested         (void) const noexcept;
    inline std::atomic_flag&        IsStopRequestedRef      (void) noexcept;

    // Task Start/Done promise
    inline StartFutureT::SP         GetStartFuture          (void);
    inline DoneFutureT::SP          GetDoneFuture           (void);

protected:
    virtual GpTaskRunRes::EnumT     Run                     (void) noexcept = 0;

    inline StartPromiseT&           StartPromise            (void) noexcept;
    inline DonePromiseT&            DonePromise             (void) noexcept;

private:
    inline static GpTaskId          SNextId                 (void) noexcept;

private:
    const std::string               iName;
    const GpTaskId                  iId;
    const GpTaskMode::EnumT         iMode;  
    std::atomic_flag                iIsStartRequested   = false;
    std::atomic_flag                iIsStopRequested    = false;
    StartPromiseT                   iStartPromise;
    DonePromiseT                    iDonePromise;
    MessageQueueT                   iMessagesQueue; 

    static std::atomic<GpTaskId::value_type>    sIdCounter;
    static AllTasksDictionaryT                  sAllTasksDictionary;
};

std::string_view    GpTask::TaskName (void) const noexcept
{
    return iName;
}

GpTaskId    GpTask::TaskId (void) const noexcept
{
    return iId;
}

GpTaskMode::EnumT   GpTask::TaskMode (void) const noexcept
{
    return iMode;
}

bool    GpTask::IsStartRequested (void) const noexcept
{
    return iIsStartRequested.test(std::memory_order_acquire);
}

void    GpTask::UpStartRequestFlag (GpMethodAccessGuard<GpTaskScheduler>) noexcept
{
    iIsStartRequested.test_and_set(std::memory_order_release);
}

bool    GpTask::IsStopRequested (void) const noexcept
{
    return iIsStopRequested.test(std::memory_order_acquire);
}

std::atomic_flag&   GpTask::IsStopRequestedRef (void) noexcept
{
    return iIsStopRequested;
}

void    GpTask::UpStopRequestFlag (GpMethodAccessGuard<GpTaskScheduler>) noexcept
{
    iIsStopRequested.test_and_set(std::memory_order_release);
}

GpTask::StartFutureT::SP    GpTask::GetStartFuture (void)
{
    return iStartPromise.Future();
}

GpTask::DoneFutureT::SP GpTask::GetDoneFuture (void)
{
    return iDonePromise.Future();
}

GpTask::StartPromiseT&  GpTask::StartPromise (void) noexcept
{
    return iStartPromise;
}

GpTask::DonePromiseT&   GpTask::DonePromise (void) noexcept
{
    return iDonePromise;
}

GpTaskId    GpTask::SNextId (void) noexcept
{
    return sIdCounter.fetch_add(1, std::memory_order_relaxed);
}

}// namespace GPlatform
