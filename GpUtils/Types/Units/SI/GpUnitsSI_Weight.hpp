#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_SI_WEIGHT;

using picoGrams_t   = GpUnit<double, GpUnitType_SI_WEIGHT, std::pico,                           GpStringTemplateArg<'p','g'>>;
using nanoGrams_t   = GpUnit<double, GpUnitType_SI_WEIGHT, std::nano,                           GpStringTemplateArg<'n','g'>>;
using microGrams_t  = GpUnit<double, GpUnitType_SI_WEIGHT, std::micro,                          GpStringTemplateArg<'u','g'>>;
using milliGrams_t  = GpUnit<double, GpUnitType_SI_WEIGHT, std::milli,                          GpStringTemplateArg<'m','g'>>;
using grams_t       = GpUnit<double, GpUnitType_SI_WEIGHT, std::ratio<intmax_t(1), intmax_t(1)>,GpStringTemplateArg<'g'>>;
using kiloGrams_t   = GpUnit<double, GpUnitType_SI_WEIGHT, std::kilo,                           GpStringTemplateArg<'k','g'>>;

constexpr picoGrams_t   operator"" _si_pg (const long double aValue) {return picoGrams_t::SMake(picoGrams_t::value_type(aValue));}
constexpr nanoGrams_t   operator"" _si_ng (const long double aValue) {return nanoGrams_t::SMake(nanoGrams_t::value_type(aValue));}
constexpr microGrams_t  operator"" _si_ug (const long double aValue) {return microGrams_t::SMake(microGrams_t::value_type(aValue));}
constexpr milliGrams_t  operator"" _si_mg (const long double aValue) {return milliGrams_t::SMake(milliGrams_t::value_type(aValue));}
constexpr grams_t       operator"" _si_g  (const long double aValue) {return grams_t::SMake(grams_t::value_type(aValue));}
constexpr kiloGrams_t   operator"" _si_kg (const long double aValue) {return kiloGrams_t::SMake(kiloGrams_t::value_type(aValue));}

}// namespace GPlatform
