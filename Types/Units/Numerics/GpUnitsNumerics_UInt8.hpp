#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_U_INT_8;

using UInt8	= GpUnit<u_int_8, GpUnitType_NUMERIC_U_INT_8, std::ratio<1, 1>, decltype("u_int_8"_template_str)>;

constexpr UInt8	operator"" _u_int_8  (const unsigned long long aValue) {return UInt8::SMake(aValue);}

}//GPlatform
