#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include "../Pointers/GpSpan.hpp"

namespace GPlatform {

using GpSpanUI8_R   = GpSpan<const u_int_8>;
using GpSpanUI8_RW  = GpSpan<u_int_8>;
using GpSpanSI8_R   = GpSpan<const s_int_8>;
using GpSpanSI8_RW  = GpSpan<s_int_8>;
using GpSpanUI16_R  = GpSpan<const u_int_16>;
using GpSpanUI16_RW = GpSpan<u_int_16>;
using GpSpanSI16_R  = GpSpan<const s_int_16>;
using GpSpanSI16_RW = GpSpan<s_int_16>;
using GpSpanUI32_R  = GpSpan<const u_int_32>;
using GpSpanUI32_RW = GpSpan<u_int_32>;
using GpSpanSI32_R  = GpSpan<const s_int_32>;
using GpSpanSI32_RW = GpSpan<s_int_32>;
using GpSpanUI64_R  = GpSpan<const u_int_64>;
using GpSpanUI64_RW = GpSpan<u_int_64>;
using GpSpanSI64_R  = GpSpan<const s_int_64>;
using GpSpanSI64_RW = GpSpan<s_int_64>;

}// namespace GPlatform

#endif// #if defined(GP_USE_CONTAINERS)
