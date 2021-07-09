#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SHARED_POINTERS)

#include <stddef.h>
#include <atomic>

#include "../Classes/GpClassesDefines.hpp"
#include "../Units/Other/count_t.hpp"
#include "../Containers/GpContainersT.hpp"
#include "../Bits/GpBitOps.hpp"

namespace GPlatform {

class GpReferenceCounter
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReferenceCounter)

protected:
    inline              GpReferenceCounter  (void* aValuePtr) noexcept;

public:
    virtual             ~GpReferenceCounter (void) noexcept = default;

public:
    inline count_t      CountStrong         (void) const noexcept;
    inline count_t      CountWeak           (void) const noexcept;

    template<bool IsWeak>
    void                Acquire             (void) const noexcept;

    template<bool IsWeak>
    void                Release             (void) noexcept;

    template<typename T>
    T*                  ValuePtr            (void) noexcept;

    template<typename T>
    const T*            ValuePtr            (void) const noexcept;

protected:
    virtual void        DeleteValue         (void) noexcept = 0;
    virtual void        DeleteSelf          (void) noexcept = 0;

private:
    template<size_t StrongSubV, size_t WeakSubV>
    inline std::array<size_t, 2>
                        Counters            (const size_t aCountersValue) const noexcept;

    inline std::array<size_t, 2>
                        Counters            (void) const noexcept;

    inline void         CounterStrongInc    (void) const noexcept;
    inline void         CounterWeakInc      (void) const noexcept;

    inline std::array<size_t, 2>
                        CounterStrongDec    (void) noexcept;
    inline std::array<size_t, 2>
                        CounterWeakDec      (void) noexcept;

private:
    mutable std::atomic_size_t  iCounters   = 0;
    void*                       iValuePtr   = nullptr;
};

GpReferenceCounter::GpReferenceCounter (void* aValuePtr) noexcept:
iCounters(1),
iValuePtr(aValuePtr)
{
}

count_t GpReferenceCounter::CountStrong (void) const noexcept
{
    return count_t::SMake(Counters().at(0));
}

count_t GpReferenceCounter::CountWeak (void) const noexcept
{
    return count_t::SMake(Counters().at(1));
}

template<bool IsWeak>
void    GpReferenceCounter::Acquire (void) const noexcept
{
    if constexpr(IsWeak)
    {
        CounterWeakInc();
    } else
    {
        CounterStrongInc();
    }
}

template<bool IsWeak>
void    GpReferenceCounter::Release (void) noexcept
{
    if constexpr(IsWeak)
    {
        CounterWeakDec();

        /*const std::array<size_t, 2> counters = CounterWeakDec();

        if (counters.at(1) == 0)
        {
            if (counters.at(0) == 0)
            {
                DeleteSelf();
            }
        }*/
    } else
    {
        const std::array<size_t, 2> counters = CounterStrongDec();

        if (counters.at(0) == 0)
        {
            DeleteValue();
            iValuePtr = nullptr;

            if (CountWeak() == 0_cnt)
            {
                DeleteSelf();
            }

            /*if (counters.at(1) > 0)
            {
                DeleteValue();
            } else
            {
                DeleteSelf();
            }*/
        }
    }
}

template<typename T>
T*  GpReferenceCounter::ValuePtr (void) noexcept
{
    return reinterpret_cast<T*>(iValuePtr);
}

template<typename T>
const T*    GpReferenceCounter::ValuePtr (void) const noexcept
{
    return reinterpret_cast<const T*>(iValuePtr);
}

template<size_t StrongSubV, size_t WeakSubV>
std::array<size_t, 2>   GpReferenceCounter::Counters (const size_t aCountersValue) const noexcept
{
    constexpr const size_t  maskStrong  = BitOps::MakeMaskLO<size_t>();
    constexpr const size_t  maskWeak    = BitOps::MakeMaskHI<size_t>();

    return {((aCountersValue & maskStrong) >> (                 0)) - StrongSubV,
            ((aCountersValue & maskWeak  ) >> (sizeof(size_t)*8/2)) - WeakSubV};
}

std::array<size_t, 2>   GpReferenceCounter::Counters (void) const noexcept
{
    return Counters<0, 0>(iCounters.load(std::memory_order_acquire));
}

void    GpReferenceCounter::CounterStrongInc (void) const noexcept
{
    constexpr const size_t val = size_t(1) << 0;
    iCounters.fetch_add(val, std::memory_order_release);
}

void    GpReferenceCounter::CounterWeakInc (void) const noexcept
{
    constexpr const size_t val = size_t(1) << (sizeof(size_t)*8/2);
    iCounters.fetch_add(val, std::memory_order_release);
}

std::array<size_t, 2>   GpReferenceCounter::CounterStrongDec (void) noexcept
{
    constexpr const size_t val = size_t(1) << 0;
    return Counters<1, 0>(iCounters.fetch_sub(val, std::memory_order_release));
}

std::array<size_t, 2>   GpReferenceCounter::CounterWeakDec (void) noexcept
{
    constexpr const size_t val = size_t(1) << (sizeof(size_t)*8/2);
    return Counters<0, 1>(iCounters.fetch_sub(val, std::memory_order_release));
}

}//namespace GPlatform

#endif//#if defined(GP_USE_SHARED_POINTERS)
