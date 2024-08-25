#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_NUMERIC_S_INT_32;

using SInt32 = GpUnit<s_int_32, GpUnitType_NUMERIC_S_INT_32, std::ratio<1, 1>, GpStringTemplateArg<'s','_','i','n','t','_','3','2'>>;

constexpr SInt32 operator"" _s_int_32  (const unsigned long long aValue) {return SInt32::SMake(aValue);}

}// namespace GPlatform
