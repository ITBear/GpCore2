#include "GpGraphNode.hpp"

#if defined(GP_USE_GRAPHS)

#include "../../Algorithms/GpFind.hpp"
#include "GpGraph.hpp"

namespace GPlatform {

GpGraphNode::GpGraphNode (void) noexcept
{
}

GpGraphNode::~GpGraphNode (void) noexcept
{
    RemoveAllEdges();
}

void    GpGraphNode::RemoveEdge (GpGraphEdge::SP aEdge)
{
    std::optional<size_t> edgeIn_id     = FindEdgeId(aEdge, iEdgesIn);
    std::optional<size_t> edgeOut_Id    = FindEdgeId(aEdge, iEdgesOut);

    THROW_GPE_COND
    (
        (
               edgeIn_id.has_value()
            || edgeOut_Id.has_value()
        ),
        "Edge not found"_sv
    );

    if (edgeIn_id.has_value())
    {
        OnDisconnectIn(aEdge);
        aEdge->OnDisconnectIn();
    }

    if (edgeOut_Id.has_value())
    {
        OnDisconnectOut(aEdge);
        aEdge->OnDisconnectOut();
    }
}

void    GpGraphNode::RemoveAllEdges (void) noexcept
{
    for (GpGraphEdge::SP egde: iEdgesIn)
    {
        OnDisconnectIn(egde);
        egde->OnDisconnectIn();
    }

    for (GpGraphEdge::SP egde: iEdgesOut)
    {
        OnDisconnectOut(egde);
        egde->OnDisconnectOut();
    }

    iEdgesIn.clear();
    iEdgesOut.clear();
}

void    GpGraphNode::OnAssignToGraph (GpGraph& aGraph)
{
    THROW_GPE_COND
    (
        !iGraph.has_value(),
        "Node already assigned to graph"_sv
    );

    iGraph = aGraph;
}

void    GpGraphNode::OnRemoveFromGraph (void) noexcept
{
    if (iGraph.has_value() == false)
    {
        return;
    }

    RemoveAllEdges();
    iGraph.reset();
}

void    GpGraphNode::OnConnectIn (GpGraphEdge::SP aEdgeIn)
{
    THROW_GPE_COND
    (
        FindEdgeId(aEdgeIn, iEdgesIn).has_value() == false,
        "Edge already connected as IN"_sv
    );

    iEdgesIn.emplace_back(aEdgeIn);
}

void    GpGraphNode::OnConnectOut (GpGraphEdge::SP aEdgeOut)
{
    THROW_GPE_COND
    (
        FindEdgeId(aEdgeOut, iEdgesOut).has_value() == false,
        "Edge already connected as OUT"_sv
    );

    iEdgesOut.emplace_back(aEdgeOut);
}

void    GpGraphNode::OnDisconnectIn (GpGraphEdge::SP aEdgeIn) noexcept
{
    std::optional<size_t> edge_id = FindEdgeId(aEdgeIn, iEdgesIn);

    if (edge_id.has_value())
    {
        iEdgesIn.erase(iEdgesIn.begin() + edge_id.value());
    }
}

void    GpGraphNode::OnDisconnectOut (GpGraphEdge::SP aEdgeOut) noexcept
{
    std::optional<size_t> edge_id = FindEdgeId(aEdgeOut, iEdgesOut);

    if (edge_id.has_value())
    {
        iEdgesOut.erase(iEdgesOut.begin() + edge_id.value());
    }
}

std::optional<size_t>   GpGraphNode::FindEdgeId
(
    const GpGraphEdge::SP&          aEdge,
    const GpGraphEdge::C::Vec::SP&  aEdges
) const noexcept
{
    return Algo::FindElementId(aEdges, aEdge);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_GRAPHS)
