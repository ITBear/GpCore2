#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_PIXELS;

using pixels_t = GpUnit<u_int_64, GpUnitType_PIXELS, std::ratio<intmax_t(1), intmax_t(1)>, GpStringTemplateArg<'p','x'>>;

constexpr pixels_t  operator""_px   (unsigned long long int aValue) {return pixels_t::SMake(aValue);}

}// namespace GPlatform
