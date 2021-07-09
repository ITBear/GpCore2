#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_GRAPHS)

#include "GpGraphEdge.hpp"

namespace GPlatform {

class GpGraph;

class GPCORE_API GpGraphNode
{
    friend class GpGraph;

public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpGraphNode)
    CLASS_DECLARE_DEFAULTS(GpGraphNode)

    using GraphRefT = std::optional<std::reference_wrapper<GpGraph>>;

public:
                                    GpGraphNode         (void) noexcept;
    virtual                         ~GpGraphNode        (void) noexcept;

    GraphRefT                       Graph               (void) noexcept {return iGraph;}
    void                            RemoveEdge          (GpGraphEdge::SP aEdge);
    void                            RemoveAllEdges      (void) noexcept;

protected:
    virtual void                    OnAssignToGraph     (GpGraph& aGraph);
    virtual void                    OnRemoveFromGraph   (void) noexcept;
    virtual void                    OnConnectIn         (GpGraphEdge::SP aEdgeIn);
    virtual void                    OnConnectOut        (GpGraphEdge::SP aEdgeOut);
    virtual void                    OnDisconnectIn      (GpGraphEdge::SP aEdgeIn) noexcept;
    virtual void                    OnDisconnectOut     (GpGraphEdge::SP aEdgeOut) noexcept;

    std::optional<size_t>           FindEdgeId          (const GpGraphEdge::SP&         aEdge,
                                                         const GpGraphEdge::C::Vec::SP& aEdges) const noexcept;

private:
    GpGraphEdge::C::Vec::SP         iEdgesIn;
    GpGraphEdge::C::Vec::SP         iEdgesOut;
    GraphRefT                       iGraph;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_GRAPHS)
