#include "GpTaskFiberCtxBoost.hpp"

#if defined(GP_USE_MULTITHREADING_FIBERS)
#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)

#include "GpStackImplPoolBoost.hpp"
#include "../GpTaskFiber.hpp"
#include "../../Scheduler/GpTaskScheduler.hpp"
#include "../GpTaskFiber.hpp"
#include "../GpTaskFiberCtxForceUnwind.hpp"

#include <GpCore2/GpUtils/Types/Strings/GpStringUtils.hpp>
#include <GpCore2/GpUtils/Threads/Timers/GpTimersManager.hpp>

namespace GPlatform {

GP_WARNING_PUSH()

#if defined(GP_OS_WINDOWS)
#   pragma warning(disable : 4297)
#endif// #if defined(GP_OS_WINDOWS)

GpTaskFiberCtxBoost::GpTaskFiberCtxBoost (void) noexcept
try
{
    std::optional<GpStackBoost::StackImplT> stackImpl = GpStackImplPoolBoost::S().Acquire();

    THROW_COND_GP
    (
        stackImpl.has_value(),
        "Failed to get fiber stack implementation from pool"_sv
    );

    iStack = std::move(stackImpl.value());

    // Create fiber
    GpTaskFiberCtxBoost* thisPtr = this;
    iFiber = boost::context::fiber
    (
        std::allocator_arg,
        GpStackBoost(iStack),
        [thisPtr](boost::context::fiber&& aFiber)
        {
            return SFiberFn(std::move(aFiber), *thisPtr);
        }
    );
} catch(const std::exception& ex)
{
    GpStringUtils::SCerr("[GpTaskFiberCtxBoost::GpTaskFiberCtxBoost]: "_sv + ex.what());
    std::terminate();
} catch(...)
{
    GpStringUtils::SCerr("[GpTaskFiberCtxBoost::GpTaskFiberCtxBoost]: unknown exception"_sv);
    std::terminate();
}

GP_WARNING_POP()

GpTaskFiberCtxBoost::~GpTaskFiberCtxBoost (void) noexcept
{   
}

GpException::C::Opt GpTaskFiberCtxBoost::Clear (void) noexcept
{
    GpException::C::Opt ex;

    try
    {
        iYieldRes               = GpTaskRunRes::DONE;
        iTaskFiber              = nullptr;
        iIsCallStopInProgress   = false;

        iException.reset();


        if (iFiber)
        {
            iFiber = {};
        }

        if (iStack.IsAllocated()) [[likely]]
        {
            GpStackImplPoolBoost::S().Release(std::move(iStack));
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
        ex = GpException("[GpTaskFiberCtxBoost::Clear]: unknown exception"_sv);
        GpStringUtils::SCerr(ex->what());
    }

    return ex;
}

GpTaskRunRes::EnumT GpTaskFiberCtxBoost::Enter (GpTaskFiber& aTaskFiber)
{
    iYieldRes   = GpTaskRunRes::DONE;
    iTaskFiber  = &aTaskFiber;
    iException.reset();

    // --------------- OUTSIDE FIBER --------------
    // --------------- JUMP INTO FIBER ---------------

    iFiber = std::move(iFiber).resume(); // Jump to GpTaskFiberCtxBoost::SFiberFn or back to GpTaskFiberCtxBoost::Yield

    // --------------- EXIT FROM FIBER ---------------
    // --------------- OUTSIDE FIBER --------------

    if (iException.has_value()) [[unlikely]]
    {
        std::rethrow_exception(std::move(iException.value()));
    }

    return iYieldRes;
}

void    GpTaskFiberCtxBoost::CallYield (const GpTaskRunRes::EnumT aRunRes)
{
    iYieldRes = aRunRes;

    // --------------- INSIDE FIBER ---------------
    // --------------- EXIT FROM FIBER ---------------

    iFiber = std::move(iFiber).resume(); // Jump back to GpTaskFiberCtxBoost::Enter

    // --------------- JUMP INTO FIBER ---------------
    // --------------- INSIDE FIBER ---------------

    if (iTaskFiber != nullptr) [[likely]]
    {
        if (iIsCallStopInProgress == false)
        {
            if (iTaskFiber->IsStopRequested()) [[unlikely]]
            {
                iIsCallStopInProgress = true;
                iTaskFiber->CallOnStop(GpMethodAccess<GpTaskFiberCtx>{this});
                throw GpTaskFiberCtxForceUnwind(iTaskFiber->TaskName());
            }
        }
    }
}

void    GpTaskFiberCtxBoost::CallYield (const milliseconds_t aTimeout)
{
    const GpTaskId taskId = GpTaskFiber::SCurrentFiber().TaskId();

    const bool isSuccess = GpTimersManager::SSingleShot
    (
        [taskId]([[maybe_unused]] const GpTimer& aTimer)
        {
            GpTaskScheduler::S().MakeTaskReady(taskId);
        },
        aTimeout
    );

    if (isSuccess) [[likely]]
    {
        CallYield(GpTaskRunRes::WAIT);
    } else
    {
        CallYield(GpTaskRunRes::READY_TO_RUN);
    }
}

boost::context::fiber   GpTaskFiberCtxBoost::SFiberFn
(
    boost::context::fiber&& aFiber,
    GpTaskFiberCtxBoost&    aFiberCtx
)
{
    // --------------- INSIDE FIBER ---------------

    aFiberCtx.iFiber = std::move(aFiber);

    GpTaskFiber* task = aFiberCtx.iTaskFiber;

    if (task != nullptr) [[likely]]
    {
        try
        {
            // First call
            GpTaskRunRes::EnumT res = task->FiberRun(GpMethodAccess<GpTaskFiberCtx>{&aFiberCtx});

            // Call until res != GpTaskRunRes::DONE
            while (res != GpTaskRunRes::DONE)
            {
                // Yield for reschedule
                aFiberCtx.CallYield(res);

                // Call
                res = task->FiberRun(GpMethodAccess<GpTaskFiberCtx>{&aFiberCtx});
            }

            aFiberCtx.iYieldRes = GpTaskRunRes::DONE;
        } catch (const boost::context::detail::forced_unwind&)
        {
            aFiberCtx.iYieldRes = GpTaskRunRes::DONE;
            throw;//rethrow
        } catch (...)
        {
            aFiberCtx.iYieldRes     = GpTaskRunRes::DONE;
            aFiberCtx.iException    = std::current_exception();// Save curren exception
        }
    }

    // Terminate fiber and exit
    return std::move(aFiberCtx.iFiber); // Jump back to GpTaskFiberCtxBoost::Enter

    // --------------- INSIDE FIBER ---------------
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)
#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
