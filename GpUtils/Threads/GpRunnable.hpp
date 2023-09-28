#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../SyncPrimitives/GpConditionVarFlag.hpp"

namespace GPlatform {

class GpRunnable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpRunnable)
    CLASS_DD(GpRunnable)

protected:
    inline                      GpRunnable  (void) noexcept = default;

public:
    virtual                     ~GpRunnable (void) noexcept = default;

    GpConditionVarFlag&         CVF         (void) noexcept {return iCVF;}
    const GpConditionVarFlag&   CVF         (void) const noexcept{return iCVF;}
    virtual void                Run         (std::atomic_flag& aStopRequest) noexcept = 0;

private:
    GpConditionVarFlag          iCVF;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
