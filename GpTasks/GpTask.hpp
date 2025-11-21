#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include <GpCore2/GpTasks/GpTaskEnums.hpp>
#include <GpCore2/GpTasks/ITC/GpItcPromise.hpp>

#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/Other/GpLinkedLibsInfo.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpSharedQueueMPMC.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpSharedMap.hpp>
#include <GpCore2/GpUtils/Types/UIDs/GpUUID.hpp>
#include <GpCore2/GpUtils/Other/GpMethodAccessGuard.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpAny.hpp>

#include <GpCore2/Config/IncludeExt/unordered_dense.hpp>

#if defined(GP_USE_MULTITHREADING)

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
    using StartPromiseT         = GpItcPromise<StartPromiseRes>;
    using StartFutureT          = typename StartPromiseT::FutureT;

    class DonePromiseShellType;
    using DonePromiseRes        = GpTypeShell<GpAny, DonePromiseShellType>;
    using DonePromiseT          = GpItcPromise<DonePromiseRes>;
    using DoneFutureT           = typename DonePromiseT::FutureT;

    using IdCounterT            = std::atomic<GpTaskId::value_type>;

protected:
                                            GpTask              (std::string        aName,
                                                                 GpTaskMode::EnumT  aTaskMode) noexcept;
                                            GpTask              (GpTaskMode::EnumT  aTaskMode) noexcept;

public:
    virtual                                 ~GpTask             (void) noexcept;

    void                                    SetSelfWP           (GpTask::WP aSelfWP,
                                                                 GpMethodAccessGuard<GpTaskScheduler>) noexcept {iSelfWP = std::move(aSelfWP);}

    static GpTask::WP                       SCurrentTask        (void) noexcept;

    inline std::string_view                 TaskName            (void) const noexcept;
    inline GpTaskId                         TaskId              (void) const noexcept;
    GpUUID                                  TaskIdAsUUID        (void) const noexcept;
    inline GpTaskMode::EnumT                TaskMode            (void) const noexcept;

    [[nodiscard]] DoneFutureT::SP           RequestStop         (void);
    void                                    RequestStopAndWait  (void);

    static GpTaskRunRes::EnumT              SExecute            (GpTask::SP aTaskSP,
                                                                 GpMethodAccessGuard<GpTaskScheduler, GpTaskExecutor>) noexcept;

    inline bool                             IsStopRequested     (void) const noexcept;
    inline std::atomic_flag&                IsStopRequestedRef  (void) noexcept;
    inline void                             UpStopRequestFlag   (GpMethodAccessGuard<GpTaskScheduler>) noexcept;

    inline void                             UpDefferedWakeupFlag(GpMethodAccessGuard<GpTaskScheduler>) noexcept;
    inline bool                             IsDefferedWakeup    (GpMethodAccessGuard<GpTaskScheduler>) const noexcept;

    // Task Start/Done future/promise
    [[nodiscard]] inline StartFutureT::SP   StartFuture         (void);
    [[nodiscard]] inline DoneFutureT::SP    DoneFuture          (void);
    [[nodiscard]] inline StartPromiseT&     StartPromise        (GpMethodAccessGuard<GpTask>) noexcept;
    [[nodiscard]] inline DonePromiseT&      DonePromise         (GpMethodAccessGuard<GpTask>) noexcept;

    virtual u_int_64                        TypeUID             (void) const noexcept;

protected:
    virtual GpTaskRunRes::EnumT             Run                 (void) noexcept = 0;

    const GpTask::WP&                       SelfWP              (void) const noexcept {return iSelfWP;}
    GpTask::WP&                             SelfWP              (void) noexcept {return iSelfWP;}

private:
    inline static GpTaskId                  SNextId             (void) noexcept;

private:
    const std::string       iName;
    const GpTaskId          iId;
    const GpTaskMode::EnumT iMode;
    std::atomic_flag        iStopRequestFlag    = ATOMIC_FLAG_INIT;
    std::atomic_flag        iDefferedWakeupFlag = ATOMIC_FLAG_INIT;
    StartPromiseT           iStartPromise;
    DonePromiseT            iDonePromise;
    GpTask::WP              iSelfWP;

    static IdCounterT       sIdCounter;
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

bool    GpTask::IsStopRequested (void) const noexcept
{
    return iStopRequestFlag.test(std::memory_order_acquire);
}

std::atomic_flag&   GpTask::IsStopRequestedRef (void) noexcept
{
    return iStopRequestFlag;
}

void    GpTask::UpStopRequestFlag (GpMethodAccessGuard<GpTaskScheduler>) noexcept
{
    iStopRequestFlag.test_and_set(std::memory_order_release);
}

void    GpTask::UpDefferedWakeupFlag (GpMethodAccessGuard<GpTaskScheduler>) noexcept
{
    iDefferedWakeupFlag.test_and_set(std::memory_order_release);
}

bool    GpTask::IsDefferedWakeup (GpMethodAccessGuard<GpTaskScheduler>) const noexcept
{
    return iDefferedWakeupFlag.test(std::memory_order_acquire);
}

GpTask::StartFutureT::SP    GpTask::StartFuture (void)
{
    return iStartPromise.Future();
}

GpTask::DoneFutureT::SP GpTask::DoneFuture (void)
{
    return iDonePromise.Future();
}

GpTask::StartPromiseT&  GpTask::StartPromise (GpMethodAccessGuard<GpTask>) noexcept
{
    return iStartPromise;
}

GpTask::DonePromiseT&   GpTask::DonePromise (GpMethodAccessGuard<GpTask>) noexcept
{
    return iDonePromise;
}

GpTaskId    GpTask::SNextId (void) noexcept
{
    return sIdCounter.fetch_add(1, std::memory_order_relaxed);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
