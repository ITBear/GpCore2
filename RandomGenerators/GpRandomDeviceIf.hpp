#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

#include "../GpCore_global.hpp"
#include "../Types/Numerics/GpNumericOps.hpp"
#include "../Types/Classes/GpClassesDefines.hpp"
#include <random>

namespace GPlatform {

class GPCORE_API GpRandomDeviceIf
{
	CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpRandomDeviceIf);

public:
	using result_type = std::random_device::result_type;

public:
									GpRandomDeviceIf	(void) noexcept = default;
	virtual							~GpRandomDeviceIf	(void) noexcept = default;

	virtual	result_type				operator()			(void) = 0;

	static constexpr result_type	min					(void) noexcept
	{
		return NumOps::SMin<result_type>();
	}

	static constexpr result_type	max					(void) noexcept
	{
		return NumOps::SMax<result_type>();
	}
};

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)

