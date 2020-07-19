#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_PIPELINE)

#include "../Types/Classes/GpClassesDefines.hpp"
#include "../Types/Containers/GpContainersT.hpp"
#include "../Multithreading/Tasks/GpTasks.hpp"

namespace GPlatform {

class GPCORE_API GpPipelineEvaluateCtx: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpPipelineEvaluateCtx)
    CLASS_DECLARE_DEFAULTS(GpPipelineEvaluateCtx)

protected:
                        GpPipelineEvaluateCtx   (void) noexcept = default;

public:
    virtual             ~GpPipelineEvaluateCtx  (void) noexcept override = default;

protected:
    virtual void        FiberFn                 (GpThreadStopToken aStopToken) override final;
};

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
