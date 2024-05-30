#pragma once

#include "../GpUnit.hpp"
#include "../../Strings/GpStringTemplateArg.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_U_INT_64;

using UInt64 = GpUnit<u_int_64, GpUnitType_NUMERIC_U_INT_64, std::ratio<1, 1>, GpStringTemplateArg<'u','_','i','n','t','_','6','4'>>;

constexpr UInt64 operator"" _u_int_64  (const unsigned long long aValue) {return UInt64::SMake(aValue);}

}// namespace GPlatform
