#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_U_INT_64;

using UInt64 = GpUnit<u_int_64, 1, GpUnitType_NUMERIC_U_INT_64, std::ratio<1, 1>, decltype("u_int_64"_template_str)>;

constexpr UInt64 operator"" _u_int_64  (const unsigned long long aValue) {return UInt64::SMake(aValue);}

}//GPlatform
