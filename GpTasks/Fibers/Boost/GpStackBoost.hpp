#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)
#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)

#include "GpFixedSizeStackBoost.hpp"

namespace GPlatform {

class GpFiberStackSizePolicyT
{
public:
    static size_t default_size()    {return size_t(16)*size_t(1024);}
    static size_t page_size()       {return size_t(sysconf(_SC_PAGESIZE));}
};

class GpStackBoost
{
public:
    using StackImplT    = GpFixedSizeStackBoost<GpFiberStackSizePolicyT>;
    using StackCtxT     = boost::context::stack_context;

public:
                    GpStackBoost    (void) noexcept = delete;
                    GpStackBoost    (const GpStackBoost& aStack) noexcept: iStackImpl(aStack.iStackImpl) {}
                    GpStackBoost    (GpStackBoost&& aStack) noexcept: iStackImpl(aStack.iStackImpl) {}
                    GpStackBoost    (StackImplT& aStackImlp) noexcept: iStackImpl(aStackImlp) {}

    StackCtxT       allocate        (void) {return iStackImpl.allocate();}
    void            deallocate      (StackCtxT& aSctx) noexcept {iStackImpl.deallocate(aSctx);}

private:
    StackImplT&     iStackImpl;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)
#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
