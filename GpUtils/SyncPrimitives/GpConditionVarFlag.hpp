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

    inline void                 UpFlagAndNotifyOne  (void) noexcept;
    inline void                 UpFlagAndNotifyOne  (const FnT& aFn) noexcept;
    inline void                 UpFlagAndNotifyAll  (void) noexcept;
    inline void                 UpFlagAndNotifyAll  (const FnT& aFn) noexcept;
    inline void                 NotifyOne           (void) noexcept;
    inline void                 NotifyAll           (void) noexcept;

    [[nodiscard]] inline bool   Test                (void) const noexcept;
    inline void                 Wait                (void) noexcept;
    [[nodiscard]] inline bool   WaitFor             (const milliseconds_t aTimeout) noexcept;

private:
    mutable GpConditionVar  iCV;
    bool                    iFlag GUARDED_BY(iCV.Mutex()) = false;
};

void    GpConditionVarFlag::UpFlagAndNotifyOne (void) noexcept
{
    GpUniqueLock uniqueLock{iCV.Mutex()};

    iFlag = true;

    iCV.NotifyOne();
}

void    GpConditionVarFlag::UpFlagAndNotifyOne (const FnT& aFn) noexcept
{
    GpUniqueLock uniqueLock{iCV.Mutex()};

    iFlag = true;
    aFn();

    iCV.NotifyOne();
}

void    GpConditionVarFlag::UpFlagAndNotifyAll (void) noexcept
{
    GpUniqueLock uniqueLock{iCV.Mutex()};

    iFlag = true;

    iCV.NotifyOne();
}

void    GpConditionVarFlag::UpFlagAndNotifyAll (const FnT& aFn) noexcept
{
    GpUniqueLock uniqueLock{iCV.Mutex()};

    iFlag = true;
    aFn();

    iCV.NotifyOne();
}

void    GpConditionVarFlag::NotifyOne (void) noexcept
{
    GpUniqueLock uniqueLock{iCV.Mutex()};

    iCV.NotifyOne();
}

void    GpConditionVarFlag::NotifyAll (void) noexcept
{
    GpUniqueLock uniqueLock{iCV.Mutex()};

    iCV.NotifyAll();
}

bool    GpConditionVarFlag::Test (void) const noexcept
{
    GpUniqueLock uniqueLock{iCV.Mutex()};

    return iFlag;
}

void    GpConditionVarFlag::Wait (void) noexcept
{
    iCV.Wait
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS {return iFlag;}
    );
}

bool    GpConditionVarFlag::WaitFor (const milliseconds_t aTimeout) noexcept
{
    return iCV.WaitFor
    (
        [&]() NO_THREAD_SAFETY_ANALYSIS {return iFlag;},
        aTimeout
    );
}

}// namespace GPlatform

#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
