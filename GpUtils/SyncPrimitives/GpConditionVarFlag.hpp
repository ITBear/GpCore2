#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)

#include <GpCore2/GpUtils/SyncPrimitives/GpConditionVar.hpp>

namespace GPlatform {

class GpConditionVarFlag
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpConditionVarFlag)
    CLASS_DD(GpConditionVarFlag)
    TAG_SET(THREAD_SAFE)

    using FnT = std::function<void()>;

public:
                    GpConditionVarFlag  (void) noexcept = default;
                    ~GpConditionVarFlag (void) noexcept = default;

    inline void     NotifyOne           (void) noexcept;
    inline void     NotifyOne           (const FnT& aFn) noexcept;
    inline void     NotifyAll           (void) noexcept;
    inline void     NotifyAll           (const FnT& aFn) noexcept;

    inline void     WaitAndReset        (void) noexcept;
    inline bool     WaitForAndReset     (const milliseconds_t aTimeout) noexcept;

private:
    mutable GpConditionVar  iCV;
    bool                    iFlag GUARDED_BY(iCV.Mutex()) = false;
};

void    GpConditionVarFlag::NotifyOne (void) noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iCV.Mutex()};

    iFlag = true;

    iCV.NotifyOne();
}

void    GpConditionVarFlag::NotifyOne (const FnT& aFn) noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iCV.Mutex()};

    iFlag = true;
    aFn();

    iCV.NotifyOne();
}

void    GpConditionVarFlag::NotifyAll (void) noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iCV.Mutex()};

    iFlag = true;

    iCV.NotifyOne();
}

void    GpConditionVarFlag::NotifyAll (const FnT& aFn) noexcept
{
    GpUniqueLock<GpMutex> uniqueLock{iCV.Mutex()};

    iFlag = true;
    aFn();

    iCV.NotifyOne();
}

void    GpConditionVarFlag::WaitAndReset (void) noexcept
{
    iCV.Wait
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS {return iFlag;}
    );
}

bool    GpConditionVarFlag::WaitForAndReset (const milliseconds_t aTimeout) noexcept
{
    return iCV.WaitFor
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS {return iFlag;},
        aTimeout
    );
}

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
