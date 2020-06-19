#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_PIPELINE)

#include "../Types/TypeSystem/GpTypeSystem.hpp"
#include "GpPipelineNodeConnector.hpp"
#include "GpPipelineNodeSocketDir.hpp"

namespace GPlatform {

class GpPipelineNode;

class GPCORE_API GpPipelineNodeSocket
{
public:
	CLASS_REMOVE_CTRS(GpPipelineNodeSocket);
	CLASS_DECLARE_DEFAULTS(GpPipelineNodeSocket);

	using ConnectorT	= GpPipelineNodeConnector;
	using DirectrionT	= GpPipelineNodeSocketDir;
	using DirectrionTE	= DirectrionT::EnumT;

public:
								GpPipelineNodeSocket	(GpPipelineNode&	aNode,
														 const GpUUID&		aTypeUID,
														 const DirectrionTE	aDirectrion,
														 std::string_view	aName);
								~GpPipelineNodeSocket	(void) noexcept;

	const GpPipelineNode&		Node					(void) const noexcept {return iNode;}
	GpPipelineNode&				Node					(void) noexcept {return iNode;}
	const GpUUID&				TypeUID					(void) const noexcept {return iTypeUID;}
	DirectrionTE				Directrion				(void) const noexcept {return iDirectrion;}
	std::string_view			Name					(void) const noexcept {return iName;}

	bool						IsEmpty					(void) const noexcept {return iConnectors.empty();}
	void						BreakConnections		(void);
	void						AddConnector			(const ConnectorT& aConnector);

private:
	void						_BreakConnection		(const ConnectorT& aConnector);

private:
	GpPipelineNode&				iNode;
	GpUUID						iTypeUID;
	ConnectorT::C::Vec::Val		iConnectors;
	const DirectrionTE			iDirectrion;
	std::string					iName;
};

}//namespace GPlatform

#endif//GP_USE_PIPELINE
