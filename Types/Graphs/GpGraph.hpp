#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_GRAPHS)

#include "GpGraphNode.hpp"
#include "GpGraphEdge.hpp"

namespace GPlatform {

class GPCORE_API GpGraph
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpGraph)
    CLASS_DECLARE_DEFAULTS(GpGraph)

public:
                                GpGraph         (void) noexcept;
    virtual                     ~GpGraph        (void) noexcept;

    void                        AddNode         (GpGraphNode::SP aNode);
    void                        ConnectNodes    (GpGraphNode::SP aNodeFrom,
                                                 GpGraphNode::SP aNodeTo,
                                                 GpGraphEdge::SP aEdge);
    void                        RemoveAllNodes  (void);

    static void                 STest           (void);

private:
    GpGraphNode::C::Set::SP     iNodes;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_GRAPHS)
