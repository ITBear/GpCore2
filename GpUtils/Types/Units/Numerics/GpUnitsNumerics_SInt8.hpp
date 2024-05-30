#pragma once

#include "../GpUnit.hpp"
#include "../../Strings/GpStringTemplateArg.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_S_INT_8;

using SInt8 = GpUnit<s_int_8, GpUnitType_NUMERIC_S_INT_8, std::ratio<1, 1>, GpStringTemplateArg<'s','_','i','n','t','_','8'>>;

constexpr SInt8 operator"" _s_int_8  (const unsigned long long aValue) {return SInt8::SMake(aValue);}

}// namespace GPlatform
