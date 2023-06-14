#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTask.hpp"

namespace GPlatform {

class GpTaskAccessor
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskAccessor)

public:
    using StateT    = GpTaskState;
    using StateTE   = StateT::EnumT;

public:
    static StateTE      SState                  (GpTask& aTask) noexcept {return aTask.State();}
    static void         SUpdateState            (GpTask& aTask,
                                                 StateTE aNewState) noexcept {aTask.UpdateState(aNewState);}
    static GpTaskDoRes  SRun                    (GpTask&            aTask,
                                                 GpThreadStopToken  aStopToken) noexcept {return aTask.Run(std::move(aStopToken));}
    static void         SRequestStop            (GpTask& aTask) noexcept {return aTask.RequestStop();}
    static void         SAddExecutorThreadId    (const std::thread::id& aThreadId) {GpTask::SAddExecutorThreadId(aThreadId);}
    static void         SSetParentTask          (GpTask&    aTask,
                                                 GpTask*    aParentTask) noexcept {aTask.SetParentTask(aParentTask);}
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
