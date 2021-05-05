#include "GpTaskFiberCtx.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../Exceptions/GpExceptionsSink.hpp"
#include "GpTaskFiberManager.hpp"
#include "GpTaskFiber.hpp"
#include "GpTaskFiberStopEx.hpp"
#include "../../../Utils/RAII/GpRAIIonDestruct.hpp"
#include "../../../Types/Containers/GpElementsCatalog.hpp"
#include "../../../Types/Strings/GpStringOps.hpp"
#include "GpTaskFiber.hpp"

#include <iostream>

namespace GPlatform {

//static thread_local FiberArgs sFiberArgsTLS;
GpElementsCatalog<std::thread::id, GpTaskFiberCtx::C::Opt::Ref> GpTaskFiberCtx::sFiberArgs;

GpTaskFiberCtx::GpTaskFiberCtx (GpWP<GpTaskFiber> aTask) noexcept:
iTask(std::move(aTask))
{
}

GpTaskFiberCtx::~GpTaskFiberCtx (void) noexcept
{
    Clear();
}

GpTask::ResT    GpTaskFiberCtx::Enter (GpThreadStopToken aStopToken)
{
    InitIfNot();
    SSetCurrentCtx(*this);

    iStopToken  = std::move(aStopToken);
    iYieldRes   = GpTask::ResT::DONE;

    try
    {
        *iFiberOuter.get() = std::move(*iFiberOuter.get()).resume();

        iStopToken = GpThreadStopToken();

        if (iException.has_value())
        {
            auto ex = std::move(iException.value());
            iException.reset();
            iYieldRes = GpTask::ResT::DONE;

            SSetCurrentCtx(std::nullopt);

            std::rethrow_exception(ex);
        }
    } catch (...)
    {
        iStopToken  = GpThreadStopToken();
        iYieldRes   = GpTask::ResT::DONE;
        iException.reset();

        throw;
    }

    return iYieldRes;
}

void    GpTaskFiberCtx::Yield (const GpTask::ResT aRes)
{
    iYieldRes = aRes;
    iFiberInner = std::move(iFiberInner).resume();
}

void    GpTaskFiberCtx::Clear (void) noexcept
{
    if (iFiberOuter)
    {
        //iForceUnwind = true;

        //try
        //{
        //  Enter(iStopToken);
            iFiberOuter.reset();
        //} catch (...)
        //{
        //  int d = 0;
        //}
    }

    if (iFiberStack.IsNotNULL())
    {
        GpTaskFiberManager::S().StackPool().Release(std::move(iFiberStack));
    }
}

void    GpTaskFiberCtx::InitIfNot (void)
{
    if (iFiberOuter)
    {
        return;
    }

    //Get stack from pool
    const auto res = GpTaskFiberManager::S().StackPool().Acquire();
    THROW_GPE_COND
    (
        res.has_value(),
        "Failed to get fiber stack from pool"_sv
    );

    iFiberStack = res.value();

    //Create fiber
    GpFiberStackT& fiberStack = *reinterpret_cast<GpFiberStackT*>(iFiberStack.Vn().Stack());
    //boost::context::stack_context stackCtx = fiberStack.NewCtx();

    iFiberOuter = std::make_unique<FiberT>
    (
        std::allocator_arg,
        //FiberPreallocatedT(stackCtx.sp, stackCtx.size, stackCtx),
        GpFiberStackWrapperT(fiberStack),
        SFiberFn
    );
}

GpTaskFiberCtx::C::Opt::Ref GpTaskFiberCtx::SCurrentCtx (void) noexcept
{
    const auto threadId = std::this_thread::get_id();
    auto res = sFiberArgs.TryFind(threadId);

    if (res.has_value())
    {
        return res.value().get();
    } else
    {
        return std::nullopt;
    }
}

void    GpTaskFiberCtx::SSetCurrentCtx (GpTaskFiberCtx::C::Opt::Ref aCtx) noexcept
{
    const auto threadId = std::this_thread::get_id();
    auto& res = sFiberArgs.FindOrRegister(threadId, [](){return std::nullopt;});

    res = aCtx;
}

FiberT  GpTaskFiberCtx::SFiberFn (FiberT&& aFiber)
{
    const auto threadId = std::this_thread::get_id();
    auto res = sFiberArgs.TryFind(threadId);

    return res.value().get().value().get().FiberFn(std::move(aFiber));
}

FiberT  GpTaskFiberCtx::FiberFn (FiberT&& aFiber)
{
    iFiberInner = std::move(aFiber);

    try
    {
        iYieldRes = GpTask::ResT::DONE;
        iTask->FiberFn(iStopToken);
    } catch (const boost::context::detail::forced_unwind&)
    {
        iYieldRes = GpTask::ResT::DONE;
        std::cout << "[GpTaskFiberCtx::FiberFn]: boost::context::detail::forced_unwind"_sv << std::endl;
        throw;
    } catch (...)
    {
        iYieldRes = GpTask::ResT::DONE;
        iException = std::current_exception();
        std::cout << "[GpTaskFiberCtx::FiberFn]: forward exception"_sv << std::endl;
    }

    return std::move(iFiberInner);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
