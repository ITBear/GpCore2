#include "GpPipelineEvaluator.hpp"

#if defined(GP_USE_PIPELINE)

namespace GPlatform {

GpPipelineEvaluator::GpPipelineEvaluator (GpPipeline::CSP aPipeline) noexcept:
iPipeline(std::move(aPipeline))
{
}

GpPipelineEvaluator::~GpPipelineEvaluator (void) noexcept
{
}

void    GpPipelineEvaluator::FiberFn (GpThreadStopToken aStopToken)
{
    const GpPipeline&           pipeline        = iPipeline.VC();
    GpPipelineNode::C::Set::CSP readyNodes      = pipeline.RootNodes();
    GpPipelineNode::C::Set::CSP evaluatingNodes;
    GpPipelineNode::C::Set::CSP nextNodes;

    while (!aStopToken.stop_requested())
    {
        ProcessEvents(readyNodes);

        if (readyNodes.size() > 0)
        {
            for (const GpPipelineNode::CSP& n: readyNodes)
            {
                GpPipelineEvaluateCtx::SP evaluateCtx = n.VCn().NewEvaluateCtx();
                //GpTaskFiber::S
            }

            readyNodes.clear();
            GpTaskFiber::SYield(GpTask::ResT::WAITING);
        } else
        {
            //if (nextNodes.size() > )

        }
    }

    //GpTaskFiber::SYield(GpTask::Res::DONE);
}

void    GpPipelineEvaluator::ProcessEvents (GpPipelineNode::C::Set::CSP& aReadyNodesOut)
{
    GpEvent::SP event = PopNextEvent();

    while (event.IsNotNULL())
    {
        //?
        aReadyNodesOut.emplace();

        event = PopNextEvent();
    }
}

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)
