#include "GpDateTimeOps.hpp"

#if defined(GP_USE_DATE_TIME)

// TODO: remove #include <date/date.h> with c++20
GP_WARNING_PUSH()

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    GP_WARNING_DISABLE(unknown-warning-option)
#endif// #if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)

#   include <date/date.h>

GP_WARNING_POP()

#include <sstream>
#include <chrono>

namespace GPlatform {

const microseconds_t    GpDateTimeOps::sStartSteadyTSus = GpDateTimeOps::SSteadyTS_us();
const milliseconds_t    GpDateTimeOps::sStartSteadyTSms = GpDateTimeOps::SSteadyTS_ms();

const std::array<std::string, GpDateTimeFormat::SCount()> GpDateTimeOps::sFormats =
{
    "%FT%X+00:00",              // ISO_8601:        2021-01-11T20:15:31+00:00
    "%a, %d %b %Y %X +0000",    // RFC_2822:        Mon, 11 Jan 2021 20:15:31 +0000
    "%F %X",                    // STD_DATE_TIME:   2021-01-11 20:15:31
    "%FT%X",                    // STD_DATE_TIME_T: 2021-01-11T20:15:31
    "%F",                       // STD_DATE:        2021-01-11
    "%X"                        // STD_TIME:        20:15:31
};

unix_ts_ms_t    GpDateTimeOps::SUnixTS_ms (void) noexcept
{
    const auto val = std::chrono::system_clock::now().time_since_epoch();
    const auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(val).count();

    return unix_ts_ms_t::SMake(cnt);
}

unix_ts_s_t GpDateTimeOps::SUnixTS_s (void) noexcept
{
    return SUnixTS_ms();
}

unix_ts_ms_t    GpDateTimeOps::SUnixTsFromStr_ms
(
    std::string_view    aStr,
    std::string_view    aFormat
)
{
    std::istringstream in{std::string(aStr)};

    //std::chrono::sys_time<std::chrono::milliseconds> tp;
    std::chrono::sys_time<std::chrono::milliseconds> tp;
    in >> date::parse(std::string(aFormat), tp);//TODO replace with std::chrono::parse

    const auto val = tp.time_since_epoch();
    const auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(val).count();

    return unix_ts_ms_t::SMake(cnt);
}

unix_ts_s_t GpDateTimeOps::SUnixTsFromStr_s
(
    std::string_view    aStr,
    std::string_view    aFormat
)
{
    return SUnixTsFromStr_ms(aStr, aFormat);
}

unix_ts_ms_t    GpDateTimeOps::SUnixTsFromStr_ms
(
    std::string_view    aStr,
    FormatTE            aFormat
)
{
    return SUnixTsFromStr_ms(aStr, sFormats.at(size_t(aFormat)));
}

unix_ts_s_t GpDateTimeOps::SUnixTsFromStr_s
(
    std::string_view    aStr,
    FormatTE            aFormat
)
{
    return SUnixTsFromStr_ms(aStr, sFormats.at(size_t(aFormat)));
}

microseconds_t  GpDateTimeOps::SSteadyTS_us (void) noexcept
{
    const auto val = std::chrono::steady_clock::now().time_since_epoch();
    const auto cnt = std::chrono::duration_cast<std::chrono::microseconds>(val).count();

    return microseconds_t::SMake(static_cast<microseconds_t::value_type>(cnt));
}

milliseconds_t  GpDateTimeOps::SSteadyTS_ms (void) noexcept
{
    const auto val = std::chrono::steady_clock::now().time_since_epoch();
    const auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(val).count();

    return milliseconds_t::SMake(static_cast<milliseconds_t::value_type>(cnt));
}

seconds_t   GpDateTimeOps::SSteadyTS_s (void) noexcept
{
    return SSteadyTS_ms();
}

microseconds_t  GpDateTimeOps::SHighResTS_us (void) noexcept
{
    const auto val = std::chrono::high_resolution_clock::now().time_since_epoch();
    const auto cnt = std::chrono::duration_cast<std::chrono::microseconds>(val).count();

    return microseconds_t::SMake(static_cast<microseconds_t::value_type>(cnt));
}

std::string GpDateTimeOps::SUnixTsToStr
(
    const unix_ts_ms_t  aTs,
    std::string_view    aFormat
)
{
    std::ostringstream out;

    //https://howardhinnant.github.io/date/date.html
    //https://gitter.im/HowardHinnant/date?at=5e404b1fb612cc7bb1588132

    //std::chrono::sys_time<std::chrono::milliseconds> tp(std::chrono::milliseconds(aTs.Value()));
    std::chrono::sys_time<std::chrono::milliseconds> tp(std::chrono::milliseconds(aTs.Value()));
    out << date::format(std::string(aFormat), tp);

    return std::string(out.str());
}

std::string GpDateTimeOps::SUnixTsToStr
(
    const unix_ts_s_t   aTs,
    std::string_view    aFormat
)
{
    return SUnixTsToStr(aTs.As<unix_ts_ms_t>(), aFormat);
}

std::string GpDateTimeOps::SUnixTsToStr
(
    const unix_ts_ms_t  aTs,
    const FormatTE      aFormat
)
{
    return SUnixTsToStr(aTs, sFormats.at(size_t(aFormat)));
}

std::string GpDateTimeOps::SUnixTsToStr
(
    const unix_ts_s_t   aTs,
    const FormatTE      aFormat
)
{
    return SUnixTsToStr(aTs.As<unix_ts_ms_t>(), sFormats.at(size_t(aFormat)));
}

std::string GpDateTimeOps::SToDaysHoursMinSec (const milliseconds_t aDuration)
{
    long double duration = static_cast<long double>(aDuration.Value());

    // Days
    const long double   daysDuration    = duration / (24.0L * 60.0L * 60.0L * 1000.0L);
    const size_t        days            = static_cast<size_t>(std::floor(daysDuration));

    if (days > 0)
    {
        duration -= static_cast<long double>(days) * (24.0L * 60.0L * 60.0L * 1000.0L);
    }

    // Hours
    const long double   hoursDuration   = duration / (60.0L * 60.0L * 1000.0L);
    const size_t        hours           = static_cast<size_t>(std::floor(hoursDuration));

    if (hours > 0)
    {
        duration -= static_cast<long double>(hours) * (60.0L * 60.0L * 1000.0L);
    }

    // Minutes
    const long double   minutesDuration = duration / (60.0L * 1000.0L);
    const size_t        minutes         = static_cast<size_t>(std::floor(minutesDuration));

    if (minutes > 0)
    {
        duration -= static_cast<long double>(minutes) * (60.0L * 1000.0L);
    }

    // Seconds
    const long double   secondsDuration = duration / (1000.0L);
    const double        seconds         = static_cast<double>(secondsDuration);

    // To string
    return fmt::format
    (
        "{} days, {} hours, {} minutes, {} seconds",
        days,
        hours,
        minutes,
        StrOps::SFromDouble(seconds)
    );
}

}// namespace GPlatform

#endif// #if defined(GP_USE_DATE_TIME)
