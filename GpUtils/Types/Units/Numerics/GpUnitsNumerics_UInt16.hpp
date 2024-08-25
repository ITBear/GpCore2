#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_NUMERIC_U_INT_16;

using UInt16 = GpUnit<u_int_16, GpUnitType_NUMERIC_U_INT_16, std::ratio<1, 1>, GpStringTemplateArg<'u','_','i','n','t','_','1','6'>>;

constexpr UInt16 operator"" _u_int_16  (const unsigned long long aValue) {return UInt16::SMake(aValue);}

}// namespace GPlatform
