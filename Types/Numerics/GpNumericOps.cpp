#include "GpNumericOps.hpp"

namespace GPlatform{

size_t	GpNumericOps::SDecDigsCountUI64 (const u_int_64 aValue) noexcept
{
	return SDecDigsCount<u_int_64>(aValue);
}

size_t	GpNumericOps::SDecDigsCountSI64 (const s_int_64 aValue) noexcept
{
	return SDecDigsCount<s_int_64>(aValue);
}

}//GPlatform