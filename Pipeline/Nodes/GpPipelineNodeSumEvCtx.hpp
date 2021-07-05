#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_PIPELINE)

#include "../GpPipelineEvaluateCtx.hpp"

namespace GPlatform {

class GPCORE_API GpPipelineNodeSumEvCtx final: public GpPipelineEvaluateCtx
{
public:
    CLASS_REMOVE_CTRS(GpPipelineNodeSumEvCtx)
    CLASS_DECLARE_DEFAULTS(GpPipelineNodeSumEvCtx)

public:
                                        GpPipelineNodeSumEvCtx  (std::string_view aName);
    virtual                             ~GpPipelineNodeSumEvCtx (void) noexcept override final;
};

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
