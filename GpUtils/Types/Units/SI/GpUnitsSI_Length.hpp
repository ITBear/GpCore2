#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_SI_LENGTH;

using picometers_t  = GpUnit<double, 1, GpUnitType_SI_LENGTH, std::pico                         , decltype("pm"_template_str)>;
using nanometers_t  = GpUnit<double, 1, GpUnitType_SI_LENGTH, std::nano                         , decltype("nm"_template_str)>;
using micrometers_t = GpUnit<double, 1, GpUnitType_SI_LENGTH, std::micro                        , decltype("um"_template_str)>;
using millimeters_t = GpUnit<double, 1, GpUnitType_SI_LENGTH, std::milli                        , decltype("mm"_template_str)>;
using centimeters_t = GpUnit<double, 1, GpUnitType_SI_LENGTH, std::centi                        , decltype("cm"_template_str)>;
using decimeters_t  = GpUnit<double, 1, GpUnitType_SI_LENGTH, std::deci                         , decltype("dm"_template_str)>;
using meters_t      = GpUnit<double, 1, GpUnitType_SI_LENGTH, std::ratio<intmax_t(1), intmax_t(1)>  , decltype("m"_template_str)>;
using kilometers_t  = GpUnit<double, 1, GpUnitType_SI_LENGTH, std::kilo                         , decltype("km"_template_str)>;

constexpr picometers_t  operator"" _si_pm (const long double aValue) {return picometers_t::SMake(picometers_t::value_type(aValue));}
constexpr nanometers_t  operator"" _si_nm (const long double aValue) {return nanometers_t::SMake(nanometers_t::value_type(aValue));}
constexpr micrometers_t operator"" _si_um (const long double aValue) {return micrometers_t::SMake(micrometers_t::value_type(aValue));}
constexpr millimeters_t operator"" _si_mm (const long double aValue) {return millimeters_t::SMake(millimeters_t::value_type(aValue));}
constexpr centimeters_t operator"" _si_cm (const long double aValue) {return centimeters_t::SMake(centimeters_t::value_type(aValue));}
constexpr decimeters_t  operator"" _si_dm (const long double aValue) {return decimeters_t::SMake(decimeters_t::value_type(aValue));}
constexpr meters_t      operator"" _si_m  (const long double aValue) {return meters_t::SMake(meters_t::value_type(aValue));}
constexpr kilometers_t  operator"" _si_km (const long double aValue) {return kilometers_t::SMake(kilometers_t::value_type(aValue));}

}//GPlatform
