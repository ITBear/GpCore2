#include "GpTaskFiberCtx.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

//#include <iostream>

#include "../../../Exceptions/GpExceptionsSink.hpp"
#include "GpTaskFiberManager.hpp"
#include "GpTaskFiber.hpp"
#include "GpTaskFiberStopEx.hpp"
#include "../../../Utils/RAII/GpRAIIonDestruct.hpp"
#include "../../../Types/Containers/GpElementsCatalog.hpp"
#include "../../../Types/Strings/GpStringOps.hpp"
#include "GpTaskFiber.hpp"

namespace GPlatform {

//static std::atomic_int64_t sGpTaskFiberManager_count = 0;

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

        SSetCurrentCtx(std::nullopt);
        Clear();

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
        iFiberOuter.reset();
    }

    if (iFiberStack.IsNotNULL())
    {
        //sGpTaskFiberManager_count--;

        GpTaskFiberManager::S().StackPool().Release(std::move(iFiberStack));

        //std::cout << "[GpTaskFiberCtx::Clear]: sGpTaskFiberManager_count = "_sv << sGpTaskFiberManager_count << std::endl;
    }
}

void    GpTaskFiberCtx::InitIfNot (void)
{
    if (iFiberOuter)
    {
        return;
    }

    //Get stack from pool
    //sGpTaskFiberManager_count++;

    auto res = GpTaskFiberManager::S().StackPool().Acquire();

    //std::cout << "[GpTaskFiberCtx::Clear]: sGpTaskFiberManager_count = "_sv << sGpTaskFiberManager_count << std::endl;

    THROW_GPE_COND
    (
        res.has_value(),
        "Failed to get fiber stack from pool"_sv
    );

    iFiberStack = std::move(res.value());

    //Create fiber
    GpFiberStackT& fiberStack = *reinterpret_cast<GpFiberStackT*>(iFiberStack.Vn().Stack());

    iFiberOuter = std::make_unique<FiberT>
    (
        std::allocator_arg,
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

void    GpTaskFiberCtx::SClearCurrentCtx (void) noexcept
{
    SSetCurrentCtx(std::nullopt);
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
        throw;
    } catch (...)
    {
        iYieldRes = GpTask::ResT::DONE;
        iException = std::current_exception();
    }

    return std::move(iFiberInner);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
