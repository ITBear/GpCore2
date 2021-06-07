#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_DATE_TIME)

#include "../../GpCore_global.hpp"
#include "../Units/SI/GpUnitsSI_Time.hpp"
#include "../Units/Other/unix_ts_t.hpp"
#include "../Containers/GpRawPtrByte.hpp"
#include "GpDateTimeFormat.hpp"

#include <chrono>

namespace GPlatform {

class GPCORE_API GpDateTimeOps
{
public:
    CLASS_REMOVE_CTRS(GpDateTimeOps);

    using FormatT   = GpDateTimeFormat;
    using FormatTE  = FormatT::EnumT;

public:
    [[nodiscard]] static unix_ts_ms_t           SUnixTS_ms          (void) noexcept;
    [[nodiscard]] static unix_ts_s_t            SUnixTS_s           (void) noexcept;

    [[nodiscard]] static std::string            SUnixTsToStr        (const unix_ts_ms_t aTs, std::string_view aFormat);
    [[nodiscard]] static std::string            SUnixTsToStr        (const unix_ts_s_t aTs, std::string_view aFormat);
    [[nodiscard]] static std::string            SUnixTsToStr        (const unix_ts_ms_t aTs, const FormatTE aFormat);
    [[nodiscard]] static std::string            SUnixTsToStr        (const unix_ts_s_t aTs, const FormatTE aFormat);

    [[nodiscard]] static unix_ts_ms_t           SUnixTsFromStr_ms   (GpRawPtrCharR aStr, std::string_view aFormat);
    [[nodiscard]] static unix_ts_s_t            SUnixTsFromStr_s    (GpRawPtrCharR aStr, std::string_view aFormat);
    [[nodiscard]] static unix_ts_ms_t           SUnixTsFromStr_ms   (GpRawPtrCharR aStr, const FormatTE aFormat);
    [[nodiscard]] static unix_ts_s_t            SUnixTsFromStr_s    (GpRawPtrCharR aStr, const FormatTE aFormat);

    //[[nodiscard]] static std::chrono::hh_mm_ss    SUnixTsToHH_MM_SS   (const unix_ts_ms_t aTs) noexcept;
    //[[nodiscard]] static hours_t              SUnixTsToHH         (const unix_ts_ms_t aTs) noexcept;

    [[nodiscard]] static milliseconds_t         SSteadyTS_ms        (void) noexcept;
    [[nodiscard]] static seconds_t              SSteadyTS_s         (void) noexcept;

    [[nodiscard]] static microseconds_t         SHighResTS_us       (void) noexcept;

private:
    static const GpArray<std::string, GpDateTimeFormat::SCount().As<size_t>()>  sFormats;
};

}//GPlatform

#endif//#if defined(GP_USE_DATE_TIME)
