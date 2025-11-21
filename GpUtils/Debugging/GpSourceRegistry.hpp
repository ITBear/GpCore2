#pragma once

#include <GpCore2/GpUtils/Debugging/GpSourceLocation.hpp>
#include <GpCore2/Config/IncludeExt/unordered_dense.hpp>
#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLockRW.hpp>
#include <GpCore2/GpUtils/Other/GpUidKeyLine.hpp>

namespace GPlatform {

class GpSourceRegistry
{
public:
    using LocationsT                = ankerl::unordered_dense::map<u_int_64, SourceLocationT>;
    using SourceLocationOptCRefT    = std::optional<std::reference_wrapper<const SourceLocationT>>;
    using SpinLockRwT               = GpSpinLockRW<ThreadSafety::LockTraceModeE::TRACE_DISABLED>;

public:
                                    GpSourceRegistry    (void) noexcept;
                                    ~GpSourceRegistry   (void) noexcept;

    static GpSourceRegistry&        S                   (void);

    template<u_int_64 Key>
    u_int_64                        RegisterLocation    (const SourceLocationT& aSourceLocation = SourceLocationT::current());
    const SourceLocationOptCRefT    Find                (u_int_64 aKey) const noexcept;

private:
    u_int_64                        RegisterLocation    (u_int_64 aKey, const SourceLocationT&  aSourceLocation);

private:
    mutable SpinLockRwT iSpinLockRW;
    LocationsT          iLocations GUARDED_BY(iSpinLockRW);
};

template<u_int_64 Key>
u_int_64    GpSourceRegistry::RegisterLocation (const SourceLocationT& aSourceLocation)
{
    return RegisterLocation(Key, aSourceLocation);
}

} // namespace GPlatform

#define SRC_REGISTER_LINE() ::GPlatform::GpSourceRegistry::S().RegisterLocation<UID_KEY_LINE()>()
