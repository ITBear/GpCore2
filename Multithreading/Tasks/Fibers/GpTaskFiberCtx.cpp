#include "GpTaskFiberCtx.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../Exceptions/GpExceptionsSink.hpp"
#include "GpTaskFiberManager.hpp"
#include "boost_context.hpp"
#include "GpTaskFiber.hpp"

namespace GPlatform {

static thread_local FiberArgsT sFiberArgsTLS;

static FiberT _FiberFn (FiberT&& aOuterFiber)
{
    //IN
    {
        // Call actual fn
        try
        {
            FiberArgsT& fiberArgs = sFiberArgsTLS;

            std::get<0>(fiberArgs) = std::move(aOuterFiber);
            std::get<1>(fiberArgs).value()(std::get<2>(fiberArgs).value());
        } catch (boost::context::detail::forced_unwind&)
        {
            throw;
        } catch (...)
        {
            FiberArgsT& fiberArgs = sFiberArgsTLS;

            std::get<3>(fiberArgs) = GpTask::Res::DONE;
            std::get<4>(fiberArgs) = std::current_exception();

            return std::move(std::get<0>(fiberArgs).value());
        }
    }

    //OUT
    {
        FiberArgsT& fiberArgs = sFiberArgsTLS;

        std::get<3>(fiberArgs) = GpTask::Res::DONE;
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

    // Allocate stack
    const auto res = GpTaskFiberManager::S().StackPool().Acquire();
    if (res.has_value())
    {
        iStack = res.value();
    } else
    {
        THROW_GPE("Failed to allocate stack for fiber"_sv);
    }

    // Create fiber
    StackContextT   sctx        = *reinterpret_cast<StackContextT*>(iStack.Vn().Context());
    void*           sctx_sp     = static_cast<char*>(sctx.sp);
    size_t          sctx_size   = sctx.size;

    iFiberPtr = GpMemOps::SEmplace<FiberT>(iFiberStorage.data(),
                                           std::allocator_arg,
                                           PreallocatedT(sctx_sp, sctx_size, sctx),
                                           GpPooledStack(sctx),
                                           _FiberFn);
}

void    GpTaskFiberCtx::Clear (void) noexcept
{
    if (iFiberPtr != nullptr)
    {
        FiberT* fiber = reinterpret_cast<FiberT*>(iFiberPtr);
        fiber->~FiberT();
        iFiberPtr = nullptr;
    }

    if (iStack.IsNotNULL())
    {
        GpTaskFiberManager::S().StackPool().Release(iStack);
        iStack.Clear();
    }
}

GpTask::Res GpTaskFiberCtx::Enter (GpThreadStopToken    aStopToken,
                                   GpTask::WP           aTask,
                                   FiberRunFnT          aRunFn)
{
    //IN
    {
        FiberArgsT& fiberArgs = sFiberArgsTLS;

        std::get<1>(fiberArgs) = aRunFn;
        std::get<2>(fiberArgs) = aStopToken;
        std::get<3>(fiberArgs) = GpTask::Res::DONE;
        //std::get<4>(fiberArgs) = task res
        std::get<5>(fiberArgs) = aTask;

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

        GpTask::Res res = std::get<3>(fiberArgs);
        return res;
    }
}

void    GpTaskFiberCtx::SYeld (const GpTask::Res aRes)
{
    FiberT fiber;

    //OUT
    {
        FiberArgsT& fiberArgs   = sFiberArgsTLS;
        fiber                   = std::move(std::get<0>(fiberArgs).value());

        std::get<3>(fiberArgs)  = aRes;
        std::get<4>(fiberArgs).reset();

        fiber = std::move(fiber).resume();
    }

    //IN
    {
        FiberArgsT& fiberArgs = sFiberArgsTLS;

        std::get<0>(fiberArgs) = std::move(fiber);

        if (std::get<2>(fiberArgs)->stop_requested())
        {
            //throw boost::context::detail::forced_unwind();
            THROW_GPE("Stop GpTaskFiber by force"_sv);
        }
    }
}

GpTask::WP  GpTaskFiberCtx::SCurrentTask (void)
{
    FiberArgsT& fiberArgs = sFiberArgsTLS;
    return std::get<5>(fiberArgs);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
