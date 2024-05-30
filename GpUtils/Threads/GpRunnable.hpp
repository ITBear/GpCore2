#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpConditionVarFlag.hpp>

namespace GPlatform {

class GpRunnable
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpRunnable)
    CLASS_DD(GpRunnable)

protected:
                                GpRunnable      (void) noexcept = default;

public:
    virtual                     ~GpRunnable     (void) noexcept = default;

    virtual void                Run             (std::atomic_flag& aStopRequest) noexcept = 0;

    inline void                 Notify          (void) noexcept;
    inline bool                 WaitForAndReset (const milliseconds_t aTimeout) noexcept;

private:
    mutable GpConditionVarFlag  iCVF;
};

void    GpRunnable::Notify (void) noexcept
{
    iCVF.NotifyAll();
}

bool    GpRunnable::WaitForAndReset (const milliseconds_t aTimeout) noexcept
{
    return iCVF.WaitForAndReset(aTimeout);
}

}// namespace GPlatform
