#include "GpTaskFiber.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberStopEx.hpp"
#include "GpTaskFiberCtx.hpp"

#include <iostream>

namespace GPlatform {

GpTaskFiber::GpTaskFiber (std::string aName) noexcept:
GpTask
(
    std::move(aName),
    GpTaskType::FIBER
)
{
}

GpTaskFiber::~GpTaskFiber (void) noexcept
{
    if (iCtx)
    {
        GpStringUtils::SCerr("[GpTaskFiber::~GpTaskFiber]: iCtx is not null!\n"_sv);
        //Yes, app will crash
        throw std::runtime_error("[GpTaskFiber::~GpTaskFiber]: iCtx is not null!");
    }
}

GpTaskDoRes GpTaskFiber::_Run (GpThreadStopToken aStopToken) noexcept
{
    GpTaskDoRes taskRes = GpTaskDoRes::DONE;

    try
    {
        if (!iCtx)
        {
            iCtx = std::make_unique<GpTaskFiberCtx>(*this);
        }

        switch (iStage)
        {
            case StageT::NOT_RUN:
            {
                iStage  = StageT::RUN;
            } [[fallthrough]];
            case StageT::RUN:
            {
                taskRes = iCtx->Enter(aStopToken);
            } break;
            case StageT::FINISHED: [[fallthrough]];
            default:
            {
                //NOP
            } break;
        }
    } catch (const boost::context::detail::forced_unwind& e)
    {
        iStage = StageT::FINISHED;
        CompletePromise(MakeSP<GpItcResult>(std::runtime_error("boost::context::detail::forced_unwind")));
    } catch (const GpTaskFiberStopEx& e)
    {
        iStage = StageT::FINISHED;
        CompletePromise(MakeSP<GpItcResult>(e));
    } catch (const GpException& e)
    {
        iStage = StageT::FINISHED;
        CompletePromise(MakeSP<GpItcResult>(e));
    } catch (const std::exception& e)
    {
        iStage = StageT::FINISHED;
        CompletePromise(MakeSP<GpItcResult>(e));
    } catch (...)
    {
        iStage = StageT::FINISHED;
        CompletePromise(MakeSP<GpItcResult>(std::runtime_error("Unknown exception")));
    }

    if (taskRes == GpTaskDoRes::DONE)
    {
        iStage = StageT::FINISHED;
        CompletePromise(MakeSP<GpItcResult>());
    }

    if (iStage == StageT::FINISHED)
    {
        ClearCtx();
    }

    return taskRes;
}

void    GpTaskFiber::ClearCtx (void) noexcept
{
    try
    {
        if (iCtx)
        {
            iCtx->Clear();
            iCtx.reset();
        }
    } catch (const boost::context::detail::forced_unwind& e)
    {
        GpStringUtils::SCerr("[GpTaskFiber::_Run]: boost::context::detail::forced_unwind\n"_sv);
    } catch (const GpTaskFiberStopEx& e)
    {
        GpStringUtils::SCerr("[GpTaskFiber::_Run]: "_sv + e.what() + "\n"_sv);
    } catch (const GpException& e)
    {
        GpStringUtils::SCerr("[GpTaskFiber::_Run]: "_sv + e.what() + "\n"_sv);
    } catch (const std::exception& e)
    {
        GpStringUtils::SCerr("[GpTaskFiber::_Run]: "_sv + e.what() + "\n"_sv);
    } catch (...)
    {
        GpStringUtils::SCerr("[GpTaskFiber::_Run]: Unknown exception\n"_sv);
    }
}

void    GpTaskFiber::SYield (const GpTaskDoRes aRes)
{
    SCtx()->Yield(aRes);
}

bool    GpTaskFiber::SIsIntoFiber (void) noexcept
{
    GpTask* currentTask = GpTask::SCurrent();

    return    (currentTask != nullptr)
           && (currentTask->TaskType() == GpTaskType::FIBER);
}

GpTaskFiberCtx* GpTaskFiber::SCtx (void)
{
    GpTask* currentTask = GpTask::SCurrent();

    THROW_COND_GP
    (
        currentTask != nullptr,
        "Current task is nullptr"_sv
    );

    THROW_COND_GP
    (
        currentTask->TaskType() == GpTaskType::FIBER,
        "Current task is not FIBER"_sv
    );

    GpTaskFiber*    currentTaskFiber    = static_cast<GpTaskFiber*>(currentTask);
    GpTaskFiberCtx* fiberCtx            = currentTaskFiber->Ctx();

    THROW_COND_GP
    (
        fiberCtx != nullptr,
        "Current task fiber ctx is nullptr"_sv
    );

    return fiberCtx;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
