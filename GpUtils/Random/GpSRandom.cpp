#include "GpSRandom.hpp"
#include "../SyncPrimitives/GpMutex.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

namespace GPlatform {

GpSRandom   GpSRandom::sInstance;

GpSRandom::GpSRandom (void) noexcept
{
}

GpSRandom::~GpSRandom (void) noexcept
{
}

void    GpSRandom::SetSeedFromRD (void)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    iRandom.SetSeedFromRD();
}

void    GpSRandom::SetSeed (const random_mt19937::result_type aSeed)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    iRandom.SetSeed(aSeed);
}

s_int_8 GpSRandom::SI8 (const s_int_8 aMin, const s_int_8 aMax) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.SI8(aMin, aMax);
}

u_int_8 GpSRandom::UI8 (const u_int_8 aMin, const u_int_8 aMax) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.UI8(aMin, aMax);
}

s_int_16    GpSRandom::SI16 (const s_int_16 aMin, const s_int_16 aMax) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.SI16(aMin, aMax);
}

u_int_16    GpSRandom::UI16 (const u_int_16 aMin, const u_int_16 aMax) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.UI16(aMin, aMax);
}

s_int_32    GpSRandom::SI32 (const s_int_32 aMin, const s_int_32 aMax) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.SI32(aMin, aMax);
}

u_int_32    GpSRandom::UI32 (const u_int_32 aMin, const u_int_32 aMax) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.UI32(aMin, aMax);
}

s_int_64    GpSRandom::SI64 (const s_int_64 aMin, const s_int_64 aMax) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.SI64(aMin, aMax);
}

u_int_64    GpSRandom::UI64 (const u_int_64 aMin, const u_int_64 aMax) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.UI64(aMin, aMax);
}

ssize_t GpSRandom::SSizeT (const ssize_t aMin, const ssize_t aMax) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.SSizeT(aMin, aMax);
}

size_t  GpSRandom::SizeT (const size_t aMin, const size_t aMax) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.SizeT(aMin, aMax);
}

bool    GpSRandom::Bool (void) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.Bool();
}

double  GpSRandom::Double
(
    const double    aMin,
    const double    aMax
) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.Double(aMin, aMax);
}

float   GpSRandom::Float
(
    const float aMin,
    const float aMax
) noexcept
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.Float(aMin, aMax);
}

std::string GpSRandom::String
(
    const GpRandomStrMode::EnumT    aMode,
    const size_t                    aSize
)
{
    GpUniqueLock<GpSpinLock> uniqueLock{iSpinLock};
    return iRandom.String(aMode, aSize);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_RANDOM_GENERATORS)
