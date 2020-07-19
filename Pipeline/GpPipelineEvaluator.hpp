#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_PIPELINE)

#include "GpPipeline.hpp"
#include "GpPipelineEvaluateCtx.hpp"

namespace GPlatform {

class GPCORE_API GpPipelineEvaluator: public GpTaskFiber
{
public:
    CLASS_REMOVE_CTRS(GpPipelineEvaluator)
    CLASS_DECLARE_DEFAULTS(GpPipelineEvaluator)

public:
                        GpPipelineEvaluator     (GpPipeline::CSP aPipeline) noexcept;
                        ~GpPipelineEvaluator    (void) noexcept override final;

protected:
    virtual void        FiberFn                 (GpThreadStopToken aStopToken) override final;

private:
    void                ProcessEvents           (GpPipelineNode::C::Set::CSP& aReadyNodesOut);

private:
    GpPipeline::CSP     iPipeline;
};

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
