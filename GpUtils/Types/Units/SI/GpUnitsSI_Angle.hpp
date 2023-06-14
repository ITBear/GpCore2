#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_SI_ANGLE;

using angle_deg_t   = GpUnit<double, GpUnitType_SI_ANGLE, std::ratio<intmax_t(1), intmax_t(1)>, decltype("deg"_template_str)>;
using angle_rad_t   = GpUnit<double, GpUnitType_SI_ANGLE, std::ratio<intmax_t(100000000000000), intmax_t(1745329251994)>, decltype("rad"_template_str)>;

constexpr angle_deg_t   operator"" _si_deg (const long double aValue) {return angle_deg_t::SMake(aValue);}
constexpr angle_rad_t   operator"" _si_rad (const long double aValue) {return angle_rad_t::SMake(aValue);}

}//GPlatform
