#pragma once

#include "../GpUnit.hpp"
#include "../../Strings/GpStringTemplateArg.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_FLOAT;

using Float = GpUnit<float, GpUnitType_NUMERIC_FLOAT, std::ratio<1, 1>, GpStringTemplateArg<'f','l','o','a','t'>>;

constexpr Float operator"" _float  (const unsigned long long aValue) {return Float::SMake(static_cast<long double>(aValue));}
constexpr Float operator"" _float  (const long double aValue) {return Float::SMake(aValue);}

}// namespace GPlatform
