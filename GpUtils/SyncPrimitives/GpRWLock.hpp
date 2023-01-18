#pragma once

#include "../GpMacro.hpp"

#if defined(GP_USE_SYNC_PRIMITIVES)

#include <atomic>
#include "GpAsmSpinPause.hpp"

namespace GPlatform{

class GpRWLock
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpRWLock)

public:
    GpRWLock (void) noexcept :iReadersCount(0), iWritersCount(0)
    {}

    void    lock_shared (void) noexcept
    {
        while(1)
        {
            //Wait until there no witers will be
            if (iWritersCount.load(std::memory_order_acquire) != 0)
            {
                GP_ASM_SPIN_PAUSE();
                continue;
            }

            //Add to the readers
            iReadersCount.fetch_add(1, std::memory_order_release);

            // Check for writers again
            if (iWritersCount.load(std::memory_order_acquire) == 0)
            {
                //Enter to shared lock
                break;
            }

            //Sub from the readers and try again
            iReadersCount.fetch_sub(1, std::memory_order_release);
            GP_ASM_SPIN_PAUSE();
        }
    }

    void    unlock_shared (void) noexcept
    {
        iReadersCount.fetch_sub(1, std::memory_order_release);
    }

    void    lock (void) noexcept
    {
        //Try to lock for write
        while (iWritersCount.exchange(1) == 1)
        {
            GP_ASM_SPIN_PAUSE();
        }

        //Wait for readers
        while (iReadersCount.load(std::memory_order_acquire) != 0)
        {
            GP_ASM_SPIN_PAUSE();
        }
    }

    void    unlock (void) noexcept
    {
        iWritersCount.store(0, std::memory_order_release);
    }

private:
    std::atomic<size_t> iReadersCount;
    std::atomic<size_t> iWritersCount;
};

}//GPlatform

#endif//#if defined(GP_USE_SYNC_PRIMITIVES)
