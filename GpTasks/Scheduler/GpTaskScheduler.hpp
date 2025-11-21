#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpAny.hpp>
#include <GpCore2/GpTasks/GpTasks_global.hpp>
#include <GpCore2/GpTasks/GpTaskEnums.hpp>
#include <GpCore2/GpTasks/GpTask.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpOutUtils.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpTaskSchedulerFactory;
class GpTaskExecutor;

class GP_TASKS_API GpTaskScheduler
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskScheduler)
    CLASS_DD(GpTaskScheduler)
    TAG_SET(THREAD_SAFE)

protected:
                            GpTaskScheduler     (size_t aExecutorsCount,
                                                 size_t aTasksMaxCount) noexcept;

public:
    virtual                 ~GpTaskScheduler    (void) noexcept = default;

    static GpTaskScheduler& S                   (void) noexcept {return *sInstance;}
    static void             SStart              (const GpTaskSchedulerFactory&  aFactory,
                                                 size_t                         aExecutorsCount,
                                                 size_t                         aTasksMaxCount);
    static void             SStop               (void);

    size_t                  ExecutorsCount      (void) const noexcept {return iExecutorsCount;}
    size_t                  TasksMaxCount       (void) const noexcept {return iTasksMaxCount;}

    [[nodiscard]] GpTask::DoneFutureT::SP
                            RequestStop         (GpTask& aTask);

    // Task wait/ready
    virtual void            SpawnReady          (GpSP<GpTask> aTaskSP) = 0;
    virtual void            SpawnWaiting        (GpSP<GpTask> aTaskSP) = 0;
    virtual void            Wakeup              (GpTask& aTask) = 0;

    // Scheduler
    virtual bool            Reschedule          (GpTaskRunRes::EnumT    aRunRes,
                                                 GpSP<GpTask>           aTaskSP,
                                                 GpMethodAccessGuard<GpTaskExecutor>) = 0;

protected:
    GpSpinLockRW<>&         SpinLockRW          (void) const noexcept RETURN_CAPABILITY(iSpinLockRW) {return iSpinLockRW;}

    virtual void            Start               (void) = 0;
    virtual void            RequestStopAndJoin  (void) = 0;

private:
    mutable GpSpinLockRW<>      iSpinLockRW;

    const size_t                iExecutorsCount = 0;
    const size_t                iTasksMaxCount  = 0;

    static GpTaskScheduler::UP  sInstance;
    static std::atomic_flag     sRequestStopAndJoinCall;
};

inline void                                     SPAWN_READY_TASK    (GpSP<GpTask> aTaskSP)  {GpTaskScheduler::S().SpawnReady(std::move(aTaskSP));}
inline void                                     SPAWN_WAITING_TASK  (GpSP<GpTask> aTaskSP)  {GpTaskScheduler::S().SpawnWaiting(std::move(aTaskSP));}
inline void                                     WAKEUP_TASK         (GpTask& aTask) {GpTaskScheduler::S().Wakeup(aTask);}
[[nodiscard]] inline GpTask::DoneFutureT::SP    STOP_TASK           (GpTask& aTask) {return GpTaskScheduler::S().RequestStop(aTask);}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
