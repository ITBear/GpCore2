#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_SI_FREQUENCY;

using frequency_Hz_t    = GpUnit<double, GpUnitType_SI_FREQUENCY, std::ratio<intmax_t(1), intmax_t(1)>, GpStringTemplateArg<'H','z'>>;
using frequency_kHz_t   = GpUnit<double, GpUnitType_SI_FREQUENCY, std::kilo                         , GpStringTemplateArg<'k','H','z'>>;
using frequency_MHz_t   = GpUnit<double, GpUnitType_SI_FREQUENCY, std::mega                         , GpStringTemplateArg<'M','H','z'>>;
using frequency_GHz_t   = GpUnit<double, GpUnitType_SI_FREQUENCY, std::giga                         , GpStringTemplateArg<'G','H','z'>>;

constexpr frequency_Hz_t    operator""_si_Hz  (const long double aValue) {return frequency_Hz_t::SMake(aValue);}
constexpr frequency_kHz_t   operator""_si_kHz (const long double aValue) {return frequency_kHz_t::SMake(aValue);}
constexpr frequency_MHz_t   operator""_si_MHz (const long double aValue) {return frequency_MHz_t::SMake(aValue);}
constexpr frequency_GHz_t   operator""_si_GHz (const long double aValue) {return frequency_GHz_t::SMake(aValue);}

}// namespace GPlatform
