#pragma once

#include "../GpMacro.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <atomic>
#include "GpAsmSpinPause.hpp"

namespace GPlatform{

class GpSpinlock
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSpinlock)

public:
            GpSpinlock  (void) noexcept = default;
            ~GpSpinlock (void) noexcept = default;

    void    lock        (void) noexcept
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

    void    unlock      (void) noexcept
    {
        iState.store(false, std::memory_order_release);
    }

    bool    try_lock    (void) noexcept
    {
        return     (!iState.load(std::memory_order_relaxed))
                && (!iState.exchange(true, std::memory_order_acquire));
    }

private:
    std::atomic<bool>   iState = {0};
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
