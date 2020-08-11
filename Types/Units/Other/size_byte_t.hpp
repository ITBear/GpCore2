#pragma once

#include "../GpUnit.hpp"

namespace GPlatform {

class GpUnitType_BYTES_SIZE;

//1 bit
using size_bit_t    = GpUnit<u_int_64, GpUnitType_BYTES_SIZE, std::ratio<intmax_t(1), intmax_t(1)>, decltype("bit"_template_str)>;

//1 byte
using size_byte_t   = GpUnit<u_int_64, GpUnitType_BYTES_SIZE, std::ratio<intmax_t(1)*intmax_t(8), intmax_t(1)>, decltype("byte"_template_str)>;

//10^3 = 1000 bytes
using size_kB_t = GpUnit<u_int_64, GpUnitType_BYTES_SIZE, std::ratio<intmax_t(1'000)*intmax_t(8), intmax_t(1)>, decltype("kB"_template_str)>;

//10^6 = 1 000 000 bytes
using size_MB_t = GpUnit<u_int_64, GpUnitType_BYTES_SIZE, std::ratio<intmax_t(1'000'000)*intmax_t(8), intmax_t(1)>, decltype("MB"_template_str)>;

//10^9 = 1 000 000 000 bytes
using size_GB_t = GpUnit<u_int_64, GpUnitType_BYTES_SIZE, std::ratio<intmax_t(1'000'000'000)*intmax_t(8), intmax_t(1)>, decltype("GB"_template_str)>;

//10^12 = 1 000 000 000 000 bytes
using size_TB_t = GpUnit<u_int_64, GpUnitType_BYTES_SIZE, std::ratio<intmax_t(1'000'000'000'000)*intmax_t(8), intmax_t(1)>, decltype("TB"_template_str)>;

constexpr size_bit_t    operator"" _bit (unsigned long long int aValue) {return size_bit_t::SMake(size_bit_t::value_type(aValue));}
constexpr size_byte_t   operator"" _byte(unsigned long long int aValue) {return size_byte_t::SMake(size_byte_t::value_type(aValue));}
constexpr size_kB_t     operator"" _kB  (unsigned long long int aValue) {return size_kB_t::SMake(size_kB_t::value_type(aValue));}
constexpr size_MB_t     operator"" _MB  (unsigned long long int aValue) {return size_MB_t::SMake(size_MB_t::value_type(aValue));}
constexpr size_GB_t     operator"" _GB  (unsigned long long int aValue) {return size_GB_t::SMake(size_GB_t::value_type(aValue));}
constexpr size_TB_t     operator"" _TB  (unsigned long long int aValue) {return size_TB_t::SMake(size_TB_t::value_type(aValue));}

}//GPlatform
