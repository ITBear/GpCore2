#include <GpCore2/GpUtils/Debugging/GpSourceRegistry.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSyncPrimitives.hpp>

namespace GPlatform {

GpSourceRegistry::GpSourceRegistry (void) noexcept
{
}

GpSourceRegistry::~GpSourceRegistry (void) noexcept
{
}

GpSourceRegistry&   GpSourceRegistry::S (void)
{
    static GpSourceRegistry sInstance;
    return sInstance;
}

u_int_64    GpSourceRegistry::RegisterLocation
(
    const u_int_64          aKey,
    const SourceLocationT&  aSourceLocation
)
{
    {
        GpSharedLock sharedLock{iSpinLockRW};

        if (iLocations.contains(aKey)) [[likely]]
        {
            return aKey;
        }
    }

    {
        GpUniqueLock uniqueLock{iSpinLockRW};
        iLocations.try_emplace(aKey, aSourceLocation);
    }

    return aKey;
}

}// namespace GPlatform
