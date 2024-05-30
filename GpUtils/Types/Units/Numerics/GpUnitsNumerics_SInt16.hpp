#pragma once

#include "../GpUnit.hpp"
#include "../../Strings/GpStringTemplateArg.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_S_INT_16;

using SInt16 = GpUnit<s_int_16, GpUnitType_NUMERIC_S_INT_16, std::ratio<1, 1>, GpStringTemplateArg<'s','_','i','n','t','_','1','6'>>;

constexpr SInt16 operator"" _s_int_16  (const unsigned long long aValue) {return SInt16::SMake(aValue);}

}// namespace GPlatform
