#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_PIXELS;

using pixels_t = GpUnit<u_int_64, 1, GpUnitType_PIXELS, std::ratio<intmax_t(1), intmax_t(1)>, decltype("px"_template_str)>;

constexpr pixels_t  operator""_px   (unsigned long long int aValue) {return pixels_t::SMake(aValue);}

}//GPlatform
