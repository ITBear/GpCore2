#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SYNC_PRIMITIVES)

#include "../Macro/GpMacroClass.hpp"
#include "GpAsmSpinPause.hpp"
#include <atomic>

namespace GPlatform{

class GpSpinlock
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSpinlock)

public:
                        GpSpinlock  (void) noexcept = default;
                        ~GpSpinlock (void) noexcept = default;

    inline void         lock        (void) noexcept;
    inline void         unlock      (void) noexcept;
    inline bool         try_lock    (void) noexcept;

private:
    std::atomic<bool>   iState = {0};
};

void    GpSpinlock::lock (void) noexcept
{
    for (;;)
    {
        if (!iState.exchange(true, std::memory_order_acquire))
        {
            return;
        }

        while (iState.load(std::memory_order_relaxed))
        {
            GP_ASM_SPIN_PAUSE();
        }
    }
}

void    GpSpinlock::unlock (void) noexcept
{
    iState.store(false, std::memory_order_release);
}

bool    GpSpinlock::try_lock (void) noexcept
{
    return     (!iState.load(std::memory_order_relaxed))
            && (!iState.exchange(true, std::memory_order_acquire));
}

}//GPlatform

#endif//#if defined(GP_USE_SYNC_PRIMITIVES)
