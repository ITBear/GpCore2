#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_PIPELINE)

#include "../GpPipelineNode.hpp"

namespace GPlatform {

class GPCORE_API GpPipelineNodeSum final: public GpPipelineNode
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpPipelineNodeSum)
    CLASS_DECLARE_DEFAULTS(GpPipelineNodeSum)

public:
                                        GpPipelineNodeSum   (void) noexcept;
    virtual                             ~GpPipelineNodeSum  (void) noexcept override final;

    virtual GpPipelineEvaluateCtx::SP   NewEvaluateCtx      (std::string_view aName) const override final;
};

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
