#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <GpService/GpServiceMainTask.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)

#include <GpCore2/GpUtils/Debugging/GpDebugging.hpp>

GP_WARNING_PUSH()

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(shadow)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#   include <boost/context/fiber.hpp>

GP_WARNING_POP()

#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtxFactory.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtxForceUnwind.hpp>

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
        ex = GpException{e.what()};
    } catch (...)
    {
        ex = GpException{"[GpTaskFiber::Run]: unknown exception"_sv};
    }

    // Check if there are was exception
    if (ex.has_value())
    {
        //GpStringUtils::SCerr(ex->what());
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
            StartPromise().Fulfill(StartPromiseRes{});
            DonePromise().Fulfill(DonePromiseRes{});
        }
    }

    return res;
}

GpTaskRunRes::EnumT GpTaskFiber::FiberRun (GpMethodAccessGuard<GpTaskFiberCtx>)
{
    GpException::C::Opt ex;

    // --------------- Call start, do step ------------------
    try
    {
        // Call start
        if (!iIsStartCalled) [[unlikely]]
        {
            OnStart();

            iIsStartCalled = true;
            StartPromise().Fulfill(StartPromiseRes{});
        }

        // Do Step
        const GpTaskRunRes::EnumT res = OnStep();

        if (res != GpTaskRunRes::DONE) [[likely]]
        {
            return res;
        }
    } catch (const GpException& e)
    {
        ex = GpException{e};
    } catch (const std::exception& e)
    {
        ex = GpException{e.what()};
    } catch (...)
    {
        ex = GpException{"[GpTaskFiber::FiberRun]: unknown exception"_sv};
    }

    // --------------- Call stop ------------------
    CallOnStop(GpMethodAccess<GpTaskFiber>{this});

    if (ex.has_value())
    {
        OnStopException(ex.value());
        throw ex.value();
    }

    return GpTaskRunRes::DONE;
}

void    GpTaskFiber::CallOnStop (GpMethodAccessGuard<GpTaskFiber, GpTaskFiberCtx>) noexcept
{
    if (iIsStopCalled)
    {
        return;
    }

    iIsStopCalled = true;
    GpTaskFiber::StopExceptionsT stopExceptions;
    OnStop(stopExceptions);

    for (const GpException& ex: stopExceptions)
    {
        OnStopException(ex);
    }
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
    } catch (const std::exception& e)
    {
        ex = GpException{e.what()};
    } catch (...)
    {
        ex = GpException{"[GpTaskFiber::ClearCtx]: unknown exception"_sv};
    }

    return ex;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
