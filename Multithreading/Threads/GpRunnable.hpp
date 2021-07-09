#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../SyncPrimitives/GpConditionVar.hpp"
#include "GpThreadStopToken.hpp"

namespace GPlatform {

class GPCORE_API GpRunnable
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpRunnable)
    CLASS_DECLARE_DEFAULTS(GpRunnable)

protected:
                            GpRunnable      (GpConditionVar::SP aCondVar) noexcept;

public:
    virtual                 ~GpRunnable     (void) noexcept;

    void                    WakeupAll       (void) noexcept {iCondVar.Vn().WakeupAll();}
    void                    WakeupOne       (void) noexcept {iCondVar.Vn().WakeupOne();}
    void                    WaitForWakeup   (void) noexcept {iCondVar.Vn().WaitForWakeup();}
    void                    WaitForWakeup   (const milliseconds_t aTimeout) noexcept {iCondVar.Vn().WaitForWakeup(aTimeout);}

    virtual void            Run             (GpThreadStopToken aStopToken) noexcept = 0;

private:
    GpConditionVar::SP      iCondVar;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
