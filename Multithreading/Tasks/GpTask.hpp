#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../Types/Containers/GpContainersT.hpp"
#include "../Threads/GpThreadStopToken.hpp"
#include "../../Types/Classes/GpClassesDefines.hpp"
#include "../../EventBus/GpEventSubscriber.hpp"
#include "GpTaskState.hpp"

namespace GPlatform {

class GpTaskAccessor;
class GpTaskScheduler;

class GPCORE_API GpTask: public GpEventSubscriber
{
    friend class GpTaskAccessor;

public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTask)
    CLASS_DECLARE_DEFAULTS(GpTask)

    enum class Res
    {
        READY_TO_EXEC,
        WAITING,
        DONE
    };

    using StateT        = GpTaskState;
    using StateTE       = StateT::EnumT;
    using SchedulerRefT = std::optional<std::reference_wrapper<GpTaskScheduler>>;

public:
                            GpTask          (void) noexcept = default;
    virtual                 ~GpTask         (void) noexcept override = default;

    void                    MoveToReady     (void);
    virtual Res             Do              (GpThreadStopToken aStopToken) noexcept = 0;
    virtual void            Terminate       (void) noexcept;

protected:
    virtual PushEvevtRes    OnPushEvent     (GpEvent::SP& aEvent) noexcept override final;
    GpTask::WP              GetWeakPtr      (void) const noexcept {return iThisWeakPtr;}

private:
    StateTE                 State           (void) const noexcept {return iState;}
    void                    UpdateState     (StateTE aNewState) noexcept {iState = aNewState;}
    void                    SetScheduler    (GpTaskScheduler& aScheduler) noexcept {iScheduler = aScheduler;}
    void                    SetWeakPtr      (WP&& aWeakPtr) noexcept {iThisWeakPtr = std::move(aWeakPtr);}

private:
    StateTE                 iState = StateTE::NOT_ASSIGNED_TO_SCHEDULER;
    SchedulerRefT           iScheduler;
    WP                      iThisWeakPtr;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
