#include "GpTaskFiberCtx.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../Exceptions/GpExceptionsSink.hpp"
#include "GpTaskFiberManager.hpp"
#include "GpTaskFiber.hpp"
#include "GpTaskFiberStopEx.hpp"
#include "boost_context.hpp"
#include "../../../Utils/RAII/GpRAIIonDestruct.hpp"

//#include <iostream>

/*#if defined(USE_ASAN)
#   include <sanitizer/common_interface_defs.h>
#endif*/

namespace GPlatform {

static thread_local FiberArgsT sFiberArgsTLS;

static FiberT _FiberFn (FiberT&& aOuterFiber)
{
    //After IN
    {
        // Call actual fn
        try
        {
            FiberArgsT& fiberArgs = sFiberArgsTLS;

            std::get<0>(fiberArgs) = std::move(aOuterFiber);
            std::get<1>(fiberArgs).value()(std::get<2>(fiberArgs).value());
        } catch (const boost::context::detail::forced_unwind&)
        {
            throw;
        } catch (...)
        {
            FiberArgsT& fiberArgs = sFiberArgsTLS;

            std::get<3>(fiberArgs) = GpTask::ResT::DONE;
            std::get<4>(fiberArgs) = std::current_exception();

            return std::move(std::get<0>(fiberArgs).value());
        }
    }

    //Before OUT
    {
        FiberArgsT& fiberArgs = sFiberArgsTLS;

        std::get<3>(fiberArgs) = GpTask::ResT::DONE;
        std::get<4>(fiberArgs).reset();

        return std::move(std::get<0>(fiberArgs).value());
    }
}

GpTaskFiberCtx::GpTaskFiberCtx (void) noexcept
{
    static_assert(sizeof(iFiberStorage) >= sizeof(FiberT));
}

GpTaskFiberCtx::~GpTaskFiberCtx (void) noexcept
{
    Clear();
}

void    GpTaskFiberCtx::Init (void)
{
    Clear();

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

    iFiberPtr = GpMemOps::SEmplace<FiberT>
    (
        iFiberStorage.data(),
        std::allocator_arg,
        //FiberPreallocatedT(stackCtx.sp, stackCtx.size, stackCtx),
        GpFiberStackWrapperT(fiberStack),
        _FiberFn
    );
}

void    GpTaskFiberCtx::Clear (void) noexcept
{
    if (iFiberPtr != nullptr)
    {
        FiberT* fiber = reinterpret_cast<FiberT*>(iFiberPtr);
        fiber->~FiberT();
        iFiberPtr = nullptr;
    }

    if (iFiberStack.IsNotNULL())
    {
        GpTaskFiberManager::S().StackPool().Release(std::move(iFiberStack));
    }
}

GpTask::ResT    GpTaskFiberCtx::Enter
(
    GpThreadStopToken   aStopToken,
    GpTask::WP          aTask,
    FiberRunFnT         aRunFn
)
{
    //IN
    {
        FiberArgsT& fiberArgs = sFiberArgsTLS;

        std::get<1>(fiberArgs) = aRunFn;
        std::get<2>(fiberArgs) = std::move(aStopToken);
        std::get<3>(fiberArgs) = GpTask::ResT::DONE;
        //std::get<4>(fiberArgs) = task res
        std::get<5>(fiberArgs) = std::move(aTask);

        FiberT& fiber = *reinterpret_cast<FiberT*>(iFiberPtr);

        fiber = std::move(fiber).resume();
    }

    //OUT
    {
        FiberArgsT& fiberArgs = sFiberArgsTLS;

        std::get<5>(fiberArgs).Clear();

        auto& ex = std::get<4>(fiberArgs);
        if (ex.has_value())
        {
             std::rethrow_exception(ex.value());
        }

        GpTask::ResT res = std::get<3>(fiberArgs);
        return res;
    }
}

void    GpTaskFiberCtx::SYeld (const GpTask::ResT aRes)
{
    //Before OUT
    FiberArgsT& fiberArgsBeforeOut  = sFiberArgsTLS;

    std::get<3>(fiberArgsBeforeOut) = aRes;
    std::get<4>(fiberArgsBeforeOut).reset();

    //std::cout << "[GpTaskFiberCtx::SYeld]: Before OUT "_sv << "thread: "_sv << std::this_thread::get_id() <<std::endl;

    FiberT fiberAfterIn = std::move(std::get<0>(fiberArgsBeforeOut).value()).resume();

    //std::cout << "[GpTaskFiberCtx::SYeld]: After IN "_sv << "thread: "_sv << std::this_thread::get_id() <<std::endl;

    //After IN
    {
        FiberArgsT& fiberArgsAfterIn = sFiberArgsTLS;

        std::get<0>(fiberArgsAfterIn) = std::move(fiberAfterIn);

        if (std::get<2>(fiberArgsAfterIn)->stop_requested())
        {
            //throw boost::context::detail::forced_unwind();
            throw GpTaskFiberStopEx("Stop GpTaskFiber by force"_sv);
        }
    }
}

GpTask::WP  GpTaskFiberCtx::SCurrentTask (void)
{
    FiberArgsT& fiberArgs = sFiberArgsTLS;
    return std::get<5>(fiberArgs);
}

bool    GpTaskFiberCtx::SIsIntoFiber (void) noexcept
{
    FiberArgsT& fiberArgs = sFiberArgsTLS;
    return std::get<0>(fiberArgs).has_value();
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
