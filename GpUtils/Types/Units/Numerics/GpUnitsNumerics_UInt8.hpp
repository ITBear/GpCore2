#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_NUMERIC_U_INT_8;

using UInt8 = GpUnit<u_int_8, GpUnitType_NUMERIC_U_INT_8, std::ratio<1, 1>, GpStringTemplateArg<'u','_','i','n','t','_','8'>>;

constexpr UInt8 operator"" _u_int_8  (const unsigned long long aValue) {return UInt8::SMake(aValue);}

}// namespace GPlatform
