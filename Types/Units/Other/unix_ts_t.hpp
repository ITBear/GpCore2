#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_UNIX_TIMESTAMP;

using unix_ts_ms_t	= GpUnit<s_int_64, GpUnitType_UNIX_TIMESTAMP, std::milli,		decltype("unix_ts_ms"_template_str)>;
using unix_ts_s_t	= GpUnit<s_int_64, GpUnitType_UNIX_TIMESTAMP, std::ratio<intmax_t(1), intmax_t(1)>, decltype("unix_ts_s"_template_str)>;

constexpr unix_ts_ms_t	operator"" _unix_ts_ms (long double aValue)				{return unix_ts_ms_t::SMake(unix_ts_ms_t::value_type(aValue));}
constexpr unix_ts_ms_t	operator"" _unix_ts_ms (unsigned long long int aValue)	{return unix_ts_ms_t::SMake(unix_ts_ms_t::value_type(aValue));}
constexpr unix_ts_s_t	operator"" _unix_ts_s  (long double aValue)				{return unix_ts_s_t::SMake(unix_ts_s_t::value_type(aValue));}
constexpr unix_ts_s_t	operator"" _unix_ts_s  (unsigned long long int aValue)	{return unix_ts_s_t::SMake(unix_ts_s_t::value_type(aValue));}

}//GPlatform
