#include <GpCore2/GpUtils/SyncPrimitives/GpLockTrace.hpp>
#include <GpCore2/GpUtils/DateTime/GpDateTimeOps.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)
#if defined(GP_LOCK_TRACE)

#include <GpCore2/GpUtils/SyncPrimitives/GpSyncPrimitives.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLock.hpp>

namespace GPlatform {

GpLockTrace GpLockTrace::sInstance;

static GpSpinLock<ThreadSafety::LockTraceModeE::TRACE_DISABLED> sGpLockTrace_SpinLock;

GpLockTrace::GpLockTrace (void) noexcept
{
}

GpLockTrace::~GpLockTrace (void) noexcept
{
}

void    GpLockTrace::OnLock
(
    const SyncPrimitiveIdT  aPrimitiveId,
    const LockIdT           aLockId
)
{
    GpUniqueLock uniqueLock{sGpLockTrace_SpinLock};

    auto info = iLocksInfo.try_emplace(aPrimitiveId).first->second;

    info.try_emplace
    (
        aLockId,
        LockInfo
        {
            .iBeginLockTS   = GpDateTimeOps::SUnixTS_ms(),
            .iIsShared      = false
        }
    );
}

void    GpLockTrace::OnLockShared
(
    const SyncPrimitiveIdT  aPrimitiveId,
    const LockIdT           aLockId
)
{
    GpUniqueLock uniqueLock{sGpLockTrace_SpinLock};

    auto info = iLocksInfo.try_emplace(aPrimitiveId).first->second;

    info.try_emplace
    (
        aLockId,
        LockInfo
        {
            .iBeginLockTS   = GpDateTimeOps::SUnixTS_ms(),
            .iIsShared      = true
        }
    );
}

void    GpLockTrace::OnUnlock
(
    const SyncPrimitiveIdT  aPrimitiveId,
    const LockIdT           aLockId
)
{
    GpUniqueLock uniqueLock{sGpLockTrace_SpinLock};

    auto info = iLocksInfo.try_emplace(aPrimitiveId).first->second;
    info.erase(aLockId);
}

void    GpLockTrace::Check (const unix_ts_ms_t aMaxLockDuration) const noexcept
{
    LocksInfoT locksInfo;

    {
        GpUniqueLock uniqueLock{sGpLockTrace_SpinLock};
        locksInfo = iLocksInfo;
    }

    const auto nowTs = GpDateTimeOps::SUnixTS_ms();

    for (const auto&[syncPrimitiveId, locks]: locksInfo)
    {
        for (const auto& [lockId, info]: locks)
        {
            const auto lockDuration = nowTs - info.iBeginLockTS;

            if (lockDuration > aMaxLockDuration)
            {
                GpOutUtils::S().StdErr
                (
                    fmt::format
                    (
                        "[GpLockTrace::Check]: Max lock duration {} ms < lock time {} ms for sync primitive {} locked by {}",
                        aMaxLockDuration,
                        lockDuration,
                        syncPrimitiveId,
                        lockId
                    )
                );
            }
        }

    }
}

}// namespace GPlatform

#endif// #if defined(GP_LOCK_TRACE)
#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
