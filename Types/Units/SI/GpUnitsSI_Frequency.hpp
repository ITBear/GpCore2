#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_SI_FREQUENCY;

using frequency_Hz_t	= GpUnit<s_int_64, GpUnitType_SI_FREQUENCY, std::ratio<intmax_t(1), intmax_t(1)>, decltype("Hz"_template_str)>;
using frequency_kHz_t	= GpUnit<s_int_64, GpUnitType_SI_FREQUENCY, std::kilo							, decltype("KHz"_template_str)>;
using frequency_MHz_t	= GpUnit<s_int_64, GpUnitType_SI_FREQUENCY, std::mega						    , decltype("MHz"_template_str)>;
using frequency_GHz_t	= GpUnit<s_int_64, GpUnitType_SI_FREQUENCY, std::giga							, decltype("GHz"_template_str)>;

constexpr frequency_Hz_t	operator"" _si_Hz  (const unsigned long long aValue) {return frequency_Hz_t::SMake(aValue);}
constexpr frequency_kHz_t	operator"" _si_kHz (const unsigned long long aValue) {return frequency_kHz_t::SMake(aValue);}
constexpr frequency_MHz_t	operator"" _si_MHz (const unsigned long long aValue) {return frequency_MHz_t::SMake(aValue);}
constexpr frequency_GHz_t	operator"" _si_GHz (const unsigned long long aValue) {return frequency_GHz_t::SMake(aValue);}

}//GPlatform
