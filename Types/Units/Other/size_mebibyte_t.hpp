#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_MEBIBYTES_SIZE;

//1 bit
using size_mebibit_t    = GpUnit<s_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1), intmax_t(1)>, decltype("bit"_template_str)>;

//1 byte
using size_mebibyte_t   = GpUnit<s_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1)*intmax_t(8), intmax_t(1)>, decltype("byte"_template_str)>;

//1024^1
using size_KiB_t        = GpUnit<s_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1024)*intmax_t(8), intmax_t(1)>, decltype("KiB"_template_str)>;

//1024^2
using size_MiB_t        = GpUnit<s_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1024)*intmax_t(1024)*intmax_t(8), intmax_t(1)>, decltype("MiB"_template_str)>;

//1024^3
using size_GiB_t        = GpUnit<s_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1024)*intmax_t(1024)*intmax_t(1024)*intmax_t(8), intmax_t(1)>, decltype("GiB"_template_str)>;

//1024^4
using size_TiB_t        = GpUnit<s_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1024)*intmax_t(1024)*intmax_t(1024)*intmax_t(1024)*intmax_t(8), intmax_t(1)>, decltype("TiB"_template_str)>;

constexpr size_mebibit_t    operator"" _mebibit (unsigned long long int aValue) {return size_mebibit_t::SMake(size_mebibyte_t::value_type(aValue));}
constexpr size_mebibyte_t   operator"" _mebibyte(unsigned long long int aValue) {return size_mebibyte_t::SMake(size_mebibyte_t::value_type(aValue));}
constexpr size_KiB_t        operator"" _KiB     (unsigned long long int aValue) {return size_KiB_t::SMake(size_KiB_t::value_type(aValue));}
constexpr size_MiB_t        operator"" _MiB     (unsigned long long int aValue) {return size_MiB_t::SMake(size_MiB_t::value_type(aValue));}
constexpr size_GiB_t        operator"" _GiB     (unsigned long long int aValue) {return size_GiB_t::SMake(size_GiB_t::value_type(aValue));}
constexpr size_TiB_t        operator"" _TiB     (unsigned long long int aValue) {return size_TiB_t::SMake(size_TiB_t::value_type(aValue));}

}//GPlatform
