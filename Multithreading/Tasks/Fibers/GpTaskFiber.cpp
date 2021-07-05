#include "GpTaskFiber.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../Exceptions/GpExceptionsSink.hpp"
#include "GpTaskFiberStopEx.hpp"
#include "GpTaskFiberCtx.hpp"

#include <iostream>

namespace GPlatform {

static int _GpTaskFiber_counter = 0;

GpTaskFiber::GpTaskFiber (std::string_view aName):
GpTask(aName)
{
    _GpTaskFiber_counter++;
    std::cout << "[GpTaskFiber::GpTaskFiber]: '" << Name() << "' counter = " << _GpTaskFiber_counter << std::endl;
}

GpTaskFiber::~GpTaskFiber (void) noexcept
{
    _GpTaskFiber_counter--;
    std::cout << "[GpTaskFiber::~GpTaskFiber]: '" << Name() << "' counter = " << _GpTaskFiber_counter << std::endl;
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
                iCtx    = MakeSP<GpTaskFiberCtx>(GetWeakPtr());
                iStage  = StageT::RUN;
            } [[fallthrough]];
            case StageT::RUN:
            {
                taskRes = iCtx.Vn().Enter(aStopToken);
            } break;
            case StageT::FINISHED: [[fallthrough]];
            default:
            {
                //NOP
            } break;
        }
    } catch (const boost::context::detail::forced_unwind& e)
    {
        iCtx.Clear();
        iStage = StageT::FINISHED;
        GpExceptionsSink::SSink("boost::context::detail::forced_unwind"_sv);
        //throw;
    } catch (const GpTaskFiberStopEx&)
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

    ClearEventsQueue();

    Do(stopToken);
}

void    GpTaskFiber::SYield (const GpTask::ResT aRes)
{
    auto ctx = GpTaskFiberCtx::SCurrentCtx();
    THROW_GPE_COND(ctx.has_value(), "Call outside of fiber"_sv);

    ctx.value().get().Yield(aRes);
}

GpWP<GpTaskFiber>   GpTaskFiber::SCurrentTask (void)
{
    auto ctx = GpTaskFiberCtx::SCurrentCtx();
    THROW_GPE_COND(ctx.has_value(), "Call outside of fiber"_sv);

    return ctx.value().get().Task();
}

bool    GpTaskFiber::SIsIntoFiber (void) noexcept
{
    auto ctx = GpTaskFiberCtx::SCurrentCtx();
    return ctx.has_value();
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
