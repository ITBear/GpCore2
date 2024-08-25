#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_UNIX_TIMESTAMP;

using unix_ts_ms_t  = GpUnit<s_int_64, GpUnitType_UNIX_TIMESTAMP, std::milli,                           GpStringTemplateArg<'u','n','i','x','_','t','s','_','m','s'>>;
using unix_ts_s_t   = GpUnit<s_int_64, GpUnitType_UNIX_TIMESTAMP, std::ratio<intmax_t(1), intmax_t(1)>, GpStringTemplateArg<'u','n','i','x','_','t','s','_','s'>>;

constexpr unix_ts_ms_t  operator"" _unix_ts_ms (long double aValue)             {return unix_ts_ms_t::SMake(unix_ts_ms_t::value_type(aValue));}
constexpr unix_ts_ms_t  operator"" _unix_ts_ms (unsigned long long int aValue)  {return unix_ts_ms_t::SMake(unix_ts_ms_t::value_type(aValue));}
constexpr unix_ts_s_t   operator"" _unix_ts_s  (long double aValue)             {return unix_ts_s_t::SMake(unix_ts_s_t::value_type(aValue));}
constexpr unix_ts_s_t   operator"" _unix_ts_s  (unsigned long long int aValue)  {return unix_ts_s_t::SMake(unix_ts_s_t::value_type(aValue));}

}// namespace GPlatform
