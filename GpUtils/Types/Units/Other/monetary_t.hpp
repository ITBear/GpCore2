#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_MONETARY;

using monetary_t = GpUnit<s_int_64, GpUnitType_MONETARY, std::ratio<intmax_t(1), intmax_t(1)>, GpStringTemplateArg<'m','o','n','e','t','a','r','y'>>;

constexpr monetary_t    operator""_monetary (unsigned long long int aValue) {return monetary_t::SMake(monetary_t::value_type(aValue));}

}// namespace GPlatform
