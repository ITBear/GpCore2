#include "GpDateTimeOps.hpp"

#if defined(GP_USE_DATE_TIME)

#include <date/date.h> //TODO: remove with c++20

namespace GPlatform {

const GpArray<std::string, GpDateTimeFormat::SCount().As<size_t>()> GpDateTimeOps::sFormats =
{
    "%FT%X+00:00",              //ISO_8601:         2021-01-11T20:15:31+00:00
    "%a, %d %b %Y %X +0000",    //RFC_2822:         Mon, 11 Jan 2021 20:15:31 +0000
    "%F %X",                    //STD_DATE_TIME:    2021-01-11 20:15:31
    "%FT%X",                    //STD_DATE_TIME_T,//2021-01-11T20:15:31
    "%F",                       //STD_DATE:         2021-01-11
    "%X"                        //STD_TIME:         20:15:31
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
    GpRawPtrCharR       aStr,
    std::string_view    aFormat
)
{
    std::istringstream in{std::string(aStr.AsStringView())};

    date::sys_time<std::chrono::milliseconds> tp;
    in >> date::parse(std::string(aFormat), tp);

    const auto val = tp.time_since_epoch();
    const auto cnt = std::chrono::duration_cast<std::chrono::milliseconds>(val).count();

    return unix_ts_ms_t::SMake(cnt);
}

unix_ts_s_t GpDateTimeOps::SUnixTsFromStr_s
(
    GpRawPtrCharR       aStr,
    std::string_view    aFormat
)
{
    return SUnixTsFromStr_ms(aStr, aFormat);
}

unix_ts_ms_t    GpDateTimeOps::SUnixTsFromStr_ms
(
    GpRawPtrCharR   aStr,
    FormatTE        aFormat
)
{
    return SUnixTsFromStr_ms(aStr, sFormats.at(aFormat));
}

unix_ts_s_t GpDateTimeOps::SUnixTsFromStr_s
(
    GpRawPtrCharR   aStr,
    FormatTE        aFormat
)
{
    return SUnixTsFromStr_ms(aStr, sFormats.at(aFormat));
}

/*std::chrono::hh_mm_ss GpDateTimeOps::SUnixTsToHH_MM_SS (const unix_ts_ms_t aTs) noexcept
{
    ?
}*/

/*hours_t       GpDateTimeOps::SUnixTsToHH (const unix_ts_ms_t aTs) noexcept
{
    date::sys_time<std::chrono::milliseconds> tp(std::chrono::milliseconds(aTs.Value()));
    date::hh_mm_ss h(tp.time_since_epoch());

    return hours_t::SMake(h.hours().count());
}*/

microseconds_t  GpDateTimeOps::SSteadyTS_us (void) noexcept
{
    const auto val = std::chrono::steady_clock::now().time_since_epoch();
    const auto cnt = std::chrono::duration_cast<std::chrono::microseconds>(val).count();

    return microseconds_t::SMake(microseconds_t::value_type(cnt));
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

std::string GpDateTimeOps::SUnixTsToStr
(
    const unix_ts_ms_t  aTs,
    std::string_view    aFormat
)
{
    std::ostringstream out;

    //https://howardhinnant.github.io/date/date.html
    //https://gitter.im/HowardHinnant/date?at=5e404b1fb612cc7bb1588132

    date::sys_time<std::chrono::milliseconds> tp(std::chrono::milliseconds(aTs.Value()));
    out << date::format(std::string(aFormat), tp);

    return out.str();
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
    return SUnixTsToStr(aTs, sFormats.at(aFormat));
}

std::string GpDateTimeOps::SUnixTsToStr
(
    const unix_ts_s_t   aTs,
    const FormatTE      aFormat
)
{
    return SUnixTsToStr(aTs.As<unix_ts_ms_t>(), sFormats.at(aFormat));
}

}//GPlatform

#endif//#if defined(GP_USE_DATE_TIME)
