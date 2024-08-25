#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_NUMERIC_S_INT_64;

using SInt64 = GpUnit<s_int_64, GpUnitType_NUMERIC_S_INT_64, std::ratio<1, 1>, GpStringTemplateArg<'s','_','i','n','t','_','6','4'>>;

constexpr SInt64 operator"" _s_int_64  (const unsigned long long aValue) {return SInt64::SMake(aValue);}

}// namespace GPlatform
