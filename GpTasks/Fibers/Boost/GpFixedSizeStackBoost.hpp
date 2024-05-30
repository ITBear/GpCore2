#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)
#if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)

#if defined(GP_POSIX)
#   include <sys/mman.h>
#endif

#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>
#include <GpCore2/GpUtils/Types/Numerics/GpNumericOps.hpp>

extern "C"
{
#   include <fcntl.h>
#   include <sys/stat.h>
}

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstddef>
#include <new>

GP_WARNING_PUSH()

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(shadow)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#include <boost/assert.hpp>
#include <boost/config.hpp>

#include <boost/context/detail/config.hpp>
#include <boost/context/stack_context.hpp>
#include <boost/context/stack_traits.hpp>

GP_WARNING_POP()

#if defined(BOOST_USE_VALGRIND)
#   include <valgrind/valgrind.h>
#endif

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace GPlatform {

template<typename SizePolicyT>
class GpFixedSizeStackBoost
{
public:
    using StackCtxT = boost::context::stack_context;

public:
                            GpFixedSizeStackBoost   (void) noexcept;
                            GpFixedSizeStackBoost   (const size_t aSize) noexcept;
                            GpFixedSizeStackBoost   (const GpFixedSizeStackBoost&) noexcept = delete;
                            GpFixedSizeStackBoost   (GpFixedSizeStackBoost&& aStack) noexcept;
                            ~GpFixedSizeStackBoost  (void) noexcept;

    GpFixedSizeStackBoost&  operator=               (GpFixedSizeStackBoost&& aStack) noexcept;

    StackCtxT               allocate                (void);
    void                    deallocate              (StackCtxT& aCtx) noexcept;

    bool                    IsAllocated             (void) const noexcept {return iMemPtr != nullptr;}

private:
    StackCtxT               NewCtx                  (void);
    void                    FreeCtx                 (StackCtxT& aCtx) noexcept;
    void                    MemAlloc                (void);
    void                    MemFree                 (void) noexcept;

private:
    size_t                  iSize           = 0;
    size_t                  iAllocatedSize  = 0;
    void*                   iMemPtr         = nullptr;
};

template<typename SizePolicyT>
GpFixedSizeStackBoost<SizePolicyT>::GpFixedSizeStackBoost (void) noexcept:
iSize(SizePolicyT::default_size())
{
}

template<typename SizePolicyT>
GpFixedSizeStackBoost<SizePolicyT>::GpFixedSizeStackBoost (const size_t aSize) noexcept:
iSize(aSize)
{
}

template<typename SizePolicyT>
GpFixedSizeStackBoost<SizePolicyT>::GpFixedSizeStackBoost (GpFixedSizeStackBoost&& aStack) noexcept:
iSize         (aStack.iSize),
iAllocatedSize(aStack.iAllocatedSize),
iMemPtr       (aStack.iMemPtr)
{
    aStack.iSize            = 0;
    aStack.iAllocatedSize   = 0;
    aStack.iMemPtr          = nullptr;
}

template<typename SizePolicyT>
GpFixedSizeStackBoost<SizePolicyT>::~GpFixedSizeStackBoost (void) noexcept
{
    MemFree();
}

template<typename SizePolicyT>
GpFixedSizeStackBoost<SizePolicyT>& GpFixedSizeStackBoost<SizePolicyT>::operator= (GpFixedSizeStackBoost&& aStack) noexcept
{
    iSize                   = aStack.iSize;
    iAllocatedSize          = aStack.iAllocatedSize;
    iMemPtr                 = aStack.iMemPtr;

    aStack.iSize            = 0;
    aStack.iAllocatedSize   = 0;
    aStack.iMemPtr          = nullptr;

    return *this;
}

template<typename SizePolicyT>
typename GpFixedSizeStackBoost<SizePolicyT>::StackCtxT
GpFixedSizeStackBoost<SizePolicyT>::allocate (void)
{
    return NewCtx();
}

template<typename SizePolicyT>
void    GpFixedSizeStackBoost<SizePolicyT>::deallocate (StackCtxT& aCtx) noexcept
{
    FreeCtx(aCtx);
}

template<typename SizePolicyT>
typename GpFixedSizeStackBoost<SizePolicyT>::StackCtxT
GpFixedSizeStackBoost<SizePolicyT>::NewCtx (void)
{
    MemAlloc();

    StackCtxT ctx;
    ctx.size    = iAllocatedSize;
    ctx.sp      = static_cast<std::byte*>(iMemPtr) + ctx.size;

#if defined(BOOST_USE_VALGRIND)
    sctx.valgrind_stack_id = VALGRIND_STACK_REGISTER(sctx.sp, iMemPtr);
#endif

    return ctx;
}

template<typename SizePolicyT>
void    GpFixedSizeStackBoost<SizePolicyT>::FreeCtx ([[maybe_unused]] StackCtxT& aCtx) noexcept
{
    BOOST_ASSERT(aCtx.sp);

#if defined(BOOST_USE_VALGRIND)
    VALGRIND_STACK_DEREGISTER(aCtx.valgrind_stack_id);
#endif
}

template<typename SizePolicyT>
void    GpFixedSizeStackBoost<SizePolicyT>::MemAlloc (void)
{
    if (iMemPtr != nullptr)
    {
        //MemFree();
        return;
    }

    const size_t pagesCount     = NumOps::SDivCeil<size_t>(iSize, SizePolicyT::page_size());
    const size_t sizeToAllocate = NumOps::SMul<size_t>(NumOps::SAdd<size_t>(pagesCount, 1), SizePolicyT::page_size());

#if defined(GP_OS_WINDOWS)
    iMemPtr = _aligned_malloc(sizeToAllocate, 4096);

    if (iMemPtr == nullptr) [[unlikely]]
    {
        throw std::bad_alloc();
    }

    iAllocatedSize = sizeToAllocate;
#else
#   if defined(BOOST_CONTEXT_USE_MAP_STACK)
        iMemPtr = ::mmap(0, sizeToAllocate, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_STACK, -1, 0);
#   elif defined(MAP_ANON)
        iMemPtr = ::mmap(0, sizeToAllocate, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
#   else
        iMemPtr = ::mmap(0, sizeToAllocate, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#   endif

        if (iMemPtr == MAP_FAILED) [[unlikely]]
        {
            iMemPtr = nullptr;
            throw std::bad_alloc();
        }

        iAllocatedSize = sizeToAllocate;

#   if defined(BOOST_DISABLE_ASSERTS)
        ::mprotect(iMemPtr, SizePolicyT::page_size(), PROT_NONE);
#   else
        const int result(::mprotect(iMemPtr, SizePolicyT::page_size(), PROT_NONE));
        BOOST_ASSERT(0 == result);
#   endif
#endif// #if defined(GP_OS_WINDOWS) ... #else
}

template<typename SizePolicyT>
void    GpFixedSizeStackBoost<SizePolicyT>::MemFree (void) noexcept
{
    if (iMemPtr == nullptr)
    {
        return;
    }

#if defined(GP_OS_WINDOWS)
    _aligned_free(iMemPtr);
    //std::free(iMemPtr);
#else
    ::munmap(iMemPtr, iAllocatedSize);
#endif

    iAllocatedSize  = 0;
    iMemPtr         = nullptr;
}

}// namespace GPlatform

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS_BOOST_IMPL)
#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
