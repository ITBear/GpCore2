#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_S_INT_64;

using SInt64 = GpUnit<s_int_64, 1, GpUnitType_NUMERIC_S_INT_64, std::ratio<1, 1>, decltype("s_int_64"_template_str)>;

constexpr SInt64 operator"" _s_int_64  (const unsigned long long aValue) {return SInt64::SMake(aValue);}

}//GPlatform
