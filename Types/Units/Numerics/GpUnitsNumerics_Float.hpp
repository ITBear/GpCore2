#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_FLOAT;

using Float	= GpUnit<float, GpUnitType_NUMERIC_FLOAT, std::ratio<1, 1>, decltype("float"_template_str)>;

constexpr Float	operator"" _float  (const unsigned long long aValue) {return Float::SMake(static_cast<long double>(aValue));}
constexpr Float	operator"" _float  (const long double aValue) {return Float::SMake(aValue);}

}//GPlatform
