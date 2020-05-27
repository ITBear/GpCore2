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

s_int_8	GpRandom::SI8 (const s_int_8 aMin, const s_int_8 aMax)
{
	return Next<s_int_8>(aMin, aMax);
}

u_int_8	GpRandom::UI8 (const u_int_8 aMin, const u_int_8 aMax)
{
	return Next<u_int_8>(aMin, aMax);
}

s_int_16	GpRandom::SI16 (const s_int_16 aMin, const s_int_16 aMax)
{
	return Next<s_int_16>(aMin, aMax);
}

u_int_16	GpRandom::UI16 (const u_int_16 aMin, const u_int_16 aMax)
{
	return Next<u_int_16>(aMin, aMax);
}

s_int_32	GpRandom::SI32 (const s_int_32 aMin, const s_int_32 aMax)
{
	return Next<s_int_32>(aMin, aMax);
}

u_int_32	GpRandom::UI32 (const u_int_32 aMin, const u_int_32 aMax)
{
	return Next<u_int_32>(aMin, aMax);
}

s_int_64	GpRandom::SI64 (const s_int_64 aMin, const s_int_64 aMax)
{
	return Next<s_int_64>(aMin, aMax);
}

u_int_64	GpRandom::UI64 (const u_int_64 aMin, const u_int_64 aMax)
{
	return Next<u_int_64>(aMin, aMax);
}

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
