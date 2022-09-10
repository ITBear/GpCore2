#pragma once

#include "../GpTasks_global.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberStackPool.hpp"

namespace GPlatform {

class GpTaskFiberManager
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFiberManager)

private:
                                        GpTaskFiberManager  (void) noexcept = default;

public:
                                        ~GpTaskFiberManager (void) noexcept = default;

    static GpTaskFiberManager&          S                   (void) noexcept {return sManager;}

    inline void                         Init                (const size_t       aMaxStacksCount,
                                                             const size_byte_t  aStackSize);
    inline void                         Clear               (void) noexcept;

    inline GpTaskFiberStack::C::Opt::SP Acquire             (void);
    inline void                         Release             (GpTaskFiberStack::SP&& aFiberStack);

private:
    GpTaskFiberStackPool                iStackPool;

    static GpTaskFiberManager           sManager;
};

void    GpTaskFiberManager::Init
(
    const size_t        aMaxStacksCount,
    const size_byte_t   aStackSize
)
{
    iStackPool.SetStackSize(aStackSize);
    iStackPool.Init(0, aMaxStacksCount);
}

void    GpTaskFiberManager::Clear (void) noexcept
{
    iStackPool.Clear();
}

GpTaskFiberStack::C::Opt::SP    GpTaskFiberManager::Acquire (void)
{
    return iStackPool.Acquire();
}

void    GpTaskFiberManager::Release (GpTaskFiberStack::SP&& aFiberStack)
{
    iStackPool.Release(std::move(aFiberStack));
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
