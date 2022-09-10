#include "GpTaskFiberCtx.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiber.hpp"
#include "GpTaskFiberAccessor.hpp"
#include "GpTaskFiberManager.hpp"

namespace GPlatform {

void    GpTaskFiberCtx::Clear (void) noexcept
{
    iStopToken  = GpThreadStopToken();
    iYieldRes   = GpTaskDoRes::DONE;
    iException.reset();

    if (iFiberOuter)
    {
        iFiberOuter.reset();
    }

    if (iFiberStack.IsNotNULL())
    {
        GpTaskFiberManager::S().Release(std::move(iFiberStack));
    }
}

GpTaskDoRes GpTaskFiberCtx::Enter (GpThreadStopToken aStopToken)
{
    if (!iFiberOuter)
    {
        InitIfNot();
    }

    iStopToken  = std::move(aStopToken);
    iYieldRes   = GpTaskDoRes::DONE;

    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //>>>>>>>>>>>>>> OUTSIDE FIBER <<<<<<<<<<<<<<


    //>>>>>>>>>>>>>> INSIDE FIBER <<<<<<<<<<<<<<
    //VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
    *iFiberOuter.get() = std::move(*iFiberOuter.get()).resume();
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //>>>>>>>>>>>>>> INSIDE FIBER <<<<<<<<<<<<<<


    //>>>>>>>>>>>>>> OUTSIDE FIBER <<<<<<<<<<<<<<
    //VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV

    iStopToken = GpThreadStopToken();

    if (iException.has_value())
    {
        auto ex = std::move(iException.value());
        std::rethrow_exception(ex);
    }

    return iYieldRes;
}

void    GpTaskFiberCtx::InitIfNot (void)
{
    if (iFiberOuter)
    {
        return;
    }

    //Get stack from pool
    auto res = GpTaskFiberManager::S().Acquire();

    THROW_COND_GP
    (
        res.has_value(),
        "Failed to get fiber stack from pool"_sv
    );

    iFiberStack = std::move(res.value());
    GpFiberStackT& fiberStack = *reinterpret_cast<GpFiberStackT*>(iFiberStack.Vn().Stack());

    //Create fiber
    iFiberOuter = std::make_unique<FiberT>
    (
        std::allocator_arg,
        GpFiberStackWrapperT(fiberStack),
        SFiberFn
    );
}

FiberT  GpTaskFiberCtx::SFiberFn (FiberT&& aFiber)
{
    //>>>>>>>>>>>>>> INSIDE FIBER <<<<<<<<<<<<<<
    //VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
    return GpTaskFiberAccessor::SCtx()->FiberFn(std::move(aFiber));
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //>>>>>>>>>>>>>> INSIDE FIBER <<<<<<<<<<<<<<
}

FiberT  GpTaskFiberCtx::FiberFn (FiberT&& aFiber)
{
    //>>>>>>>>>>>>>> INSIDE FIBER <<<<<<<<<<<<<<
    //VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV

    iFiberInner = std::move(aFiber);

    try
    {
        iYieldRes = GpTaskDoRes::DONE;
        GpTaskFiberAccessor::SFiberFn(iTaskFiber, iStopToken);
    } catch (const boost::context::detail::forced_unwind&)
    {
        iYieldRes = GpTaskDoRes::DONE;
        throw;//rethrow
    } catch (...)
    {
        iYieldRes   = GpTaskDoRes::DONE;
        iException  = std::current_exception();//save
    }

    return std::move(iFiberInner);
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //>>>>>>>>>>>>>> INSIDE FIBER <<<<<<<<<<<<<<
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
