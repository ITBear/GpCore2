#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../GpTask.hpp"
#include "GpTaskFiberStage.hpp"

namespace GPlatform {

class GpTaskFiberCtx;

class GPCORE_API GpTaskFiber: public GpTask
{   
public: 
    CLASS_REMOVE_CTRS(GpTaskFiber)
    CLASS_DECLARE_DEFAULTS(GpTaskFiber)

    using StageT = GpTaskFiberStage;

public:
                                GpTaskFiber         (std::string_view aName);
    virtual                     ~GpTaskFiber        (void) noexcept override;

    virtual ResT                Do                  (GpThreadStopToken aStopToken) noexcept override final;
    virtual void                Terminate           (void) noexcept override final;

    virtual void                FiberFn             (GpThreadStopToken aStopToken) = 0;
    GpTaskFiber::WP             GetWeakPtr          (void) const noexcept {return GpTask::GetWeakPtr().As<GpTaskFiber::WP>();}

    static void                 SYield              (const GpTask::ResT aRes);
    static GpWP<GpTaskFiber>    SCurrentTask        (void);
    static bool                 SIsIntoFiber        (void) noexcept;

private:
    GpSP<GpTaskFiberCtx>        iCtx;
    StageT                      iStage = StageT::NOT_RUN;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
