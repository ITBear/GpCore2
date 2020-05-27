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

GpSRandom&	GpSRandom::S (void) noexcept
{
	static GpSRandom sSRnd;
	return sSRnd;
}

void	GpSRandom::SetSeedFromRD (void)
{
	std::scoped_lock l(iLock);
	iRandom.SetSeedFromRD();
}

void	GpSRandom::SetSeed (const random_mt19937::result_type aSeed)
{
	std::scoped_lock l(iLock);
	iRandom.SetSeed(aSeed);
}

s_int_8	GpSRandom::SI8 (const s_int_8 aMin, const s_int_8 aMax)
{
	std::scoped_lock l(iLock);
	return iRandom.SI8(aMin, aMax);
}

u_int_8	GpSRandom::UI8 (const u_int_8 aMin, const u_int_8 aMax)
{
	std::scoped_lock l(iLock);
	return iRandom.UI8(aMin, aMax);
}

s_int_16	GpSRandom::SI16 (const s_int_16 aMin, const s_int_16 aMax)
{
	std::scoped_lock l(iLock);
	return iRandom.SI16(aMin, aMax);
}

u_int_16	GpSRandom::UI16 (const u_int_16 aMin, const u_int_16 aMax)
{
	std::scoped_lock l(iLock);
	return iRandom.UI16(aMin, aMax);
}

s_int_32	GpSRandom::SI32 (const s_int_32 aMin, const s_int_32 aMax)
{
	std::scoped_lock l(iLock);
	return iRandom.SI32(aMin, aMax);
}

u_int_32	GpSRandom::UI32 (const u_int_32 aMin, const u_int_32 aMax)
{
	std::scoped_lock l(iLock);
	return iRandom.UI32(aMin, aMax);
}

s_int_64	GpSRandom::SI64 (const s_int_64 aMin, const s_int_64 aMax)
{
	std::scoped_lock l(iLock);
	return iRandom.SI64(aMin, aMax);
}

u_int_64	GpSRandom::UI64 (const u_int_64 aMin, const u_int_64 aMax)
{
	std::scoped_lock l(iLock);
	return iRandom.UI64(aMin, aMax);
}

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
