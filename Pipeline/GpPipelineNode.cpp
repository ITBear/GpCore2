#include "GpPipelineNode.hpp"

#if defined(GP_USE_PIPELINE)

namespace GPlatform {

GpPipelineNode::GpPipelineNode (void) noexcept
{
}

GpPipelineNode::~GpPipelineNode (void) noexcept
{
}

void    GpPipelineNode::AssignToPipeline (GpPipeline& aPipeline)
{
    THROW_GPE_COND_CHECK_M(iPipeline == nullptr, "Node assigned to pipeline already"_sv);
    iPipeline = &aPipeline;
}

void    GpPipelineNode::BreakConnections (void)
{
    BreakInputConnections();
    BreakOutputConnections();
}

void    GpPipelineNode::BreakInputConnections (void)
{
    _SBreakConnections(iInputSockets);
}

void    GpPipelineNode::BreakOutputConnections (void)
{
    _SBreakConnections(iOutputSockets);
}

void    GpPipelineNode::_SBreakConnections (SocketT::C::Vec::SP& aSockets)
{
    for (SocketT::SP& socketSP: aSockets)
    {
        SocketT& socket = socketSP.Vn();
        socket.BreakConnections();
    }
}

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
