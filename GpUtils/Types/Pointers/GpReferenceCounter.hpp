#pragma once

#include "../../GpMacro.hpp"

#if defined(GP_USE_SHARED_POINTERS)

#include <stddef.h>
#include <atomic>

namespace GPlatform {

class GpReferenceCounter
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReferenceCounter)

protected:
    inline explicit         GpReferenceCounter  (const void* aValuePtr) noexcept;
    inline explicit         GpReferenceCounter  (void* aValuePtr) noexcept;

public:
    virtual                 ~GpReferenceCounter (void) noexcept = default;

public:
    inline size_t           Counter             (void) const noexcept;

    template<bool IsWeak>
    size_t                  Acquire             (void) noexcept;

    template<bool IsWeak>
    size_t                  Release             (void) noexcept;

    template<typename T>
    T*                      ValuePtr            (void) noexcept;

private:
    std::atomic_size_t      iCounter    = 0;
    void*                   iValuePtr   = nullptr;
};

GpReferenceCounter::GpReferenceCounter (const void* aValuePtr) noexcept:
iCounter(1),
iValuePtr(const_cast<void*>(aValuePtr))
{
}

GpReferenceCounter::GpReferenceCounter (void* aValuePtr) noexcept:
iCounter(1),
iValuePtr(aValuePtr)
{
}

size_t  GpReferenceCounter::Counter (void) const noexcept
{
    return iCounter.load(std::memory_order_acquire);
}

template<bool IsWeak>
size_t  GpReferenceCounter::Acquire (void) noexcept
{
    if constexpr(!IsWeak)
    {
        return iCounter.fetch_add(1, std::memory_order_release) + 1;
    } else
    {
        return iCounter.load(std::memory_order_acquire);
    }
}

template<bool IsWeak>
size_t  GpReferenceCounter::Release (void) noexcept
{
    if constexpr(!IsWeak)
    {
        const size_t prevCount = iCounter.fetch_sub(1, std::memory_order_release);

        if (prevCount == 1)
        {
            iValuePtr = nullptr;
        }

        return prevCount - 1;
    } else
    {
        return iCounter.load(std::memory_order_acquire);
    }
}

template<typename T>
T*  GpReferenceCounter::ValuePtr (void) noexcept
{
    return reinterpret_cast<T*>(iValuePtr);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_SHARED_POINTERS)
