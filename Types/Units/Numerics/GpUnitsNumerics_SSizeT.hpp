#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_S_SIZE_T;

using SSizeT = GpUnit<ssize_t, GpUnitType_NUMERIC_S_SIZE_T, std::ratio<1, 1>, decltype("ssize_t"_template_str)>;

constexpr SSizeT    operator"" _ssize_t  (const unsigned long long aValue) {return SSizeT::SMake(aValue);}

}//GPlatform
