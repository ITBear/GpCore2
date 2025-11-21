#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpConditionVarFlag.hpp>

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

class GpRunnable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpRunnable)
    CLASS_DD(GpRunnable)

protected:
                    GpRunnable  (void) noexcept = default;

public:
    virtual         ~GpRunnable (void) noexcept = default;

    virtual void    Run         (GpConditionVarFlag& aStopFlag) noexcept = 0;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING)
