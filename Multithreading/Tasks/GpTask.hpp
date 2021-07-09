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
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTask)
    CLASS_DECLARE_DEFAULTS(GpTask)

    enum class ResT
    {
        READY_TO_EXEC,
        WAITING,
        DONE
    };

    using StateT        = GpTaskState;
    using StateTE       = StateT::EnumT;
    using SchedulerRefT = std::optional<std::reference_wrapper<GpTaskScheduler>>;

protected:
                            GpTask          (std::string_view aName);

public:
    virtual                 ~GpTask         (void) noexcept override;

    std::string_view        Name            (void) const noexcept {return iName;}

    void                    MoveToReady     (void);
    virtual ResT            Do              (GpThreadStopToken aStopToken) noexcept = 0;
    virtual void            Terminate       (void) noexcept = 0;

    void                    JoinForState    (StateTE aStateTE) const;

protected:
    virtual void            OnPushEvent     (void) override final;
    GpTask::WP              GetWeakPtr      (void) const noexcept {return iThisWeakPtr;}

private:
    StateTE                 State           (void) const noexcept {return iState;}
    void                    UpdateState     (StateTE aNewState) noexcept {iState = aNewState;}
    void                    SetScheduler    (GpTaskScheduler& aScheduler) noexcept {iScheduler = aScheduler;}
    void                    SetWeakPtr      (GpTask::WP aWeakPtr) noexcept {iThisWeakPtr = std::move(aWeakPtr);}

private:
    const std::string       iName;
    StateTE                 iState = StateTE::NOT_ASSIGNED_TO_SCHEDULER;
    SchedulerRefT           iScheduler;
    GpTask::WP              iThisWeakPtr;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
