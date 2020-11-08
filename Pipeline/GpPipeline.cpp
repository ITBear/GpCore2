#include "GpPipeline.hpp"

#if defined(GP_USE_PIPELINE)

namespace GPlatform {

GpPipeline::GpPipeline (void) noexcept
{
}

GpPipeline::~GpPipeline (void) noexcept
{
    Clear();
}

void    GpPipeline::Clear (void) noexcept
{
    std::scoped_lock lock(iLock);

    iRootNodes.clear();
    iAllNodes.clear();
}

void    GpPipeline::AddNode (NodeT::SP aNode)
{
    std::scoped_lock lock(iLock);

    THROW_GPE_COND_CHECK_M(iAllNodes.count(aNode) == 0, "Node already added"_sv);

    aNode->AssignToPipeline(*this);

    iRootNodes.emplace(aNode);
    iAllNodes.emplace(aNode);
}

void    GpPipeline::RemoveNode (NodeT::SP aNode)
{
    std::scoped_lock lock(iLock);

    auto iter = iAllNodes.find(aNode);
    THROW_GPE_COND_CHECK_M(iter != iAllNodes.end(), "Node not found"_sv);

    _BreakConnections(aNode);

    // Remove from allNodes set
    iAllNodes.erase(iter);
    iRootNodes.erase(aNode);
}

GpPipeline::ConnectorT  GpPipeline::ConnectNodes (NodeT::SP aNodeFrom, SocketT& aFrom,
                                                  NodeT::SP aNodeTo,   SocketT& aTo)
{   
    SocketT&    socketFrom  = aFrom;
    SocketT&    socketTo    = aTo;
    NodeT&      nodeFrom    = socketFrom.Node();
    NodeT&      nodeTo      = socketTo.Node();

    THROW_GPE_COND_CHECK_M(&nodeFrom == aNodeFrom.PCn(), "FROM Node != socket.Node"_sv);
    THROW_GPE_COND_CHECK_M(&nodeTo   == aNodeTo.PCn(), "FROM Node != socket.Node"_sv);
    THROW_GPE_COND_CHECK_M(nodeFrom.Pipeline() == this, "Node FROM not assigned to this pipeline"_sv);
    THROW_GPE_COND_CHECK_M(nodeTo.Pipeline() == this, "Node TO not assigned to this pipeline"_sv);
    THROW_GPE_COND_CHECK_M(socketFrom.Directrion() == GpPipelineNodeSocketDir::OUT, "Socket FROM must be OUT"_sv);
    THROW_GPE_COND_CHECK_M(socketTo.Directrion() == GpPipelineNodeSocketDir::IN, "Socket TO must be IN"_sv);
    THROW_GPE_COND_CHECK_M(socketTo.IsEmpty(), "Socket TO must be empty"_sv);
    THROW_GPE_COND_CHECK_M(socketFrom.TypeUID() == socketTo.TypeUID(), "FROM socket type UID != TO socket type UID"_sv);

    ConnectorT connector(socketFrom, socketTo);

    {
        std::scoped_lock lock(iLock);

        socketFrom.AddConnector(connector);
        socketTo.AddConnector(connector);

        iRootNodes.erase(aNodeTo);
    }

    return connector;
}

void    GpPipeline::BreakConnections (NodeT::SP aNode)
{
    std::scoped_lock lock(iLock);

    _BreakConnections(aNode);
}

GpPipeline::NodeT::C::Set::CSP  GpPipeline::RootNodes (void) const
{
    std::shared_lock lock(iLock);

    return iRootNodes;
}

void    GpPipeline::_BreakConnections (NodeT::SP /*aNode*/)
{
    THROW_NOT_IMPLEMENTED();
    /*auto iter = iNodes.find(aNode);

    THROW_GPE_COND_CHECK_M(iter != iNodes.end(), "Node not found"_sv);

    NodeT& n = aNode.V();
    n.BreakConnections();*/
}


}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
