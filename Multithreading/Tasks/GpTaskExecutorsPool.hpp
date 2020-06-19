#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../../Types/Containers/GpElementsPool.hpp"
#include "../Threads/GpThread.hpp"
#include "GpTaskExecutor.hpp"

namespace GPlatform {

class GPCORE_API GpTaskExecutorsPool final: public GpElementsPool<GpTaskExecutor*>
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTaskExecutorsPool);
    CLASS_DECLARE_DEFAULTS(GpTaskExecutorsPool);

    using ExecutorT = GpTaskExecutor;

public:
                                GpTaskExecutorsPool     (GpTaskScheduler& aScheduler) noexcept;
    virtual                     ~GpTaskExecutorsPool    (void) noexcept override final;

    void                        RequestStop             (void) noexcept;
    void                        Join                    (void) noexcept;
    void                        WakeupAll               (void) noexcept;

protected:
    virtual void                PreInit                 (const count_t aCount) override final;
    virtual value_type          NewElement              (void) override final;
    virtual void                OnClear                 (void) noexcept override final;

private:
    GpTaskScheduler&            iScheduler;
    GpThread::C::Vec::Val       iThreads;
    ExecutorT::C::Vec::SP       iExecutors;
    count_t                     iExecutorsLeft  = 0_cnt;
    GpConditionVar::SP          iExecutorsCondVar;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
