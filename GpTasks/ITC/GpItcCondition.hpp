#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_set.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpConditionVar.hpp>
#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtx.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GP_TASKS_API CAPABILITY("GpItcCondition") GpItcCondition
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcCondition)
    CLASS_DD(GpItcCondition)
    TAG_SET(THREAD_SAFE)

    using FiberTaskIDsT = boost::container::small_flat_set<GpTaskId, 2>;
    using TaskInfo      = std::tuple<GpTaskMode::EnumT, GpTaskId>;
    using AtBeginFnT    = std::function<void()>;
    using AtEndFnT      = std::function<void(bool)>;// must be noexcept
    using CheckFnT      = std::function<bool()>;

    struct ThreadWatingData
    {
        using UP = std::unique_ptr<ThreadWatingData>;

        mutable GpConditionVar  iCV;
        std::atomic_uint32_t    iWaiting;
    };

public:
                        GpItcCondition      (void) noexcept = default;
                        ~GpItcCondition     (void) noexcept = default;

    GpSpinLockRW&       SpinLock            (void) const noexcept RETURN_CAPABILITY(iFiberSpinLockRW) {return iFiberSpinLockRW;}
    bool                NotifyOne           (void) REQUIRES(iFiberSpinLockRW);
    bool                NotifyAll           (void) REQUIRES(iFiberSpinLockRW);

    void                SubscribeAsFiber    (GpTaskId aGpTaskId);
    bool                UnsubscribeAsFiber  (GpTaskId aGpTaskId);

    inline void         Wait                (const CheckFnT&    aCheckFn);
    inline void         Wait                (const CheckFnT&    aCheckFn,
                                             const AtBeginFnT&  aAtBeginFn,
                                             const AtEndFnT&    aAtEndFn);

    inline bool         WaitFor             (const CheckFnT&    aCheckFn,
                                             milliseconds_t     aTimeout);
    inline bool         WaitFor             (const CheckFnT&    aCheckFn,
                                             milliseconds_t     aTimeout,
                                             const AtBeginFnT&  aAtBeginFn,
                                             const AtEndFnT&    aAtEndFn);

private:
    bool                WaitForFiber        (const CheckFnT&    aCheckFn,
                                             milliseconds_t     aTimeout,
                                             const AtBeginFnT&  aAtBeginFn,
                                             const AtEndFnT&    aAtEndFn,
                                             GpTaskId           aFiberTaskId);
    bool                WaitForThread       (const CheckFnT&    aCheckFn,
                                             milliseconds_t     aTimeout,
                                             const AtBeginFnT&  aAtBeginFn,
                                             const AtEndFnT&    aAtEndFn);

    static TaskInfo     SCurrentTaskInfo    (void);
    static void         SYield              (milliseconds_t aTimeout);
    static void         SYield              (void);

private:
    FiberTaskIDsT           iFiberTaskIDs       GUARDED_BY(iFiberSpinLockRW);   // For waiting fiber tasks
    ThreadWatingData::UP    iThreadWatingDataUP;                                // For waiting threads
    mutable GpSpinLockRW    iFiberSpinLockRW;
};

void    GpItcCondition::Wait (const CheckFnT& aCheckFn)
{
    WaitFor(aCheckFn, 0.0_si_ms);
}

void    GpItcCondition::Wait
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

bool    GpItcCondition::WaitFor
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
        atBeginFn,
        atEndFn
    );
}

bool    GpItcCondition::WaitFor
(
    const CheckFnT&         aCheckFn,
    const milliseconds_t    aTimeout,
    const AtBeginFnT&       aAtBeginFn,
    const AtEndFnT&         aAtEndFn
)
{
    // Collect current task info
    const auto[taskMode, taskId] = SCurrentTaskInfo();

    if (taskMode == GpTaskMode::FIBER) [[likely]]
    {
        return WaitForFiber
        (
            aCheckFn,
            aTimeout,
            aAtBeginFn,
            aAtEndFn,
            taskId
        );
    } else // taskMode == GpTaskMode::THREAD
    {
        return WaitForThread
        (
            aCheckFn,
            aTimeout,
            aAtBeginFn,
            aAtEndFn
        );
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
