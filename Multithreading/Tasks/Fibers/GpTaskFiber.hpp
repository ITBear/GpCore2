#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../GpTask.hpp"
#include "GpTaskFiberCtx.hpp"
#include "GpTaskFiberStage.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFiber: public GpTask
{   
public: 
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpTaskFiber);
    CLASS_DECLARE_DEFAULTS(GpTaskFiber);

    using StageT = GpTaskFiberStage;

public:
                            GpTaskFiber         (void) noexcept;
    virtual                 ~GpTaskFiber        (void) noexcept override;

    virtual Res             Do                  (GpThreadStopToken aStopToken) noexcept override final;

protected:
    virtual void            FiberFn             (GpThreadStopToken aStopToken) = 0;

private:
    GpTaskFiberCtx::SP      iCtx;
    StageT                  iStage = StageT::NOT_RUN;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)