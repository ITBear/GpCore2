#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_set.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpConditionVar.hpp>
#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtx.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpTask;

class GP_TASKS_API GpItcConditionVar
{
public:
    CLASS_REMOVE_CTRS_COPY(GpItcConditionVar)
    CLASS_DD(GpItcConditionVar)
    TAG_SET(THREAD_SAFE)

    struct TaskInfo
    {
        GpWP<GpTask>    iTaskWP;
        bool            iIsSubscriber;
    };

    using TasksT        = boost::container::small_flat_set<TaskInfo, 2, std::less<>>;
    using AtBeginFnT    = std::function<void()>;
    using AtEndFnT      = std::function<void(bool)>;// must be noexcept
    using CheckFnT      = std::function<bool()>;

public:
                        GpItcConditionVar   (void) noexcept;
                        GpItcConditionVar   (GpItcConditionVar&& aConditionVar) noexcept;
                        ~GpItcConditionVar  (void) noexcept;

    GpSpinLockRW<>&     SpinLockRW          (void) const noexcept RETURN_CAPABILITY(iSpinLockRW) {return iSpinLockRW;}
    void                NotifyOne           (void) REQUIRES(iSpinLockRW);
    void                NotifyAll           (void) REQUIRES(iSpinLockRW);

    // For wakeup without Wait call
    void                Subscribe           (GpWP<GpTask> aTaskWP);
    bool                Unsubscribe         (GpWP<GpTask> aTaskWP);

    void                Wait                (const CheckFnT&    aCheckFn);
    void                Wait                (const CheckFnT&    aCheckFn,
                                             const AtBeginFnT&  aAtBeginFn,
                                             const AtEndFnT&    aAtEndFn);
    bool                WaitFor             (const CheckFnT&    aCheckFn,
                                             milliseconds_t     aTimeout);
    bool                WaitFor             (const CheckFnT&    aCheckFn,
                                             milliseconds_t     aTimeout,
                                             const AtBeginFnT&  aAtBeginFn,
                                             const AtEndFnT&    aAtEndFn);

private:
    mutable GpSpinLockRW<>  iSpinLockRW;
    TasksT                  iTasks GUARDED_BY(iSpinLockRW);
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
