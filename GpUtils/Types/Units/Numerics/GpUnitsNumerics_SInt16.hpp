#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_S_INT_16;

using SInt16 = GpUnit<s_int_16, GpUnitType_NUMERIC_S_INT_16, std::ratio<1, 1>, decltype("s_int_16"_template_str)>;

constexpr SInt16 operator"" _s_int_16  (const unsigned long long aValue) {return SInt16::SMake(aValue);}

}//GPlatform
