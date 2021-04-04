#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../../Types/Numerics/GpNumericOps.hpp"

#include <iostream>

extern "C"
{
#   include <fcntl.h>
#   include <sys/mman.h>
#   include <sys/stat.h>
#   include <unistd.h>
}

#include <cmath>
#include <cstddef>
#include <new>

#include <boost/assert.hpp>
#include <boost/config.hpp>

#include <boost/context/detail/config.hpp>
#include <boost/context/stack_context.hpp>
#include <boost/context/stack_traits.hpp>

#if defined(BOOST_USE_VALGRIND)
#include <valgrind/valgrind.h>
#endif

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_PREFIX
#endif

namespace GPlatform {

template<typename TraitsT>
class GpFixedSizeStack
{
public:
    using traits_type   = TraitsT;
    using stack_context = boost::context::stack_context;

public:
                        GpFixedSizeStack    (void) noexcept = delete;
                        GpFixedSizeStack    (const GpFixedSizeStack&) noexcept = delete;
                        GpFixedSizeStack    (GpFixedSizeStack&&) noexcept = delete;
                        GpFixedSizeStack    (const size_t aSize = traits_type::default_size()) noexcept;
                        ~GpFixedSizeStack   (void) noexcept;

    stack_context       allocate            (void);
    void                deallocate          (stack_context& aSctx) noexcept;

private:
    stack_context       NewCtx              (void);
    void                FreeCtx             (stack_context& aSctx) noexcept;
    void                MemAlloc            (void);
    void                MemFree             (void) noexcept;

private:
    const size_t        iSize           = 0;
    size_t              iAllocatedSize  = 0;
    void*               iMemPtr         = nullptr;
};

template<typename TraitsT>
GpFixedSizeStack<TraitsT>::GpFixedSizeStack (const size_t aSize) noexcept:
iSize(aSize)
{
}

template<typename TraitsT>
GpFixedSizeStack<TraitsT>::~GpFixedSizeStack (void) noexcept
{
    MemFree();
}

template<typename TraitsT>
typename GpFixedSizeStack<TraitsT>::stack_context
GpFixedSizeStack<TraitsT>::allocate (void)
{
    return NewCtx();
}

template<typename TraitsT>
void    GpFixedSizeStack<TraitsT>::deallocate (stack_context& aSctx) noexcept
{
    FreeCtx(aSctx);
}

template<typename TraitsT>
typename GpFixedSizeStack<TraitsT>::stack_context
GpFixedSizeStack<TraitsT>::NewCtx (void)
{
    MemAlloc();

    stack_context sctx;
    sctx.size   = iAllocatedSize;
    sctx.sp     = static_cast<std::byte*>(iMemPtr) + sctx.size;
#if defined(BOOST_USE_VALGRIND)
    sctx.valgrind_stack_id = VALGRIND_STACK_REGISTER(sctx.sp, iMemPtr);
#endif

    return sctx;
}

template<typename TraitsT>
void    GpFixedSizeStack<TraitsT>::FreeCtx (stack_context& aSctx) noexcept
{
    BOOST_ASSERT(aSctx.sp);

#if defined(BOOST_USE_VALGRIND)
    VALGRIND_STACK_DEREGISTER(aSctx.valgrind_stack_id);
#endif
}

template<typename TraitsT>
void    GpFixedSizeStack<TraitsT>::MemAlloc (void)
{
    if (iMemPtr != nullptr)
    {
        //MemFree();
        return;
    }

    const size_t pagesCount     = NumOps::SDivCeil<size_t>(iSize, traits_type::page_size());
    const size_t sizeToAllocate = NumOps::SMul<size_t>(NumOps::SAdd<size_t>(pagesCount, 1), traits_type::page_size());

    //iMemPtr = std::aligned_alloc(4096, sizeToAllocate);
    //iAllocatedSize = sizeToAllocate;

    //std::cout << iMemPtr << std::endl;

#if defined(BOOST_CONTEXT_USE_MAP_STACK)
    iMemPtr = ::mmap(0, sizeToAllocate, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON | MAP_STACK, -1, 0);
#elif defined(MAP_ANON)
    iMemPtr = ::mmap(0, sizeToAllocate, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANON, -1, 0);
#else
    iMemPtr = ::mmap(0, sizeToAllocate, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
#endif

    if (iMemPtr == MAP_FAILED)
    {
        iMemPtr = nullptr;
        throw std::bad_alloc();
    }

    iAllocatedSize = sizeToAllocate;

#if defined(BOOST_DISABLE_ASSERTS)
    ::mprotect(iMemPtr, traits_type::page_size(), PROT_NONE);
#else
    const int result(::mprotect(iMemPtr, traits_type::page_size(), PROT_NONE));
    BOOST_ASSERT(0 == result);
#endif
}

template<typename TraitsT>
void    GpFixedSizeStack<TraitsT>::MemFree (void) noexcept
{
    if (iMemPtr == nullptr)
    {
        return;
    }

    ::munmap(iMemPtr, iAllocatedSize);
    //std::free(iMemPtr);

    iAllocatedSize  = 0;
    iMemPtr         = nullptr;
}

}//namespace GPlatform

#ifdef BOOST_HAS_ABI_HEADERS
#  include BOOST_ABI_SUFFIX
#endif

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
