#include "GpDateTimeOps.hpp"

#if defined(GP_USE_DATE_TIME)

#include <chrono>

namespace GPlatform {

unix_ts_ms_t    GpDateTimeOps::SUnixTS_ms (void) noexcept
{
    const auto val = std::chrono::system_clock::now().time_since_epoch();
    const auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(val).count();

    return unix_ts_s_t::SMake(cnt);
}

unix_ts_s_t GpDateTimeOps::SUnixTS_s (void) noexcept
{
    return SUnixTS_ms();
}

milliseconds_t  GpDateTimeOps::SSteadyTS_ms (void) noexcept
{
    const auto val = std::chrono::steady_clock::now().time_since_epoch();
    const auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(val).count();

    return milliseconds_t::SMake(milliseconds_t::value_type(cnt));
}

seconds_t   GpDateTimeOps::SSteadyTS_s (void) noexcept
{
    return SSteadyTS_ms();
}

microseconds_t  GpDateTimeOps::SHighResTS_us (void) noexcept
{
    const auto val = std::chrono::high_resolution_clock::now().time_since_epoch();
    const auto cnt = std::chrono::duration_cast<std::chrono::microseconds>(val).count();

    return microseconds_t::SMake(microseconds_t::value_type(cnt));
}

}//GPlatform

#endif//#if defined(GP_USE_DATE_TIME)