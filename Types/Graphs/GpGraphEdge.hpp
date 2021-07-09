#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_GRAPHS)

#include "../TypeSystem/GpTypeSystem.hpp"

namespace GPlatform {

class GpGraph;
class GpGraphNode;

class GPCORE_API GpGraphEdge
{
    friend class GpGraph;
    friend class GpGraphNode;

public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpGraphEdge)
    CLASS_DECLARE_DEFAULTS(GpGraphEdge)

    using NodeRefT = std::optional<std::reference_wrapper<GpGraphNode>>;

public:
                            GpGraphEdge         (void) noexcept;
    virtual                 ~GpGraphEdge        (void) noexcept;

    GpWP<GpGraphNode>       NodeIn              (void) noexcept {return iNodeIn;}
    GpWP<GpGraphNode>       NodeOut             (void) noexcept {return iNodeOut;}

protected:
    virtual void            OnConnectIn         (GpWP<GpGraphNode> aInNode);
    virtual void            OnConnectOut        (GpWP<GpGraphNode> aOutNode);
    virtual void            OnDisconnectIn      (void) noexcept;
    virtual void            OnDisconnectOut     (void) noexcept;

private:
    GpWP<GpGraphNode>       iNodeIn;
    GpWP<GpGraphNode>       iNodeOut;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_GRAPHS)
