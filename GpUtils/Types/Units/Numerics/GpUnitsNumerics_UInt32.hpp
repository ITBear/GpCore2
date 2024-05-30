#pragma once

#include "../GpUnit.hpp"
#include "../../Strings/GpStringTemplateArg.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_U_INT_32;

using UInt32 = GpUnit<u_int_32, GpUnitType_NUMERIC_U_INT_32, std::ratio<1, 1>, GpStringTemplateArg<'u','_','i','n','t','_','3','2'>>;

constexpr UInt32 operator"" _u_int_32  (const unsigned long long aValue) {return UInt32::SMake(aValue);}

}// namespace GPlatform
