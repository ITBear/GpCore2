#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_NUMERIC_DOUBLE;

using Double = GpUnit<double, GpUnitType_NUMERIC_DOUBLE, std::ratio<1, 1>, GpStringTemplateArg<'d','o','u','b','l','e'>>;

constexpr Double    operator"" _double  (const unsigned long long aValue) {return Double::SMake(static_cast<long double>(aValue));}
constexpr Double    operator"" _double  (const long double aValue) {return Double::SMake(aValue);}

}// namespace GPlatform
