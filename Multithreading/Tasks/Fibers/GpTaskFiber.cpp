#include "GpTaskFiber.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../Exceptions/GpExceptionsSink.hpp"
#include "GpTaskFiberStopEx.hpp"
#include "boost_context.hpp"
#include <iostream>

namespace GPlatform {

GpTaskFiber::GpTaskFiber (void) noexcept
{
}

GpTaskFiber::~GpTaskFiber (void) noexcept
{
}

GpTask::ResT    GpTaskFiber::Do (GpThreadStopToken aStopToken) noexcept
{
    ResT taskRes = ResT::DONE;

    try
    {
        switch (iStage.Value())
        {
            case StageT::NOT_RUN:
            {
                iCtx = MakeSP<GpTaskFiberCtx>();
                iCtx.Vn().Init();
                iStage = StageT::RUN;
            } [[fallthrough]];
            case StageT::RUN:
            {
                taskRes = iCtx.Vn().Enter(aStopToken,
                                          GetWeakPtr(),
                                          std::bind(&GpTaskFiber::FiberFn, this, std::placeholders::_1));
            } break;
            case StageT::FINISHED: [[fallthrough]];
            default:
            {
                //NOP
            } break;
        }
    } /*catch (const boost::context::detail::forced_unwind& e)
    {
        iCtx.Clear();
        iStage = StageT::FINISHED;
        GpExceptionsSink::SSink(e);
        throw;
    }*/ catch (const GpTaskFiberStopEx&)
    {
        iCtx.Clear();
        iStage = StageT::FINISHED;
    } catch (const std::exception& e)
    {
        iCtx.Clear();
        iStage = StageT::FINISHED;
        GpExceptionsSink::SSink(e);
    } catch (...)
    {
        iCtx.Clear();
        iStage = StageT::FINISHED;
        GpExceptionsSink::SSinkUnknown();
    }

    if (taskRes == ResT::DONE)
    {
        iCtx.Clear();
        iStage = StageT::FINISHED;      
    }

    return taskRes;
}

void    GpTaskFiber::Terminate (void) noexcept
{
    if (iStage != StageT::RUN)
    {
        return;
    }

    GpThreadStopSource  stopSource;
    GpThreadStopToken   stopToken = stopSource.get_token();
    stopSource.request_stop();

    Do(stopToken);
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
