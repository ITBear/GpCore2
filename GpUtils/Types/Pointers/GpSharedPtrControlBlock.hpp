#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/Types/Numerics/GpNumericTypes.hpp>
#include <GpCore2/GpUtils/GpMemOps.hpp>

#include <atomic>

namespace GPlatform {

class GpSharedPtrControlBlock
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpSharedPtrControlBlock)
    TAG_SET(THREAD_SAFE)

protected:
    inline                  GpSharedPtrControlBlock     (void* aPointer) noexcept;

public:
    virtual                 ~GpSharedPtrControlBlock    (void) noexcept = default;

    inline const void*      Pointer                     (void) const noexcept;
    inline void*            Pointer                     (void) noexcept;

    inline u_int_64         CounterRaw                  (void) const noexcept;
    inline u_int_64         CounterStrong               (void) const noexcept;
    inline u_int_64         CounterWeak                 (void) const noexcept;

    inline void             AcquireStrong               (void) noexcept;
    inline void             AcquireWeak                 (void) noexcept;

    template<typename T>
    inline void             ReleaseStrong               (void) noexcept;
    inline void             ReleaseWeak                 (void) noexcept;

    inline bool             TryAcquireStrongIfNotZero   (void) noexcept;

private:
    template<typename T>
    void                    DestructValue               (void) noexcept;
    inline void             DeleteSelf                  (void) noexcept;

private:
    std::atomic<u_int_64>   iCounter = u_int_64{u_int_64{1} | (u_int_64{1} << 32)};
    void*                   iPointer = nullptr;
};

GpSharedPtrControlBlock::GpSharedPtrControlBlock (void* aPointer) noexcept:
iPointer{aPointer}
{
}

const void* GpSharedPtrControlBlock::Pointer (void) const noexcept
{
    return iPointer;
}

void*   GpSharedPtrControlBlock::Pointer (void) noexcept
{
    return iPointer;
}

u_int_64    GpSharedPtrControlBlock::CounterRaw (void) const noexcept
{
    return iCounter.load(std::memory_order_acquire);
}

u_int_64    GpSharedPtrControlBlock::CounterStrong (void) const noexcept
{
    const u_int_64 counter = CounterRaw();
    return counter & u_int_64{0x00000000FFFFFFFF};
}

u_int_64    GpSharedPtrControlBlock::CounterWeak (void) const noexcept
{
    const u_int_64 counter = CounterRaw();
    return (counter & u_int_64{0xFFFFFFFF00000000}) >> 32;
}

void    GpSharedPtrControlBlock::AcquireStrong (void) noexcept
{
    iCounter.fetch_add(1, std::memory_order_acq_rel);
}

void    GpSharedPtrControlBlock::AcquireWeak (void) noexcept
{
    iCounter.fetch_add((u_int_64{1} << 32), std::memory_order_acq_rel);
}

template<typename T>
void    GpSharedPtrControlBlock::ReleaseStrong (void) noexcept
{
    const u_int_64 prevCounter      = iCounter.fetch_sub(1, std::memory_order_acq_rel);
    const u_int_64 newStrongCounter = (prevCounter - 1) & u_int_64{0x00000000FFFFFFFF};

    if (newStrongCounter == 0) [[unlikely]]
    {
        DestructValue<T>();
        ReleaseWeak();
    }
}

void    GpSharedPtrControlBlock::ReleaseWeak (void) noexcept
{
    const u_int_64 prevCounter              = iCounter.fetch_sub((u_int_64{1} << 32), std::memory_order_acq_rel);
    const u_int_64 newStrongAndWeakCounter  = prevCounter - (u_int_64{1} << 32);

    if (newStrongAndWeakCounter == 0) [[unlikely]]
    {
        DeleteSelf();
    }
}

bool    GpSharedPtrControlBlock::TryAcquireStrongIfNotZero (void) noexcept
{
    u_int_64 counter = CounterRaw();

    for (;;)
    {
        if (u_int_64 strong = counter & u_int_64{0x00000000FFFFFFFF}; strong == 0)
        {
            return false;
        }

        const u_int_64 desired = counter + u_int_64{1};

        if (iCounter.compare_exchange_weak(
            counter,
            desired,
            std::memory_order_acq_rel,
            std::memory_order_acquire
        ))
        {
            return true;
        }

        // CAS failed: counter now has updated value, loop and re-check strong
    }
}

template<typename T>
void    GpSharedPtrControlBlock::DestructValue (void) noexcept
{
    static_cast<T*>(Pointer())->~T();
    iPointer = nullptr;
}

void    GpSharedPtrControlBlock::DeleteSelf (void) noexcept
{
    MemOps::SDelete(this);
}

}// namespace GPlatform

