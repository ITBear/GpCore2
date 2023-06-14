#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_S_INT_8;

using SInt8 = GpUnit<s_int_8, GpUnitType_NUMERIC_S_INT_8, std::ratio<1, 1>, decltype("s_int_8"_template_str)>;

constexpr SInt8 operator"" _s_int_8  (const unsigned long long aValue) {return SInt8::SMake(aValue);}

}//GPlatform
