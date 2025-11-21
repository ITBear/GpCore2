#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)

#include <GpCore2/GpUtils/Debugging/GpStackTrace.hpp>

GP_WARNING_PUSH()

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(shadow)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#include <boost/context/fiber.hpp>

GP_WARNING_POP()

#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtxFactory.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtxForceUnwind.hpp>

namespace GPlatform {

GpTaskFiber::GpTaskFiber (void) noexcept:
GpTask{GpTaskMode::FIBER}
{
}

GpTaskFiber::GpTaskFiber (std::string aName) noexcept:
GpTask{std::move(aName), GpTaskMode::FIBER}
{
}

GpTaskFiber::~GpTaskFiber (void) noexcept
{
    if (iCtxUP == nullptr) [[likely]]
    {
        return;
    }

    GpOutUtils::S().Err
    (
        fmt::format
        (
            "[GpTaskFiber::~GpTaskFiber]: iCtxUP is not null!. Task '{}', '{}'",
            TaskName(),
            TaskId().Value()
        )
    );

    // Yes, app will crash
    std::terminate();
}

GpTaskFiber::SP GpTaskFiber::SCurrentFiber (void)
{
    GpTask::WP taskWP = GpTask::SCurrentTask();
    GpTask::SP taskSP = taskWP.Lock();

    VERIFY
    (
        taskSP.IsNotNULL(),
        "[GpTaskFiber::SCurrentFiber]: Function invoked outside a fiber context (current task is null)"
    );

    VERIFY
    (
        taskSP.Vn().TaskMode() == GpTaskMode::FIBER,
        "[GpTaskFiber::SCurrentFiber]: Function invoked outside a fiber context (task mode == GpTaskMode::THREAD)"
    );

    return taskSP.CastTo<GpTaskFiber::SP>();
}

void    GpTaskFiber::SYield (const GpTaskRunRes::EnumT aValue)
{
    SCurrentFiber().Vn().iCtxUP->CallYield(aValue);
}

GpTaskFiberCtx::TimeoutRes  GpTaskFiber::SYield (const milliseconds_t aTimeout)
{
    return SCurrentFiber().Vn().iCtxUP->CallYield(aTimeout);
}

GpTaskRunRes::EnumT GpTaskFiber::Run (void) noexcept
{
    GpTaskRunRes::EnumT res = GpTaskRunRes::DONE;
    GpException::C::Opt ex;

    // Catch all exceptions
    try
    {
        // Check if fiber context is created
        if (iCtxUP == nullptr) [[unlikely]]
        {
            iCtxUP = GpTaskFiberCtxFactory::S().NewInstance();
        }

        // Enter to fiber (jump to GpTaskFiberCtxBoost::SFiberFn)
        res = iCtxUP->Enter(*this);
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
    try
    {
        if (ex.has_value())
        {
            res = GpTaskRunRes::DONE;

            GpException::C::Opt clearExOpt = ClearCtx();

            if (clearExOpt.has_value())
            {
                StartPromise(GpMethodAccess{this}).Fulfill(clearExOpt.value());
                DonePromise(GpMethodAccess{this}).Fulfill(clearExOpt.value());
            } else
            {
                StartPromise(GpMethodAccess{this}).Fulfill(ex.value());
                DonePromise(GpMethodAccess{this}).Fulfill(ex.value());
            }
        } else if (res == GpTaskRunRes::DONE) // Check if result is DONE
        {
            GpException::C::Opt clearExOpt = ClearCtx();

            if (clearExOpt.has_value())
            {
                StartPromise(GpMethodAccess{this}).Fulfill(clearExOpt.value());
                DonePromise(GpMethodAccess{this}).Fulfill(clearExOpt.value());
            } else
            {
                StartPromise(GpMethodAccess{this}).Fulfill(StartPromiseRes{});
                DonePromise(GpMethodAccess{this}).Fulfill(DonePromiseRes{});
            }
        }
    } catch (const GpException& e)
    {
        GpOutUtils::S().StdErr("[GpTaskFiber::Run]: exception: "_sv + e.what());
    } catch (const std::exception& e)
    {
        GpOutUtils::S().StdErr("[GpTaskFiber::Run]: exception: "_sv + e.what());
    } catch (...)
    {
        GpOutUtils::S().StdErr("[GpTaskFiber::Run]: unknown exception: "_sv);
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
            StartPromise(GpMethodAccess{this}).Fulfill(StartPromiseRes{});
        }

        // Do Step
        const GpTaskRunRes::EnumT res = OnStep();

        if (res != GpTaskRunRes::DONE) [[likely]]
        {
            return res;
        }
    } catch (const GpTaskFiberCtxForceUnwind&)
    {
        // NOP
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

    ExceptionsT stopExceptions;
    if (ex.has_value())
    {
        stopExceptions.emplace_back(std::move(ex.value()));
    }

    // --------------- Call stop ------------------
    CallOnStop(stopExceptions);

    for (const GpException& exeption: stopExceptions)
    {
        OnStopException(exeption);
    }

    if (!stopExceptions.empty())
    {
        throw stopExceptions[0];
    }

    return GpTaskRunRes::DONE;
}

void    GpTaskFiber::CallOnStop (GpMethodAccessGuard<GpTaskFiberCtx>) noexcept
{
    GpTaskFiber::ExceptionsT stopExceptions;
    CallOnStop(stopExceptions);

    for (const GpException& exeption: stopExceptions)
    {
        OnStopException(exeption);
    }
}

void    GpTaskFiber::CallOnStop (ExceptionsT& aStopExceptionsOut) noexcept
{
    if (iIsStopCalled)
    {
        return;
    }

    iIsStopCalled = true;
    OnStop(aStopExceptionsOut);
}

GpException::C::Opt GpTaskFiber::ClearCtx (void) noexcept
{
    GpException::C::Opt ex;

    try
    {
        if (iCtxUP != nullptr)
        {
            ex = iCtxUP->Clear();
            iCtxUP.reset();
        }
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
