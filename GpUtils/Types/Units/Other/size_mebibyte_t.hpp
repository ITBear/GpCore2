#pragma once

#include <GpCore2/GpUtils/Types/Units/GpUnit.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringTemplateArg.hpp>

namespace GPlatform {

class GpUnitType_MEBIBYTES_SIZE;

// 1 bit
using size_mibit_t  = GpUnit<u_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1), intmax_t(1)>, GpStringTemplateArg<'b','i','t'>>;

// 1 byte
using size_mibyte_t = GpUnit<u_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1)*intmax_t(8), intmax_t(1)>, GpStringTemplateArg<'b','y','t','e'>>;

// 1024^1
using size_KiB_t    = GpUnit<u_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1024)*intmax_t(8), intmax_t(1)>, GpStringTemplateArg<'K','i','B'>>;

// 1024^2
using size_MiB_t    = GpUnit<u_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1024)*intmax_t(1024)*intmax_t(8), intmax_t(1)>, GpStringTemplateArg<'M','i','B'>>;

// 1024^3
using size_GiB_t    = GpUnit<u_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1024)*intmax_t(1024)*intmax_t(1024)*intmax_t(8), intmax_t(1)>, GpStringTemplateArg<'G','i','B'>>;

// 1024^4
using size_TiB_t    = GpUnit<u_int_64, GpUnitType_MEBIBYTES_SIZE, std::ratio<intmax_t(1024)*intmax_t(1024)*intmax_t(1024)*intmax_t(1024)*intmax_t(8), intmax_t(1)>, GpStringTemplateArg<'T','i','B'>>;

constexpr size_mibit_t  operator"" _mibit   (unsigned long long int aValue) {return size_mibit_t::SMake(size_mibyte_t::value_type(aValue));}
constexpr size_mibyte_t operator"" _mibyte  (unsigned long long int aValue) {return size_mibyte_t::SMake(size_mibyte_t::value_type(aValue));}
constexpr size_KiB_t    operator"" _KiB     (unsigned long long int aValue) {return size_KiB_t::SMake(size_KiB_t::value_type(aValue));}
constexpr size_MiB_t    operator"" _MiB     (unsigned long long int aValue) {return size_MiB_t::SMake(size_MiB_t::value_type(aValue));}
constexpr size_GiB_t    operator"" _GiB     (unsigned long long int aValue) {return size_GiB_t::SMake(size_GiB_t::value_type(aValue));}
constexpr size_TiB_t    operator"" _TiB     (unsigned long long int aValue) {return size_TiB_t::SMake(size_TiB_t::value_type(aValue));}

}// namespace GPlatform
