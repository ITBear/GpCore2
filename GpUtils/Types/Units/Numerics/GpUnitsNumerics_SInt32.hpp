#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_S_INT_32;

using SInt32 = GpUnit<s_int_32, 1, GpUnitType_NUMERIC_S_INT_32, std::ratio<1, 1>, decltype("s_int_32"_template_str)>;

constexpr SInt32 operator"" _s_int_32  (const unsigned long long aValue) {return SInt32::SMake(aValue);}

}//GPlatform
