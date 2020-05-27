#include "GpPipelineNodeSumEvCtx.hpp"

#if defined(GP_USE_PIPELINE)

namespace GPlatform {

/*void	GpPipelineNodeSumEvCtx::DoCalc (void) const
{
	// Read inputs
	GpVector<s_int_64> inputs;

	// Do calc
	const s_int_64 sum = SSum(inputs);

	// Write outputs
}

s_int_64	GpPipelineNodeSumEvCtx::SSum (const GpVector<s_int_64>& aInputs)
{
	try
	{
		s_int_64 sum = 0;
		for (const s_int_64 val: aInputs)
		{
			sum = NumOps::SAdd(sum, val);
		}
	} catch (...)
	{
		THROW_GPE("s_int_64 overflow"_sv);
	}
}*/

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)