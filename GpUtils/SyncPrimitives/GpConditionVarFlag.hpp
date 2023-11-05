#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_SYNC_PRIMITIVES)

#include "GpConditionVar.hpp"

namespace GPlatform {

class GpConditionVarFlag
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpConditionVarFlag)
    CLASS_DD(GpConditionVarFlag)
    TAG_SET(THREAD_SAFE)

public:
                            GpConditionVarFlag  (void) noexcept = default;
                            ~GpConditionVarFlag (void) noexcept = default;

    inline void             NotifyOne           (void) noexcept;
    inline void             NotifyAll           (void) noexcept;

    inline void             WaitAndReset        (void) noexcept;
    inline bool             WaitForAndReset     (const milliseconds_t aTimeout) noexcept;

private:
    mutable GpConditionVar  iCV;
    bool                    iFlag GUARDED_BY(iCV.Mutex()) = false;
};

void    GpConditionVarFlag::NotifyOne (void) noexcept
{
    GpUniqueLock<GpMutex> lock(iCV.Mutex());

    iFlag = true;

    iCV.NotifyOne();
}

void    GpConditionVarFlag::NotifyAll (void) noexcept
{
    GpUniqueLock<GpMutex> lock(iCV.Mutex());

    iFlag = true;

    iCV.NotifyOne();
}

void    GpConditionVarFlag::WaitAndReset (void) noexcept
{
    iCV.Wait<bool>
    (
        []() {}, // at begin
        [&]() NO_THREAD_SAFETY_ANALYSIS {iFlag = false;}, // at end
        [&]() NO_THREAD_SAFETY_ANALYSIS {return iFlag;}, // check
        []() {return true;} // condition met
    );
}

bool    GpConditionVarFlag::WaitForAndReset (const milliseconds_t aTimeout) noexcept
{
    return iCV.WaitFor<bool>
    (
        []() {}, // at begin
        [&]() NO_THREAD_SAFETY_ANALYSIS {iFlag = false;}, // at end
        [&]() NO_THREAD_SAFETY_ANALYSIS {return iFlag;}, // check
        []() {return true;}, // condition met
        []() {return false;}, // condition not met
        aTimeout
    ).value();
}

}//GPlatform

#endif//#if defined(GP_USE_SYNC_PRIMITIVES)
