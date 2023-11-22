#include "GpTaskFiber.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

//#include <fmt/include/fmt/core.h>
#include <iostream>

GP_WARNING_PUSH()
GP_WARNING_DISABLE(shadow)

#include <boost/context/fiber.hpp>

GP_WARNING_POP()

#include "GpTaskFiberCtxFactory.hpp"
#include "GpTaskFiberCtxForceUnwind.hpp"

namespace GPlatform {

GpTaskFiber::~GpTaskFiber (void) noexcept
{
    if (iCtx.IsNotNULL())
    {
        //__builtin_trap();

        //std::cout.flush();
        //std::cout << boost::stacktrace::stacktrace() << std::endl;

        //GpStringUtils::SCerr(fmt::format("[GpTaskFiber::~GpTaskFiber]: iCtx is not null!. Task '{}'", GpUTF::S_UTF8_To_STR(Name())));
        GpStringUtils::SCerr(u8"[GpTaskFiber::~GpTaskFiber]: iCtx is not null!. Task '"_sv + Name() + u8"'");

        //Yes, app will crash
        std::terminate();
    }
}

GpTaskRunRes::EnumT GpTaskFiber::Run (void) noexcept
{
    GpTaskRunRes::EnumT         res = GpTaskRunRes::DONE;
    std::optional<GpException>  ex;

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
        ex = GpException(e.what());
    } catch (...)
    {
        ex = GpException(u8"[GpTaskFiber::Run]: unknown exception"_sv);
    }

    // Check if there are was exception
    if (ex.has_value())
    {
        GpStringUtils::SCerr(ex->what());
        res = GpTaskRunRes::DONE;

        std::optional<GpException> clearExOpt = ClearCtx();

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
        std::optional<GpException> clearExOpt = ClearCtx();

        if (clearExOpt.has_value())
        {
            StartPromise().Fulfill(clearExOpt.value());
            DonePromise().Fulfill(clearExOpt.value());
        } else
        {
            StartPromise().Fulfill(GpAny{});
            DonePromise().Fulfill(GpAny{});
        }
    }

    return res;
}

GpTaskRunRes::EnumT GpTaskFiber::FiberRun (void)
{
    try
    {
        // Check if OnStart was called
        if (!iIsStartCalled)
        {
            OnStart();

            iIsStartCalled = true;
            StartPromise().Fulfill(GpAny{});
        }

        // Do Step
        const GpTaskRunRes::EnumT res = OnStep();

        if (res == GpTaskRunRes::DONE)
        {
            CallStop();
        }

        return res;
    } catch (...)
    {
        CallStop();
        throw;
    }

    // Will never reach here
    return GpTaskRunRes::DONE;
}

void    GpTaskFiber::CallStop (void)
{
    if (iIsStopCalled)
    {
        return;
    }

    std::optional<GpException> stopExOpt = OnStop();
    if (stopExOpt.has_value())
    {
        throw stopExOpt.value();
    }

    iIsStopCalled = true;
}

std::optional<GpException>  GpTaskFiber::ClearCtx (void) noexcept
{
    std::optional<GpException> ex;

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
        GpStringUtils::SCerr(ex->what());
    } catch (const std::exception& e)
    {
        ex = GpException(e.what());
        GpStringUtils::SCerr(ex->what());
    } catch (...)
    {
        ex = GpException(u8"[GpTaskFiber::ClearCtx]: unknown exception"_sv);
        GpStringUtils::SCerr(ex->what());
    }

    return ex;
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
