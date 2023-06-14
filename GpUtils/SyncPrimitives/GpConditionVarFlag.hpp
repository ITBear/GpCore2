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

    using WaitForResT = GpConditionVar::WaitForResT;

public:
                                        GpConditionVarFlag  (void) noexcept = default;
                                        ~GpConditionVarFlag (void) noexcept = default;

    inline void                         Reset               (void);
    inline void                         WakeupAll           (void);
    inline void                         WakeupOne           (void);
    inline void                         Wait                (void);
    [[nodiscard]] inline WaitForResT    WaitFor             (const milliseconds_t aTimeout);
    [[nodiscard]] inline WaitForResT    WaitForAndReset     (const milliseconds_t aTimeout);

private:
    bool                                iFlag = false;
    GpConditionVar                      iCV;
};

void    GpConditionVarFlag::Reset (void)
{
    iCV.Do
    (
        [&]()
        {
            iFlag = false;
        }
    );
}

void    GpConditionVarFlag::WakeupAll (void)
{
    iCV.WakeupAll
    (
        [&]()
        {
            iFlag = true;
        }
    );
}

void    GpConditionVarFlag::WakeupOne (void)
{
    iCV.WakeupOne
    (
        [&]()
        {
            iFlag = true;
        }
    );
}

void    GpConditionVarFlag::Wait (void)
{
    iCV.Wait
    (
        [&]()
        {
            return iFlag;
        }
    );
}

GpConditionVarFlag::WaitForResT GpConditionVarFlag::WaitFor (const milliseconds_t aTimeout)
{
    return iCV.WaitFor
    (
        [&]()
        {
            return iFlag;
        },
        aTimeout
    );
}

GpConditionVarFlag::WaitForResT GpConditionVarFlag::WaitForAndReset (const milliseconds_t aTimeout)
{
    return iCV.WaitFor
    (
        [&]()
        {
            if (iFlag)
            {
                iFlag = false;
                return true;
            } else
            {
                return false;
            }
        },
        aTimeout
    );
}

}//GPlatform

#endif//#if defined(GP_USE_SYNC_PRIMITIVES)
