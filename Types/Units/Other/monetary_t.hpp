#pragma once

#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_MONETARY;

using monetary_t = GpUnit<s_int_64, GpUnitType_MONETARY, std::ratio<intmax_t(1), intmax_t(1)>, decltype("monetary"_template_str)>;

constexpr monetary_t    operator""_monetary (unsigned long long int aValue) {return monetary_t::SMake(monetary_t::value_type(aValue));}

}//GPlatform
