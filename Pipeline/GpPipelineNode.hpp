#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_PIPELINE)

#include "GpPipelineNodeSocket.hpp"
#include "GpPipelineEvaluateCtx.hpp"

namespace GPlatform {

class GpPipeline;

class GPCORE_API GpPipelineNode
{
public:
	CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpPipelineNode);
	CLASS_DECLARE_DEFAULTS(GpPipelineNode);

	using SocketT = GpPipelineNodeSocket;

protected:
										GpPipelineNode			(void) noexcept;

public:
	virtual								~GpPipelineNode			(void) noexcept;

	const GpPipeline*					Pipeline				(void) const noexcept {return iPipeline;}
	GpPipeline*							Pipeline				(void) noexcept {return iPipeline;}
	void								AssignToPipeline		(GpPipeline& aPipeline);
	void								BreakConnections		(void);
	void								BreakInputConnections	(void);
	void								BreakOutputConnections	(void);

	virtual GpPipelineEvaluateCtx::SP	NewEvaluateCtx			(void) const = 0;

private:
	static void							_SBreakConnections		(SocketT::C::Vec::SP& aSockets);

private:
	GpPipeline*							iPipeline = nullptr;
	std::string							iName;
	SocketT::C::Vec::SP					iInputSockets;
	SocketT::C::Vec::SP					iOutputSockets;
};

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
