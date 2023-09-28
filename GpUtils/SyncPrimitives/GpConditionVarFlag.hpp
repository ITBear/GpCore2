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

    using AtomicFnT = GpConditionVar::AtomicFnT;

public:
                        GpConditionVarFlag  (void) noexcept = default;
                        ~GpConditionVarFlag (void) noexcept = default;

    inline void         DoAtomic            (const AtomicFnT& aFn) const;
    inline void         NotifyAll           (void);
    inline void         NotifyOne           (void);
    inline void         Wait                (void);
    inline bool         WaitFor             (const milliseconds_t aTimeout);
    inline bool         WaitForAndReset     (const milliseconds_t aTimeout);

private:
    GpConditionVar      iCV;
    bool                iFlag = false;
};

void    GpConditionVarFlag::DoAtomic (const AtomicFnT& aFn) const
{
    iCV.DoAtomic(aFn);
}

void    GpConditionVarFlag::NotifyAll (void)
{
    iCV.DoAtomic
    (
        [&](std::mutex&)
        {
            iFlag = true;
        }
    );

    iCV.NotifyAll();
}

void    GpConditionVarFlag::NotifyOne (void)
{
    iCV.DoAtomic
    (
        [&](std::mutex&)
        {
            iFlag = true;
        }
    );

    iCV.NotifyOne();
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

bool    GpConditionVarFlag::WaitFor (const milliseconds_t aTimeout)
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

bool    GpConditionVarFlag::WaitForAndReset (const milliseconds_t aTimeout)
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
