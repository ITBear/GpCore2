#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_SYNC_PRIMITIVES)
#if defined(GP_LOCK_TRACE)

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>
#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>
#include <GpCore2/GpUtils/Types/Units/Other/unix_ts_t.hpp>

namespace GPlatform {

class GP_UTILS_API GpLockTrace
{
public:
    using SyncPrimitiveIdT  = void*;
    using LockIdT           = void*;

    struct LockInfo
    {
        unix_ts_ms_t    iBeginLockTS;
        bool            iIsShared;
    };

    using LocksInfoT = boost::container::flat_map<SyncPrimitiveIdT, boost::container::small_flat_map<LockIdT, LockInfo, 8>>;

public:
                        GpLockTrace     (void) noexcept;
                        ~GpLockTrace    (void) noexcept;

    static GpLockTrace& S               (void) noexcept {return sInstance;}

    void                OnLock          (SyncPrimitiveIdT   aPrimitiveId,
                                         LockIdT            aLockId);   
    void                OnLockShared    (SyncPrimitiveIdT   aPrimitiveId,
                                         LockIdT            aLockId);
    void                OnUnlock        (SyncPrimitiveIdT   aPrimitiveId,
                                         LockIdT            aLockId);

    void                Check           (unix_ts_ms_t aMaxLockDuration) const noexcept;

private:
    LocksInfoT          iLocksInfo;

    static GpLockTrace  sInstance;
};

}// namespace GPlatform

#endif// #if defined(GP_LOCK_TRACE)
#endif// #if defined(GP_USE_SYNC_PRIMITIVES)
