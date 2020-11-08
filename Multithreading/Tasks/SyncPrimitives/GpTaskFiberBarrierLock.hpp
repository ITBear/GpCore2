#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiberBarrier.hpp"

namespace GPlatform {

class GPCORE_API GpTaskFiberBarrierLock
{
public:
                            GpTaskFiberBarrierLock  (void) noexcept = delete;
                            GpTaskFiberBarrierLock  (const GpTaskFiberBarrierLock&) noexcept = delete;

public:
    inline                  GpTaskFiberBarrierLock  (GpTaskFiberBarrierLock&& aLock) noexcept;
    inline                  GpTaskFiberBarrierLock  (GpTaskFiberBarrier::SP aBarrier) noexcept;
    inline                  ~GpTaskFiberBarrierLock (void) noexcept;

    inline void             Release                 (void);

private:
    GpTaskFiberBarrier::SP  iBarrier;
};

GpTaskFiberBarrierLock::GpTaskFiberBarrierLock (GpTaskFiberBarrierLock&& aLock) noexcept:
iBarrier(std::move(aLock.iBarrier))
{
}

GpTaskFiberBarrierLock::GpTaskFiberBarrierLock (GpTaskFiberBarrier::SP aBarrier) noexcept:
iBarrier(std::move(aBarrier))
{
    iBarrier->Acquire();
}

GpTaskFiberBarrierLock::~GpTaskFiberBarrierLock (void) noexcept
{
    Release();
}

void    GpTaskFiberBarrierLock::Release (void)
{
    if (iBarrier.IsNotNULL())
    {
        iBarrier.Vn().Release();
        iBarrier.Clear();
    }
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
