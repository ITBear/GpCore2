#include "GpGraphEdge.hpp"

#if defined(GP_USE_GRAPHS)

#include "GpGraphNode.hpp"

namespace GPlatform {

GpGraphEdge::GpGraphEdge (void) noexcept
{
}

GpGraphEdge::~GpGraphEdge (void) noexcept
{
}

void    GpGraphEdge::OnConnectIn (GpGraphNode::WP aInNode)
{
    THROW_GPE_COND
    (
        iNodeIn.IsNULL(),
        "In node already connected"_sv
    );

    iNodeIn = aInNode;
}

void    GpGraphEdge::OnConnectOut (GpGraphNode::WP aOutNode)
{
    THROW_GPE_COND
    (
        iNodeOut.IsNULL(),
        "Out node already connected"_sv
    );

    iNodeOut = aOutNode;
}

void    GpGraphEdge::OnDisconnectIn (void) noexcept
{
    iNodeIn.Clear();
}

void    GpGraphEdge::OnDisconnectOut (void) noexcept
{
    iNodeOut.Clear();
}

}//namespace GPlatform

#endif//#if defined(GP_USE_GRAPHS)
