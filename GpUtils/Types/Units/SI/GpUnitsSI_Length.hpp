#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_SI_LENGTH;

using picometers_t  = GpUnit<double, GpUnitType_SI_LENGTH, std::pico                            , GpStringTemplateArg<'p','m'>>;
using nanometers_t  = GpUnit<double, GpUnitType_SI_LENGTH, std::nano                            , GpStringTemplateArg<'n','m'>>;
using micrometers_t = GpUnit<double, GpUnitType_SI_LENGTH, std::micro                           , GpStringTemplateArg<'u','m'>>;
using millimeters_t = GpUnit<double, GpUnitType_SI_LENGTH, std::milli                           , GpStringTemplateArg<'m','m'>>;
using centimeters_t = GpUnit<double, GpUnitType_SI_LENGTH, std::centi                           , GpStringTemplateArg<'c','m'>>;
using decimeters_t  = GpUnit<double, GpUnitType_SI_LENGTH, std::deci                            , GpStringTemplateArg<'d','m'>>;
using meters_t      = GpUnit<double, GpUnitType_SI_LENGTH, std::ratio<intmax_t(1), intmax_t(1)> , GpStringTemplateArg<'m'>>;
using kilometers_t  = GpUnit<double, GpUnitType_SI_LENGTH, std::kilo                            , GpStringTemplateArg<'k','m'>>;

constexpr picometers_t  operator"" _si_pm (const long double aValue) {return picometers_t::SMake(picometers_t::value_type(aValue));}
constexpr nanometers_t  operator"" _si_nm (const long double aValue) {return nanometers_t::SMake(nanometers_t::value_type(aValue));}
constexpr micrometers_t operator"" _si_um (const long double aValue) {return micrometers_t::SMake(micrometers_t::value_type(aValue));}
constexpr millimeters_t operator"" _si_mm (const long double aValue) {return millimeters_t::SMake(millimeters_t::value_type(aValue));}
constexpr centimeters_t operator"" _si_cm (const long double aValue) {return centimeters_t::SMake(centimeters_t::value_type(aValue));}
constexpr decimeters_t  operator"" _si_dm (const long double aValue) {return decimeters_t::SMake(decimeters_t::value_type(aValue));}
constexpr meters_t      operator"" _si_m  (const long double aValue) {return meters_t::SMake(meters_t::value_type(aValue));}
constexpr kilometers_t  operator"" _si_km (const long double aValue) {return kilometers_t::SMake(kilometers_t::value_type(aValue));}

}// namespace GPlatform
