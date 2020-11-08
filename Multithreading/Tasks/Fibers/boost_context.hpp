#pragma once

#include <boost/context/protected_fixedsize_stack.hpp>
#include <boost/context/fiber.hpp>
#include <boost/context/stack_context.hpp>

#include "../GpTask.hpp"
#include "../../Threads/GpThreadStopToken.hpp"

namespace GPlatform {

class StackSizePolicyT
{
public:
    static size_t default_size()    {return size_t(16)*size_t(1024);}
    static size_t page_size()       {return size_t(sysconf(_SC_PAGESIZE));}
};


using StackContextT                 = boost::context::stack_context;
using FiberRunFnT                   = std::function<void(GpThreadStopToken)>;
using FiberT                        = boost::context::fiber;
using PreallocatedT                 = boost::context::preallocated;
using FiberArgsT                    = GpTuple</*0*/std::optional<FiberT>,
                                              /*1*/std::optional<FiberRunFnT>,
                                              /*2*/std::optional<GpThreadStopToken>,
                                              /*3*/GpTask::ResT,
                                              /*4*/std::optional<std::exception_ptr>,
                                              /*5*/GpTask::WP>;
using BasicProtectedFixedSizeStackT = boost::context::basic_protected_fixedsize_stack<StackSizePolicyT>;


class GpPooledStack
{
public:
                            GpPooledStack   (StackContextT aStackContext) noexcept: iStackContext(aStackContext) {}
                            ~GpPooledStack  (void) noexcept = default;

    StackContextT           allocate        (void)
    {
        return iStackContext;
    }

    void                    deallocate      (StackContextT&)
    {
        //nop
    }

private:
    StackContextT           iStackContext;
};

}//namespace GPlatform
