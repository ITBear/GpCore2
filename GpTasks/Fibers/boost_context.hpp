#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

GP_WARNING_PUSH()
GP_WARNING_DISABLE(shadow)

#include <boost/context/fiber.hpp>
#include <boost/context/stack_context.hpp>

GP_WARNING_POP()

#include "GpFixedSizeStack.hpp"

namespace GPlatform {

class GpTaskFiberStack;

class GpFiberStackSizePolicyT
{
public:
    static size_t default_size()    {return size_t(16)*size_t(1024);}
    static size_t page_size()       {return size_t(sysconf(_SC_PAGESIZE));}
};

using GpFiberStackT = GpFixedSizeStack<GpFiberStackSizePolicyT>;

using FiberStackContextT    = boost::context::stack_context;
using FiberT                = boost::context::fiber;
using FiberPreallocatedT    = boost::context::preallocated;

class GpFiberStackWrapperT
{
public:
    using stack_context = FiberStackContextT;

public:
                    GpFiberStackWrapperT    (void) noexcept = delete;
                    GpFiberStackWrapperT    (const GpFiberStackWrapperT&) noexcept = delete;
                    GpFiberStackWrapperT    (GpFiberStackWrapperT&& aWrapper) noexcept: iStack(aWrapper.iStack) {}
                    GpFiberStackWrapperT    (GpFiberStackT& aStack) noexcept: iStack(aStack) {}

    stack_context   allocate                (void) {return iStack.allocate();}
    void            deallocate              (stack_context& aSctx) noexcept {iStack.deallocate(aSctx);}

private:
    GpFiberStackT&  iStack;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
