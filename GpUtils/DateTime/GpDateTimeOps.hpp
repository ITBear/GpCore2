#pragma once

#include "../GpMacro.hpp"

#if defined(GP_USE_DATE_TIME)

#include "../Types/Units/SI/GpUnitsSI_Time.hpp"
#include "../Types/Units/Other/unix_ts_t.hpp"
#include "GpDateTimeFormat.hpp"

namespace GPlatform {

class GP_UTILS_API GpDateTimeOps
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpDateTimeOps);

    using FormatT   = GpDateTimeFormat;
    using FormatTE  = FormatT::EnumT;

public:
    [[nodiscard]] static unix_ts_ms_t       SUnixTS_ms          (void) noexcept;
    [[nodiscard]] static unix_ts_s_t        SUnixTS_s           (void) noexcept;

    [[nodiscard]] static unix_ts_ms_t       SUnixTsFromStr_ms   (std::string_view   aStr,
                                                                 std::string_view   aFormat);
    [[nodiscard]] static unix_ts_s_t        SUnixTsFromStr_s    (std::string_view   aStr,
                                                                 std::string_view   aFormat);
    [[nodiscard]] static unix_ts_ms_t       SUnixTsFromStr_ms   (std::string_view   aStr,
                                                                 const FormatTE     aFormat);
    [[nodiscard]] static unix_ts_s_t        SUnixTsFromStr_s    (std::string_view   aStr,
                                                                 const FormatTE     aFormat);

    [[nodiscard]] static microseconds_t     SSteadyTS_us        (void) noexcept;
    [[nodiscard]] static milliseconds_t     SSteadyTS_ms        (void) noexcept;
    [[nodiscard]] static seconds_t          SSteadyTS_s         (void) noexcept;

    [[nodiscard]] static microseconds_t     SSteadyTS_us_AtAppStart (void) noexcept {return sStartSteadyTS;}

    [[nodiscard]] static microseconds_t     SHighResTS_us       (void) noexcept;

    [[nodiscard]] static std::string        SUnixTsToStr        (const unix_ts_ms_t aTs,
                                                                 std::string_view   aFormat);
    [[nodiscard]] static std::string        SUnixTsToStr        (const unix_ts_s_t  aTs,
                                                                 std::string_view   aFormat);
    [[nodiscard]] static std::string        SUnixTsToStr        (const unix_ts_ms_t aTs,
                                                                 const FormatTE     aFormat);
    [[nodiscard]] static std::string        SUnixTsToStr        (const unix_ts_s_t  aTs,
                                                                 const FormatTE     aFormat);

private:
    static const microseconds_t                                     sStartSteadyTS;
    static const GpArray<std::string, GpDateTimeFormat::SCount()>   sFormats;
};

}//GPlatform

#endif//#if defined(GP_USE_DATE_TIME)
