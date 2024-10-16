#include <GpCore2/GpTasks/Fibers/Boost/GpTaskFiberCtxBoost.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)
#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)

#include <GpCore2/GpTasks/Fibers/Boost/GpStackImplPoolBoost.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <GpCore2/GpTasks/Scheduler/GpTaskScheduler.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiber.hpp>
#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtxForceUnwind.hpp>
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
    } catch (const std::exception& e)
    {
        ex = GpException{e.what()};
    } catch (...)
    {
        ex = GpException{"[GpTaskFiberCtxBoost::Clear]: unknown exception"_sv};
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
        if (iIsCallStopInProgress == false) [[likely]]
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

GpTaskFiberCtx::TimeoutRes  GpTaskFiberCtxBoost::CallYield (const milliseconds_t aTimeout)
{
    const GpTaskId taskId = GpTaskFiber::SCurrentFiber().TaskId();

    GpTimer::SP waitingTimerSP = GpTimersManager::SSingleShot
    (
        [taskId]([[maybe_unused]] const GpTimer& aTimer)
        {
            GpTaskScheduler::S().MakeTaskReady(taskId);
        },
        aTimeout,
        false
    );

    CallYield(GpTaskRunRes::WAIT);

    // Turn off the timer
    return waitingTimerSP->Stop() ? TimeoutRes::WAKEUP_BY_TIMEOUT : TimeoutRes::WAKEUP_BY_OTHER_EVENT;
}

boost::context::fiber   GpTaskFiberCtxBoost::SFiberFn
(
    boost::context::fiber&& aFiber,
    GpTaskFiberCtxBoost&    aFiberCtxBoost
)
{
    // --------------- INSIDE FIBER ---------------

    aFiberCtxBoost.iFiber = std::move(aFiber);

    GpTaskFiber* task = aFiberCtxBoost.iTaskFiber;

    if (   (task != nullptr)
        && (task->TaskState() != GpTaskState::DONE)) [[likely]]
    {
        try
        {
            // First call
            GpTaskRunRes::EnumT res = task->FiberRun(GpMethodAccess<GpTaskFiberCtx>{&aFiberCtxBoost});

            // Call until res != GpTaskRunRes::DONE
            while (res != GpTaskRunRes::DONE)
            {
                // Yield for reschedule
                aFiberCtxBoost.CallYield(res);

                // Call
                res = task->FiberRun(GpMethodAccess<GpTaskFiberCtx>{&aFiberCtxBoost});
            }

            aFiberCtxBoost.iYieldRes = GpTaskRunRes::DONE;
        } catch (...)
        {
            aFiberCtxBoost.iYieldRes    = GpTaskRunRes::DONE;
            aFiberCtxBoost.iException   = std::current_exception();// Save curren exception
        }
    }

    // Terminate fiber and exit
    return std::move(aFiberCtxBoost.iFiber); // Jump back to GpTaskFiberCtxBoost::Enter

    // --------------- INSIDE FIBER ---------------
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)
#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
