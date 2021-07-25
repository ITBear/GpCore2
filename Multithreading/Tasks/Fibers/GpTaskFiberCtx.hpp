#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberStack.hpp"
#include "../../Threads/GpThreadStopToken.hpp"
#include "../GpTask.hpp"
#include "boost_context.hpp"
#include <cstddef>
#include <functional>

namespace GPlatform {

class GpTaskFiber;

class GpTaskFiberCtx
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskFiberCtx)
    CLASS_DECLARE_DEFAULTS(GpTaskFiberCtx)

public:
                                        GpTaskFiberCtx      (GpWP<GpTaskFiber> aTask) noexcept;
                                        ~GpTaskFiberCtx     (void) noexcept;

    GpTask::ResT                        Enter               (GpThreadStopToken aStopToken);
    void                                Yield               (const GpTask::ResT aRes);
    GpWP<GpTaskFiber>                   Task                (void) {return iTask;}

    static GpTaskFiberCtx::C::Opt::Ref  SCurrentCtx         (void) noexcept;
    static void                         SClearCurrentCtx    (void) noexcept;

private:
    void                                Clear               (void) noexcept;
    void                                InitIfNot           (void);

    static void                         SSetCurrentCtx      (GpTaskFiberCtx::C::Opt::Ref aCtx) noexcept;

    static FiberT                       SFiberFn            (FiberT&& aFiber);
    FiberT                              FiberFn             (FiberT&& aFiber);

private:
    std::unique_ptr<FiberT>             iFiberOuter;
    FiberT                              iFiberInner;
    GpTaskFiberStack::SP                iFiberStack;

    GpThreadStopToken                   iStopToken;
    GpWP<GpTaskFiber>                   iTask;
    GpTask::ResT                        iYieldRes       = GpTask::ResT::DONE;
    std::optional<std::exception_ptr>   iException;

    static GpElementsCatalog<std::thread::id, GpTaskFiberCtx::C::Opt::Ref>  sFiberArgs;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
