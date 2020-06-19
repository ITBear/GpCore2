#include "GpSRandom.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

#include <mutex>

namespace GPlatform {

GpSRandom::GpSRandom (void) noexcept
{
}

GpSRandom::~GpSRandom (void) noexcept
{
}

GpSRandom&  GpSRandom::S (void) noexcept
{
    static GpSRandom sSRnd;
    return sSRnd;
}

void    GpSRandom::SetSeedFromRD (void)
{
    std::scoped_lock l(iLock);
    iRandom.SetSeedFromRD();
}

void    GpSRandom::SetSeed (const random_mt19937::result_type aSeed)
{
    std::scoped_lock l(iLock);
    iRandom.SetSeed(aSeed);
}

SInt8   GpSRandom::SI8 (const SInt8 aMin, const SInt8 aMax)
{
    std::scoped_lock l(iLock);
    return iRandom.SI8(aMin, aMax);
}

UInt8   GpSRandom::UI8 (const UInt8 aMin, const UInt8 aMax)
{
    std::scoped_lock l(iLock);
    return iRandom.UI8(aMin, aMax);
}

SInt16  GpSRandom::SI16 (const SInt16 aMin, const SInt16 aMax)
{
    std::scoped_lock l(iLock);
    return iRandom.SI16(aMin, aMax);
}

UInt16  GpSRandom::UI16 (const UInt16 aMin, const UInt16 aMax)
{
    std::scoped_lock l(iLock);
    return iRandom.UI16(aMin, aMax);
}

SInt32  GpSRandom::SI32 (const SInt32 aMin, const SInt32 aMax)
{
    std::scoped_lock l(iLock);
    return iRandom.SI32(aMin, aMax);
}

UInt32  GpSRandom::UI32 (const UInt32 aMin, const UInt32 aMax)
{
    std::scoped_lock l(iLock);
    return iRandom.UI32(aMin, aMax);
}

SInt64  GpSRandom::SI64 (const SInt64 aMin, const SInt64 aMax)
{
    std::scoped_lock l(iLock);
    return iRandom.SI64(aMin, aMax);
}

UInt64  GpSRandom::UI64 (const UInt64 aMin, const UInt64 aMax)
{
    std::scoped_lock l(iLock);
    return iRandom.UI64(aMin, aMax);
}

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
