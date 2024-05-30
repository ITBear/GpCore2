#include "GpTaskFiber.hpp"

#if defined(GP_USE_MULTITHREADING_FIBERS)

#include <iostream>
#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>

GP_WARNING_PUSH()

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(shadow)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#   include <boost/context/fiber.hpp>

GP_WARNING_POP()

#include "GpTaskFiberCtxFactory.hpp"
#include "GpTaskFiberCtxForceUnwind.hpp"

namespace GPlatform {

GpTaskFiber::~GpTaskFiber (void) noexcept
{
    if (iCtx.IsNULL()) [[likely]]
    {
        return;
    }

#if defined(DEBUG_BUILD)
    GpDebugging::SBreakpoint();
#endif// #if defined(DEBUG_BUILD)

    GpStringUtils::SCerr
    (
        fmt::format
        (
            "[GpTaskFiber::~GpTaskFiber]: iCtx is not null!. Task '{}'",
            TaskName()
        )
    );

    // Yes, app will crash
    std::terminate();
}

GpTaskFiber&    GpTaskFiber::SCurrentFiber (void)
{
    GpTask::C::Opt::Ref currentTaskOpt = GpTask::SCurrentTask();

    if (!currentTaskOpt.has_value()) [[unlikely]]
    {
        THROW_GP("Call Yeld from outside fiber"_sv);
    }

    GpTask& currentTask = currentTaskOpt.value();

    if (currentTask.TaskMode() != GpTaskMode::FIBER) [[unlikely]]
    {
        THROW_GP("Call Yeld from not fiber task"_sv);
    }

    return static_cast<GpTaskFiber&>(currentTask);
}

GpTaskRunRes::EnumT GpTaskFiber::Run (void) noexcept
{
    GpTaskRunRes::EnumT res = GpTaskRunRes::DONE;
    GpException::C::Opt ex;

    // Catch all exceptions
    try
    {
        // Check if fiber context is created
        if (iCtx.IsNULL()) [[unlikely]]
        {
            // TODO: add ctx pool (cache)
            iCtx = GpTaskFiberCtxFactory::S().NewInstance();
        }

        // Enter to fiber
        res = iCtx->Enter(*this);
    } catch (const GpTaskFiberCtxForceUnwind&)
    {
        // NOP
    } catch (const GpException& e)
    {
        ex = e;
    } catch (const std::exception& e)
    {
        ex = GpException(e.what());
    } catch (...)
    {
        ex = GpException("[GpTaskFiber::Run]: unknown exception"_sv);
    }

    // Check if there are was exception
    if (ex.has_value())
    {
        GpStringUtils::SCerr(ex->what());
        res = GpTaskRunRes::DONE;

        GpException::C::Opt clearExOpt = ClearCtx();

        if (clearExOpt.has_value())
        {
            StartPromise().Fulfill(clearExOpt.value());
            DonePromise().Fulfill(clearExOpt.value());
        } else
        {
            StartPromise().Fulfill(ex.value());
            DonePromise().Fulfill(ex.value());
        }
    } else if (res == GpTaskRunRes::DONE) // Check if result is DONE
    {
        GpException::C::Opt clearExOpt = ClearCtx();

        if (clearExOpt.has_value())
        {
            StartPromise().Fulfill(clearExOpt.value());
            DonePromise().Fulfill(clearExOpt.value());
        } else
        {
            StartPromise().Fulfill(GpAny{});
            DonePromise().Fulfill(GpAny{});
        }
    }

    return res;
}

GpTaskRunRes::EnumT GpTaskFiber::FiberRun (GpMethodAccessGuard<GpTaskFiberCtx>)
{
    try
    {       
        // Check if OnStart was called
        if (!iIsStartCalled) [[unlikely]]
        {
            OnStart();

            iIsStartCalled = true;
            StartPromise().Fulfill(GpAny{});
        }

        // Do Step
        const GpTaskRunRes::EnumT res = OnStep();

        if (res == GpTaskRunRes::DONE) [[unlikely]]
        {           
            CallOnStop(GpMethodAccess<GpTaskFiber>{this});
        }

        return res;
    } catch (...)
    {
        CallOnStop(GpMethodAccess<GpTaskFiber>{this});
        throw;      
    }

    // Will never reach here
    return GpTaskRunRes::DONE;
}

void    GpTaskFiber::CallOnStop (GpMethodAccessGuard<GpTaskFiber, GpTaskFiberCtx>)
{
    if (iIsStopCalled)
    {
        return;
    }

    GpException::C::Opt stopExOpt = OnStop();
    if (stopExOpt.has_value())
    {
        throw stopExOpt.value();
    }

    iIsStopCalled = true;
}

GpException::C::Opt GpTaskFiber::ClearCtx (void) noexcept
{
    GpException::C::Opt ex;

    try
    {
        if (iCtx.IsNotNULL())
        {
            ex = iCtx->Clear();         
            iCtx.Clear();
        }

        return std::nullopt;
    } catch (const GpException& e)
    {
        ex = e;
        GpStringUtils::SCerr(ex->what());
    } catch (const std::exception& e)
    {
        ex = GpException(e.what());
        GpStringUtils::SCerr(ex->what());
    } catch (...)
    {
        ex = GpException("[GpTaskFiber::ClearCtx]: unknown exception"_sv);
        GpStringUtils::SCerr(ex->what());
    }

    return ex;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
