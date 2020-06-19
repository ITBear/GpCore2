#include "GpPipelineNodeSocket.hpp"

#if defined(GP_USE_PIPELINE)

#include "GpPipelineNode.hpp"

namespace GPlatform {

GpPipelineNodeSocket::GpPipelineNodeSocket (GpPipelineNode&		aNode,
											const GpUUID&		aTypeUID,
											const DirectrionTE	aDirectrion,
											std::string_view	aName):
iNode(aNode),
iTypeUID(aTypeUID),
iDirectrion(aDirectrion),
iName(aName)
{
}

GpPipelineNodeSocket::~GpPipelineNodeSocket (void) noexcept
{
}

void	GpPipelineNodeSocket::BreakConnections (void)
{
	if (iDirectrion == DirectrionTE::IN)
	{
		for (ConnectorT& connector: iConnectors)
		{
			GpPipelineNodeSocket& otherSocket = connector.From();
			otherSocket._BreakConnection(connector);
		}
	} else //(iDirectrion == DirectrionTE::OUT)
	{
		for (ConnectorT& connector: iConnectors)
		{
			GpPipelineNodeSocket& otherSocket = connector.To();
			otherSocket._BreakConnection(connector);
		}
	}

	iConnectors.clear();
}

void	GpPipelineNodeSocket::AddConnector (const ConnectorT& aConnector)
{
	iConnectors.emplace_back(aConnector);
}

void	GpPipelineNodeSocket::_BreakConnection (const ConnectorT& aConnector)
{
	for (auto iter = iConnectors.begin(); iter != iConnectors.end(); ++iter)
	{
		if (iter->IsSame(aConnector))
		{
			iConnectors.erase(iter);
			return;
		}
	}

	THROW_GPE("Connector not found"_sv);
}

}//namespace GPlatform

#endif//GP_USE_PIPELINE
