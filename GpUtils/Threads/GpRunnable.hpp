#pragma once

#include "../GpMacro.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "../SyncPrimitives/GpConditionVarFlag.hpp"
#include "GpThreadStopToken.hpp"

namespace GPlatform {

class GpRunnable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpRunnable)
    CLASS_DD(GpRunnable)

    using WaitForResT = GpConditionVar::WaitForResT;

protected:
    inline                              GpRunnable      (void) noexcept = default;

public:
    virtual                             ~GpRunnable     (void) noexcept = default;

    GpConditionVarFlag&                 CVF             (void) {return iCVF;}

public:
    virtual void                        Run             (GpThreadStopToken aStopToken) noexcept = 0;

private:
    GpConditionVarFlag                  iCVF;
};

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
