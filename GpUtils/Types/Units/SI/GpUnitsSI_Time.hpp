#pragma once

#include "../GpUnit.hpp"
#include "../../Strings/GpStringTemplateArg.hpp"

namespace GPlatform {

class GpUnitType_SI_TIME;

using microseconds_t    = GpUnit<double, GpUnitType_SI_TIME, std::micro,                                GpStringTemplateArg<'u','s'>>;
using milliseconds_t    = GpUnit<double, GpUnitType_SI_TIME, std::milli,                                GpStringTemplateArg<'m','s'>>;
using seconds_t         = GpUnit<double, GpUnitType_SI_TIME, std::ratio<intmax_t(1),    intmax_t(1)>,   GpStringTemplateArg<'s'>>;
using minutes_t         = GpUnit<double, GpUnitType_SI_TIME, std::ratio<intmax_t(60),   intmax_t(1)>,   GpStringTemplateArg<'m','i','n','u','t','e','s'>>;
using hours_t           = GpUnit<double, GpUnitType_SI_TIME, std::ratio<intmax_t(3600), intmax_t(1)>,   GpStringTemplateArg<'h','o','u','r','s'>>;

constexpr microseconds_t    operator"" _si_us (long double aValue)      {return microseconds_t::SMake(microseconds_t::value_type(aValue));}
constexpr milliseconds_t    operator"" _si_ms (long double aValue)      {return milliseconds_t::SMake(milliseconds_t::value_type(aValue));}
constexpr seconds_t         operator"" _si_s  (long double aValue)      {return seconds_t::SMake(seconds_t::value_type(aValue));}
constexpr minutes_t         operator"" _si_minutes(long double aValue)  {return minutes_t::SMake(minutes_t::value_type(aValue));}
constexpr hours_t           operator"" _si_hours(long double aValue)    {return hours_t::SMake(hours_t::value_type(aValue));}

}// namespace GPlatform
