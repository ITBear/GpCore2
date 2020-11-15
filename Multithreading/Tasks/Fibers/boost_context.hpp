#pragma once

#include <boost/context/fiber.hpp>
#include <boost/context/stack_context.hpp>
#include "GpFixedSizeStack.hpp"

#include "../GpTask.hpp"
#include "../../Threads/GpThreadStopToken.hpp"

namespace GPlatform {

class GpFiberStackSizePolicyT
{
public:
    static size_t default_size()    {return size_t(16)*size_t(1024);}
    static size_t page_size()       {return size_t(sysconf(_SC_PAGESIZE));}
};

using GpFiberStackT = GpFixedSizeStack<GpFiberStackSizePolicyT>;

using FiberStackContextT    = boost::context::stack_context;
using FiberRunFnT           = std::function<void(GpThreadStopToken)>;
using FiberT                = boost::context::fiber;
using FiberPreallocatedT    = boost::context::preallocated;
using FiberArgsT            = GpTuple</*0*/std::optional<FiberT>,
                                      /*1*/std::optional<FiberRunFnT>,
                                      /*2*/std::optional<GpThreadStopToken>,
                                      /*3*/GpTask::ResT,
                                      /*4*/std::optional<std::exception_ptr>,
                                      /*5*/GpTask::WP>;

//using BasicProtectedFixedSizeStackT   = boost::context::basic_protected_fixedsize_stack<StackSizePolicyT>;

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
