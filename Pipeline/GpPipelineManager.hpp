#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_PIPELINE)

#include "Nodes/GpPipelineNodes.hpp"
#include "GpPipeline.hpp"
#include "GpPipelineEvaluator.hpp"

namespace GPlatform {

class GpPipelineManager
{
public:
	GpPipelineManager();
		~GpPipelineManager();
};

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
