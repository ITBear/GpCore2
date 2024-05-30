#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_DATE_TIME)

#include "../Types/Units/SI/GpUnitsSI_Time.hpp"
#include "../Types/Units/Other/unix_ts_t.hpp"
#include "GpDateTimeFormat.hpp"

#include <GpCore2/Config/IncludeExt/fmt.hpp>

namespace GPlatform {

class GP_UTILS_API GpDateTimeOps
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDateTimeOps)

    using FormatT   = GpDateTimeFormat;
    using FormatTE  = FormatT::EnumT;

public:
    static unix_ts_ms_t     SUnixTS_ms              (void) noexcept;
    static unix_ts_s_t      SUnixTS_s               (void) noexcept;

    static unix_ts_ms_t     SUnixTsFromStr_ms       (std::string_view   aStr,
                                                     std::string_view   aFormat);
    static unix_ts_s_t      SUnixTsFromStr_s        (std::string_view   aStr,
                                                     std::string_view   aFormat);
    static unix_ts_ms_t     SUnixTsFromStr_ms       (std::string_view   aStr,
                                                     const FormatTE     aFormat);
    static unix_ts_s_t      SUnixTsFromStr_s        (std::string_view   aStr,
                                                     const FormatTE     aFormat);

    static microseconds_t   SSteadyTS_us            (void) noexcept;
    static milliseconds_t   SSteadyTS_ms            (void) noexcept;
    static seconds_t        SSteadyTS_s             (void) noexcept;

    static microseconds_t   SSteadyTS_us_AtAppStart (void) noexcept {return sStartSteadyTSus;}
    static microseconds_t   SSteadyTS_ms_AtAppStart (void) noexcept {return sStartSteadyTSms;}

    static microseconds_t   SHighResTS_us           (void) noexcept;

    static std::string      SUnixTsToStr            (const unix_ts_ms_t aTs,
                                                     std::string_view   aFormat);
    static std::string      SUnixTsToStr            (const unix_ts_s_t  aTs,
                                                     std::string_view   aFormat);
    static std::string      SUnixTsToStr            (const unix_ts_ms_t aTs,
                                                     const FormatTE     aFormat);
    static std::string      SUnixTsToStr            (const unix_ts_s_t  aTs,
                                                     const FormatTE     aFormat);

private:
    static const microseconds_t                                         sStartSteadyTSus;
    static const milliseconds_t                                         sStartSteadyTSms;
    static const std::array<std::string, GpDateTimeFormat::SCount()>    sFormats;
};

}// namespace GPlatform

// ------------------------------------ fmt ------------------------------------

namespace fmt {

using namespace GPlatform;

template<>
struct formatter<unix_ts_ms_t>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& aCtx)
    {
        return std::begin(aCtx);
    }

    template<typename FormatContext>
    auto format(unix_ts_ms_t aUnixTS, FormatContext& aCtx) const
    {
        const std::string str = GpDateTimeOps::SUnixTsToStr(aUnixTS, GpDateTimeFormat::STD_DATE_TIME);

        return ::fmt::format_to
        (
            aCtx.out(),
            "{}",
            str
        );
    }
};

template<>
struct formatter<unix_ts_s_t>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& aCtx)
    {
        return std::begin(aCtx);
    }

    template<typename FormatContext>
    auto format(unix_ts_s_t aUnixTS, FormatContext& aCtx) const
    {
        const std::string str = GpDateTimeOps::SUnixTsToStr(aUnixTS, GpDateTimeFormat::STD_DATE_TIME);

        return ::fmt::format_to
        (
            aCtx.out(),
            "{}",
            str
        );
    }
};

}// namespace std

#endif// #if defined(GP_USE_DATE_TIME)
