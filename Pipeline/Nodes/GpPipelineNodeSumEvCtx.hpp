#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_PIPELINE)

#include "../GpPipelineEvaluateCtx.hpp"

namespace GPlatform {

class GPCORE_API GpPipelineNodeSumEvCtx: public GpPipelineEvaluateCtx
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpPipelineNodeSumEvCtx)
    CLASS_DECLARE_DEFAULTS(GpPipelineNodeSumEvCtx)

public:
                                        GpPipelineNodeSumEvCtx  (void) noexcept = default;
    virtual                             ~GpPipelineNodeSumEvCtx (void) noexcept override final = default;
};

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
