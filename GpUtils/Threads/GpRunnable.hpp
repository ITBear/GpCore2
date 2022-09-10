#pragma once

#include "../GpMacro.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../SyncPrimitives/GpConditionVar.hpp"
#include "GpThreadStopToken.hpp"

namespace GPlatform {

class GpRunnable
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpRunnable)
    CLASS_DECLARE_DEFAULTS(GpRunnable)

protected:
    inline                  GpRunnable      (GpConditionVar::SP aCondVar) noexcept;

public:
    virtual                 ~GpRunnable     (void) noexcept = default;

    void                    WakeupAll       (void) noexcept {iCondVar.Vn().WakeupAll();}
    void                    WakeupOne       (void) noexcept {iCondVar.Vn().WakeupOne();}
    [[nodiscard]] bool      WaitForWakeup   (void) noexcept {return iCondVar.Vn().WaitForWakeup();}
    [[nodiscard]] bool      WaitForWakeup   (const milliseconds_t aTimeout) noexcept {return iCondVar.Vn().WaitForWakeup(aTimeout);}

    virtual void            Run             (GpThreadStopToken aStopToken) noexcept = 0;

private:
    GpConditionVar::SP      iCondVar;
};

GpRunnable::GpRunnable (GpConditionVar::SP aCondVar) noexcept:
iCondVar(std::move(aCondVar))
{
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
