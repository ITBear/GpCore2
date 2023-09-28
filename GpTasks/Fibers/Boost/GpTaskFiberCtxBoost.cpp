#include "GpTaskFiberCtxBoost.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)
#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)

#include "GpStackImplPoolBoost.hpp"
#include "../GpTaskFiber.hpp"
#include "../../../GpUtils/Types/Strings/GpStringUtils.hpp"
#include "../../../GpUtils/Threads/Timers/GpTimersManager.hpp"
#include "../../Scheduler/GpTaskScheduler.hpp"
#include "../GpTaskFiber.hpp"

namespace GPlatform {

GpTaskFiberCtxBoost::GpTaskFiberCtxBoost (void) noexcept
try
{
    std::optional<GpStackBoost::StackImplT> stackImpl = GpStackImplPoolBoost::S().Acquire();

    THROW_COND_GP
    (
        stackImpl.has_value(),
        u8"Failed to get fiber stack implementation from pool"_sv
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
    GpStringUtils::SCerr(u8"[GpTaskFiberCtxBoost::GpTaskFiberCtxBoost]: "_sv + ex.what());
    std::terminate();
} catch(...)
{
    GpStringUtils::SCerr(u8"[GpTaskFiberCtxBoost::GpTaskFiberCtxBoost]: unknown exception"_sv);
    std::terminate();
}

GpTaskFiberCtxBoost::~GpTaskFiberCtxBoost (void) noexcept
{   
}

std::optional<GpException>  GpTaskFiberCtxBoost::Clear (void) noexcept
{
    std::optional<GpException> ex;

    try
    {
        iYieldRes   = GpTaskRunRes::DONE;
        iTaskFiber  = nullptr;
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
        ex = GpException(u8"[GpTaskFiberCtxBoost::Clear]: unknown exception"_sv);
        GpStringUtils::SCerr(ex->what());
    }

    return ex;
}

GpTaskRunRes::EnumT GpTaskFiberCtxBoost::Enter (GpTaskFiber& aTaskFiber)
{
    iYieldRes   = GpTaskRunRes::DONE;
    iTaskFiber  = &aTaskFiber;
    iException.reset();

    //--------------- OUTSIDE FIBER --------------
    //--------------- JUMP INTO FIBER ---------------

    iFiber = std::move(iFiber).resume(); // Jump to GpTaskFiberCtxBoost::SFiberFn or back to GpTaskFiberCtxBoost::Yield

    //--------------- EXIT FROM FIBER ---------------
    //--------------- OUTSIDE FIBER --------------

    if (iException.has_value()) [[unlikely]]
    {
        std::rethrow_exception(std::move(iException.value()));
    }

    return iYieldRes;
}

void    GpTaskFiberCtxBoost::Yield (const GpTaskRunRes::EnumT aRunRes)
{
    iYieldRes = aRunRes;

    //--------------- INSIDE FIBER ---------------
    //--------------- EXIT FROM FIBER ---------------

    iFiber = std::move(iFiber).resume(); // Jump back to GpTaskFiberCtxBoost::Enter

    //--------------- JUMP INTO FIBER ---------------
    //--------------- INSIDE FIBER ---------------

    if (iTaskFiber != nullptr) [[likely]]
    {
        if (iTaskFiber->IsStopRequested()) [[unlikely]]
        {
            iTaskFiber->CallStop();

            std::u8string_view taskName = iTaskFiber->Name();

            if (taskName.empty())
            {
                THROW_GP(u8"Task fiber interrupted"_sv);
            } else
            {
                THROW_GP(u8"Task fiber interrupted: '"_sv + taskName + u8"'"_sv);
            }
        }
    }
}

void    GpTaskFiberCtxBoost::Yield (const milliseconds_t aTimeout)
{
    const GpTask::IdT taskId = GpTaskFiber::SCurrentFiber().Id();

    GpTimersManager::SSingleShot
    (
        [taskId](const GpTimer& /*aTimer*/)
        {
            GpTaskScheduler::S().MakeTaskReady(taskId);
        },
        aTimeout
    );

    Yield(GpTaskRunRes::WAIT);
}

boost::context::fiber   GpTaskFiberCtxBoost::SFiberFn
(
    boost::context::fiber&& aFiber,
    GpTaskFiberCtxBoost&    aFiberCtx
)
{
    //--------------- INSIDE FIBER ---------------

    aFiberCtx.iFiber = std::move(aFiber);

    GpTaskFiber* task = aFiberCtx.iTaskFiber;

    if (task != nullptr) [[likely]]
    {
        try
        {
            // First call
            GpTaskRunRes::EnumT res = task->FiberRun();

            // Call until res != GpTaskRunRes::DONE
            while (res != GpTaskRunRes::DONE)
            {
                // Yield for reschedule
                aFiberCtx.Yield(res);

                // Call
                res = task->FiberRun();
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

    //--------------- INSIDE FIBER ---------------
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)
#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
