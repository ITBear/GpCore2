#include "GpRandom.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

#include <functional>

#   if defined(GP_OS_WINDOWS)
#       include "GpRandomDeviceWin.hpp"
#   endif//if defined(GP_OS_WINDOWS)

namespace GPlatform {

GpRandom::GpRandom (void) noexcept
{
}

GpRandom::~GpRandom (void) noexcept
{
}

void	GpRandom::SetSeedFromRD (void)
{
#if defined(GP_OS_WINDOWS)
	GpRandomDeviceWin rd;
#else
	std::random_device rd;
#endif

	std::array<random_mt19937::result_type, random_mt19937::state_size> seed_data;
	std::generate_n(seed_data.data(), seed_data.size(), std::ref(rd));
	std::seed_seq seq(std::begin(seed_data), std::end(seed_data));
	iEngine.seed(seq);
}

void	GpRandom::SetSeed (const random_mt19937::result_type aSeed)
{
	iEngine.seed(aSeed);
}

SInt8	GpRandom::SI8 (const SInt8 aMin, const SInt8 aMax)
{
	return Next<SInt8>(aMin, aMax);
}

UInt8	GpRandom::UI8 (const UInt8 aMin, const UInt8 aMax)
{
	return Next<UInt8>(aMin, aMax);
}

SInt16	GpRandom::SI16 (const SInt16 aMin, const SInt16 aMax)
{
	return Next<SInt16>(aMin, aMax);
}

UInt16	GpRandom::UI16 (const UInt16 aMin, const UInt16 aMax)
{
	return Next<UInt16>(aMin, aMax);
}

SInt32	GpRandom::SI32 (const SInt32 aMin, const SInt32 aMax)
{
	return Next<SInt32>(aMin, aMax);
}

UInt32	GpRandom::UI32 (const UInt32 aMin, const UInt32 aMax)
{
	return Next<UInt32>(aMin, aMax);
}

SInt64	GpRandom::SI64 (const SInt64 aMin, const SInt64 aMax)
{
	return Next<SInt64>(aMin, aMax);
}

UInt64	GpRandom::UI64 (const UInt64 aMin, const UInt64 aMax)
{
	return Next<UInt64>(aMin, aMax);
}

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
