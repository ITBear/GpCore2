#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../GpTasks_global.hpp"

#include "../../GpUtils/Macro/GpMacroClass.hpp"
#include "../../GpUtils/Types/Containers/GpContainersT.hpp"
#include "../../GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp"
#include "../../GpUtils/SyncPrimitives/GpConditionVarFlag.hpp"

#include <boost/container/flat_set.hpp>

namespace GPlatform {

class GP_TASKS_API GpItcSharedCondition
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpItcSharedCondition)
    CLASS_DD(GpItcSharedCondition)

    enum class NotifyMode
    {
        ONE,
        ALL
    };

    using AtomicFnT     = GpConditionVar::AtomicFnT;
    using AtomicFnOptT  = std::optional<std::reference_wrapper<const AtomicFnT>>;
    using CondFnT       = std::function<bool(std::mutex& aLock)>;
    using TaskGuidsT    = boost::container::flat_set<u_int_64>;

public:
                            GpItcSharedCondition    (void) noexcept = default;
                            ~GpItcSharedCondition   (void) noexcept = default;

    inline void             DoAtomic                (const AtomicFnT& aFn) const;
    void                    Notify                  (const NotifyMode   aNotifyMode,
                                                     AtomicFnOptT       aBeforeSendNotifyFn = std::nullopt);
    bool                    WaitFor                 (const milliseconds_t   aTimeout,
                                                     const CondFnT&         aCondFn);

private:
    bool                    WaitForFiber            (const u_int_64         aTaskId,
                                                     const milliseconds_t   aTimeout,
                                                     const CondFnT&         aCondFn);
    bool                    WaitForThread           (const milliseconds_t   aTimeout,
                                                     const CondFnT&         aCondFn);

private:
    // For waiting threads
    GpConditionVarFlag      iThreadsFlagCV;
    std::atomic<size_t>     iThreadsWaiting;

    // For waiting fiber tasks
    TaskGuidsT              iFiberTaskIds;
};

void    GpItcSharedCondition::DoAtomic (const AtomicFnT& aFn) const
{
    iThreadsFlagCV.DoAtomic(aFn);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
