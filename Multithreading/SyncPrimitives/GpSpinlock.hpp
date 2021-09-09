#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include <atomic>
#include "../../Asm/GpAsmSpinPause.hpp"
#include "../../Types/Classes/GpClassesDefines.hpp"

#include "../../Types/DateTime/GpDateTimeOps.hpp"

#include <iostream>

namespace GPlatform{

class GpSpinlock
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSpinlock)

public:
            //GpSpinlock    (void) noexcept :iState(false) {}
            GpSpinlock  (void) noexcept = default;
            ~GpSpinlock (void) noexcept = default;

    void    lock        (void) noexcept
    {
        microseconds_t beginTS = 0.0_si_us;

        /*while (iState.test_and_set(std::memory_order_acquire))
        {
            beginTS = GpDateTimeOps::SHighResTS_us();
            GP_ASM_SPIN_PAUSE();
        }*/

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

        if (beginTS > 0.0_si_us)
        {
            const microseconds_t endTS = GpDateTimeOps::SHighResTS_us();
            const microseconds_t delta = endTS - beginTS;

            if (delta > 0.01_si_s)
            {
                std::cout << "GpSpinlock delta: " << delta.Value() << std::endl;
            }
        }
    }

    void    unlock      (void) noexcept
    {
        //iState.clear(std::memory_order_release);
        iState.store(false, std::memory_order_release);
    }

    bool    try_lock    (void) noexcept
    {
        //return (iState.test_and_set(std::memory_order_acquire) == false);
        return     (!iState.load(std::memory_order_relaxed))
                && (!iState.exchange(true, std::memory_order_acquire));
    }

private:
    //std::atomic_flag  iState;
    std::atomic<bool>   iState = {0};
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
