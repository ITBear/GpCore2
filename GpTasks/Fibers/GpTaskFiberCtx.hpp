#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "boost_context.hpp"
#include <cstddef>
#include <functional>

#include "GpTaskFiberStopEx.hpp"
#include "GpTaskFiberStack.hpp"
#include "../GpTask.hpp"

namespace GPlatform {

class GpTaskFiber;

class GpTaskFiberCtx
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskFiberCtx)
    CLASS_DD(GpTaskFiberCtx)

public:
    inline                              GpTaskFiberCtx  (GpTaskFiber& aTaskFiber) noexcept;
    inline                              ~GpTaskFiberCtx (void) noexcept;

    void                                Clear           (void) noexcept;
    GpTaskDoRes                         Enter           (GpThreadStopToken aStopToken);
    inline void                         Yield           (const GpTaskDoRes aRes);

private:
    void                                InitIfNot       (void);

    static FiberT                       SFiberFn        (FiberT&& aFiber);
    FiberT                              FiberFn         (FiberT&& aFiber);

private:
    GpTaskFiber&                        iTaskFiber;

    std::unique_ptr<FiberT>             iFiberOuter;
    FiberT                              iFiberInner;
    GpTaskFiberStack::SP                iFiberStack;

    GpThreadStopToken                   iStopToken;
    GpTaskDoRes                         iYieldRes   = GpTaskDoRes::DONE;
    std::optional<std::exception_ptr>   iException;
};

GpTaskFiberCtx::GpTaskFiberCtx (GpTaskFiber& aTaskFiber) noexcept:
iTaskFiber(aTaskFiber)
{
}

GpTaskFiberCtx::~GpTaskFiberCtx (void) noexcept
{
    //Clear();
}

void    GpTaskFiberCtx::Yield (const GpTaskDoRes aRes)
{
    iYieldRes = aRes;

    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //>>>>>>>>>>>>>> INSIDE FIBER <<<<<<<<<<<<<<

    //>>>>>>>>>>>>>> OUTSIDE FIBER <<<<<<<<<<<<<<
    //VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
    iFiberInner = std::move(iFiberInner).resume();
    //^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    //>>>>>>>>>>>>>> OUTSIDE FIBER <<<<<<<<<<<<<<

    //>>>>>>>>>>>>>> INSIDE FIBER <<<<<<<<<<<<<<
    //VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
