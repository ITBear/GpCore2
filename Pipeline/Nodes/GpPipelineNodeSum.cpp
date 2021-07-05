#include "GpPipelineNodeSum.hpp"

#if defined(GP_USE_PIPELINE)

#include "GpPipelineNodeSumEvCtx.hpp"

namespace GPlatform {

GpPipelineNodeSum::GpPipelineNodeSum (void) noexcept
{
}

GpPipelineNodeSum::~GpPipelineNodeSum (void) noexcept
{
}

GpPipelineEvaluateCtx::SP   GpPipelineNodeSum::NewEvaluateCtx (std::string_view aName) const
{
    return MakeSP<GpPipelineNodeSumEvCtx>(aName);
}

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
