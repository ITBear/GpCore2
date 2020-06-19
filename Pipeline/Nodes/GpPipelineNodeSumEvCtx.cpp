#include "GpPipelineNodeSumEvCtx.hpp"

#if defined(GP_USE_PIPELINE)

namespace GPlatform {

/*void	GpPipelineNodeSumEvCtx::DoCalc (void) const
{
	// Read inputs
	GpVector<SInt_64> inputs;

	// Do calc
	const SInt_64 sum = SSum(inputs);

	// Write outputs
}

SInt_64	GpPipelineNodeSumEvCtx::SSum (const GpVector<SInt_64>& aInputs)
{
	try
	{
		SInt_64 sum = 0;
		for (const SInt_64 val: aInputs)
		{
			sum = NumOps::SAdd(sum, val);
		}
	} catch (...)
	{
		THROW_GPE("SInt_64 overflow"_sv);
	}
}*/

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
