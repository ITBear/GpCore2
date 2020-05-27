#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_DATE_TIME)

#include "../../GpCore_global.hpp"
#include "../Units/SI/GpUnitsSI_Time.hpp"
#include "../Units/Other/unix_ts_t.hpp"

namespace GPlatform {

class GPCORE_API GpDateTimeOps
{
	CLASS_REMOVE_CTRS(GpDateTimeOps);

public:
	[[nodiscard]] static unix_ts_ms_t		SUnixTS_ms		(void) noexcept;
	[[nodiscard]] static unix_ts_s_t		SUnixTS_s		(void) noexcept;

	[[nodiscard]] static milliseconds_t		SSteadyTS_ms	(void) noexcept;
	[[nodiscard]] static seconds_t			SSteadyTS_s		(void) noexcept;

	[[nodiscard]] static microseconds_t		SHighResTS_us	(void) noexcept;
};

}//GPlatform

#endif//#if defined(GP_USE_DATE_TIME)