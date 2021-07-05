#include "GpPipelineEvaluateCtx.hpp"

#if defined(GP_USE_PIPELINE)

namespace GPlatform {

GpPipelineEvaluateCtx::GpPipelineEvaluateCtx (std::string_view aName):
GpTaskFiber(aName)
{
}

GpPipelineEvaluateCtx::~GpPipelineEvaluateCtx (void) noexcept
{
}

void    GpPipelineEvaluateCtx::FiberFn (GpThreadStopToken /*aStopToken*/)
{
    //NOP
}

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
