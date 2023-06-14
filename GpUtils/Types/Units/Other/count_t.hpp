#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_COUNT;

//using count_t = GpUnit<size_t, GpUnitType_COUNT, std::ratio<intmax_t(1), intmax_t(1)>, decltype("cnt"_template_str)>;
//constexpr count_t operator""_cnt  (unsigned long long int aValue) {return count_t::SMake(aValue);}

}//GPlatform
