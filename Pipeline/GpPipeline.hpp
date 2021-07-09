#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_PIPELINE)

#include "GpPipelineNode.hpp"

namespace GPlatform {

class GPCORE_API GpPipeline
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpPipeline)
    CLASS_DECLARE_DEFAULTS(GpPipeline)

    using NodeT         = GpPipelineNode;
    using SocketT       = GpPipelineNodeSocket;
    using ConnectorT    = GpPipelineNodeConnector;

public:
                                GpPipeline          (void) noexcept;
                                ~GpPipeline         (void) noexcept;

    void                        Clear               (void) noexcept;
    void                        AddNode             (NodeT::SP aNode);
    void                        RemoveNode          (NodeT::SP aNode);
    ConnectorT                  ConnectNodes        (NodeT::SP aNodeFrom, SocketT& aFrom,
                                                     NodeT::SP aNodeTo,   SocketT& aTo);
    void                        BreakConnections    (NodeT::SP aNode);

    NodeT::C::Set::CSP          RootNodes           (void) const;

private:
    void                        _BreakConnections   (NodeT::SP aNode);

private:
    mutable GpRWLock            iLock;
    NodeT::C::Set::CSP          iRootNodes;
    NodeT::C::Set::SP           iAllNodes;
};

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
