#pragma once

#include "GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTaskExecutor.hpp"

namespace GPlatform {

class GpTask;

class GP_TASKS_API GpTaskExecutorsPool final: public GpElementsPool<GpTaskExecutor*>
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskExecutorsPool)
    CLASS_DD(GpTaskExecutorsPool)

    using ExecutorT = GpTaskExecutor;

public:
                            GpTaskExecutorsPool     (void) noexcept;
    virtual                 ~GpTaskExecutorsPool    (void) noexcept override final;

    void                    RequestStop             (void) noexcept;
    void                    Join                    (void) noexcept;
    inline void             WakeupAll               (void) noexcept;
    inline void             WakeupOne               (void) noexcept;

protected:
    virtual void            PreInit                 (const size_t aCount) override final;
    virtual value_type      NewElement              (GpSpinlock& aLocked) override final;
    virtual void            OnClear                 (void) noexcept override final;

private:
    GpThread::C::Vec::Val   iThreads;
    ExecutorT::C::Vec::SP   iExecutors;
    size_t                  iExecutorsLeft  = 0;
    GpConditionVar::SP      iExecutorsCondVar;
};

void    GpTaskExecutorsPool::WakeupAll (void) noexcept
{
    iExecutorsCondVar.Vn().WakeupAll();
}

void    GpTaskExecutorsPool::WakeupOne (void) noexcept
{
    iExecutorsCondVar.Vn().WakeupOne();
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
