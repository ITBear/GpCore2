#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_SI_TIME;

using microseconds_t    = GpUnit<double, 1, GpUnitType_SI_TIME, std::micro,                             decltype("us"_template_str)>;
using milliseconds_t    = GpUnit<double, 1, GpUnitType_SI_TIME, std::milli,                             decltype("ms"_template_str)>;
using seconds_t         = GpUnit<double, 1, GpUnitType_SI_TIME, std::ratio<intmax_t(1),    intmax_t(1)>,    decltype("s"_template_str)>;
using minutes_t         = GpUnit<double, 1, GpUnitType_SI_TIME, std::ratio<intmax_t(60),   intmax_t(1)>,    decltype("minutes"_template_str)>;
using hours_t           = GpUnit<double, 1, GpUnitType_SI_TIME, std::ratio<intmax_t(3600), intmax_t(1)>,    decltype("hours"_template_str)>;

constexpr microseconds_t    operator"" _si_us (long double aValue)      {return microseconds_t::SMake(microseconds_t::value_type(aValue));}
constexpr milliseconds_t    operator"" _si_ms (long double aValue)      {return milliseconds_t::SMake(milliseconds_t::value_type(aValue));}
constexpr seconds_t         operator"" _si_s  (long double aValue)      {return seconds_t::SMake(seconds_t::value_type(aValue));}
constexpr minutes_t         operator"" _si_minutes(long double aValue)  {return minutes_t::SMake(minutes_t::value_type(aValue));}
constexpr hours_t           operator"" _si_hours(long double aValue)    {return hours_t::SMake(hours_t::value_type(aValue));}

}//GPlatform
