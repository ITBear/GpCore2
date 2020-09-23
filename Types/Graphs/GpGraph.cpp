#include "GpGraph.hpp"
#include "../../Utils/RAII/GpRAIIutils.hpp"

#if defined(GP_USE_GRAPHS)

namespace GPlatform {

GpGraph::GpGraph (void) noexcept
{
}

GpGraph::~GpGraph (void) noexcept
{
}

void    GpGraph::AddNode (GpGraphNode::SP aNode)
{
    aNode.V().OnAssignToGraph(*this);

    const bool isAdded = std::get<1>(iNodes.emplace(std::move(aNode)));

    if (!isAdded)
    {
        aNode.Vn().OnRemoveFromGraph();

        THROW_GPE("Failed to add node"_sv);
    }
}

void    GpGraph::ConnectNodes (GpGraphNode::SP aNodeFrom,
                               GpGraphNode::SP aNodeTo,
                               GpGraphEdge::SP aEdge)
{
    //From
    if (iNodes.count(aNodeFrom) == 0)
    {
        THROW_GPE("Node FROM not in a graph"_sv);
    }

    //To
    if (iNodes.count(aNodeTo) == 0)
    {
        THROW_GPE("Node TO not in a graph"_sv);
    }

    GpOnThrowStackUnwindFn<std::function<void()>> onThrowStackUnwind;

    //Edge
    GpGraphEdge& edge = aEdge.V();

    onThrowStackUnwind.Push([&](){aEdge.Vn().OnDisconnectIn();});
    edge.OnConnectIn(aNodeTo);

    onThrowStackUnwind.Push([&](){aEdge.Vn().OnDisconnectOut();});
    edge.OnConnectOut(aNodeFrom);

    onThrowStackUnwind.Push([&](){aNodeFrom.Vn().OnDisconnectOut(aEdge);});
    aNodeFrom.Vn().OnConnectOut(aEdge);

    onThrowStackUnwind.Push([&](){aNodeTo.Vn().OnDisconnectIn(aEdge);});
    aNodeTo.Vn().OnConnectIn(aEdge);
}

void    GpGraph::RemoveAllNodes (void)
{
    iNodes.clear();
}

void    GpGraph::STest (void)
{
    GpGraph graph;

    GpGraphNode::SP node1 = MakeSP<GpGraphNode>();
    GpGraphNode::SP node2 = MakeSP<GpGraphNode>();

    graph.AddNode(node1);
    graph.AddNode(node2);
    graph.ConnectNodes(node1, node2, MakeSP<GpGraphEdge>());
    //graph.RemoveAllNodes();

    node1.Clear();
    node2.Clear();

    //graph.ConnectNodes(node2, node1, MakeSP<GpGraphEdge>());
    //graph.ConnectNodes(node1, node1, MakeSP<GpGraphEdge>());
    //graph.ConnectNodes(node2, node2, MakeSP<GpGraphEdge>());
}

}//namespace GPlatform

#endif//#if defined(GP_USE_GRAPHS)
