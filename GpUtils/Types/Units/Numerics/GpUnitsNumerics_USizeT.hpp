#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_NUMERIC_U_SIZE_T;

using USizeT = GpUnit<size_t, 1, GpUnitType_NUMERIC_U_SIZE_T, std::ratio<1, 1>, decltype("size_t"_template_str)>;

constexpr USizeT    operator"" _usize_t  (const unsigned long long aValue) {return USizeT::SMake(aValue);}

}//GPlatform
