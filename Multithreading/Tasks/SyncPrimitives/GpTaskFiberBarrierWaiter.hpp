#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberBarrier.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFiberBarrierWaiter
{
public:
                            GpTaskFiberBarrierWaiter    (void) noexcept = delete;
                            GpTaskFiberBarrierWaiter    (const GpTaskFiberBarrierWaiter&) noexcept = delete;

public:
    inline                  GpTaskFiberBarrierWaiter    (GpTaskFiberBarrierWaiter&& aWaiter) noexcept;
    inline                  GpTaskFiberBarrierWaiter    (GpTaskFiberBarrier::SP aBarrier) noexcept;
    inline                  ~GpTaskFiberBarrierWaiter   (void) noexcept;

    inline void             Wait                        (GpTaskFiber::WP aTask);

private:
    GpTaskFiberBarrier::SP  iBarrier;
};

GpTaskFiberBarrierWaiter::GpTaskFiberBarrierWaiter (GpTaskFiberBarrierWaiter&& aWaiter) noexcept:
iBarrier(std::move(aWaiter.iBarrier))
{
}

GpTaskFiberBarrierWaiter::GpTaskFiberBarrierWaiter (GpTaskFiberBarrier::SP aBarrier) noexcept:
iBarrier(std::move(aBarrier))
{
}

GpTaskFiberBarrierWaiter::~GpTaskFiberBarrierWaiter (void) noexcept
{
}

void    GpTaskFiberBarrierWaiter::Wait (GpTaskFiber::WP aTask)
{
    iBarrier->Wait(std::move(aTask));
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
