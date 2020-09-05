#include "GpPipelineNodeSum.hpp"

#if defined(GP_USE_PIPELINE)

#include "GpPipelineNodeSumEvCtx.hpp"

namespace GPlatform {

GpPipelineEvaluateCtx::SP   GpPipelineNodeSum::NewEvaluateCtx (void) const
{
    return MakeSP<GpPipelineNodeSumEvCtx>();
}

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
