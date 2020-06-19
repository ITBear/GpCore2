#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SHARED_POINTERS)

#include <stddef.h>
#include <atomic>

#include "../Classes/GpClassesDefines.hpp"
#include "../Units/Other/count_t.hpp"

namespace GPlatform {

class GpReferenceCounter
{
    CLASS_REMOVE_CTRS(GpReferenceCounter);

protected:
    inline          GpReferenceCounter  (const count_t aCounterValue, void* aValuePtr) noexcept;

public:
    virtual         ~GpReferenceCounter (void) noexcept = default;

public:
    inline count_t  Count               (void) const noexcept;

    template<bool IsWeak>
    count_t         Accuire             (void) const noexcept;

    template<bool IsWeak>
    count_t         Release             (void) noexcept;

    template<typename T>
    T&              Value               (void) noexcept;

    template<typename T>
    const T&        Value               (void) const noexcept;

    template<typename T>
    T*              Ptr                 (void) noexcept;

    template<typename T>
    const T*        Ptr                 (void) const noexcept;

protected:
    virtual void    Clear               (void) noexcept = 0;

private:
    mutable std::atomic_size_t  iCounter;
    void*                       iValuePtr   = nullptr;
};

GpReferenceCounter::GpReferenceCounter (const count_t aCounterValue, void* aValuePtr) noexcept:
iCounter(aCounterValue.ValueAs<size_t>()),
iValuePtr(aValuePtr)
{
}

count_t GpReferenceCounter::Count (void) const noexcept
{
    const size_t cnt = iCounter.load(std::memory_order_acquire);
    return count_t::SMake(cnt);
}

template<bool IsWeak>
count_t GpReferenceCounter::Accuire (void) const noexcept
{
    if constexpr(IsWeak)
    {
        return Count();
    } else
    {
        const size_t cnt = iCounter.fetch_add(1, std::memory_order_relaxed);
        return count_t::SMake(cnt) + 1_cnt;
    }
}

template<bool IsWeak>
count_t GpReferenceCounter::Release (void) noexcept
{
    if constexpr(IsWeak)
    {
        return Count();
    } else
    {
        const size_t cnt = iCounter.fetch_sub(1, std::memory_order_relaxed);

        if (cnt <= 1)
        {
            iValuePtr = nullptr;
            Clear();
            return 0_cnt;
        } else
        {
            return count_t::SMake(cnt - 1);
        }
    }
}

template<typename T>
T&  GpReferenceCounter::Value (void) noexcept
{
    return *Ptr<T>();
}

template<typename T>
const T&    GpReferenceCounter::Value (void) const noexcept
{
    return *Ptr<T>();
}

template<typename T>
T*  GpReferenceCounter::Ptr (void) noexcept
{
    return reinterpret_cast<T*>(iValuePtr);
}

template<typename T>
const T*    GpReferenceCounter::Ptr (void) const noexcept
{
    return reinterpret_cast<const T*>(iValuePtr);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_SHARED_POINTERS)
