#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_U_INT_32;

using UInt32 = GpUnit<u_int_32, 1, GpUnitType_NUMERIC_U_INT_32, std::ratio<1, 1>, decltype("u_int_32"_template_str)>;

constexpr UInt32 operator"" _u_int_32  (const unsigned long long aValue) {return UInt32::SMake(aValue);}

}//GPlatform
