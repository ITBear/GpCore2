#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_U_INT_16;

using UInt16 = GpUnit<u_int_16, GpUnitType_NUMERIC_U_INT_16, std::ratio<1, 1>, decltype("u_int_16"_template_str)>;

constexpr UInt16 operator"" _u_int_16  (const unsigned long long aValue) {return UInt16::SMake(aValue);}

}//GPlatform
