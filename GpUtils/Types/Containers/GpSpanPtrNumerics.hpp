#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../Pointers/GpSpanPtr.hpp"

namespace GPlatform {

using GpSpanPtrUI8_R    = GpSpanPtr<const u_int_8*>;
using GpSpanPtrUI8_RW   = GpSpanPtr<u_int_8*>;
using GpSpanPtrSI8_R    = GpSpanPtr<const s_int_8*>;
using GpSpanPtrSI8_RW   = GpSpanPtr<s_int_8*>;
using GpSpanPtrUI16_R   = GpSpanPtr<const u_int_16*>;
using GpSpanPtrUI16_RW  = GpSpanPtr<u_int_16*>;
using GpSpanPtrSI16_R   = GpSpanPtr<const s_int_16*>;
using GpSpanPtrSI16_RW  = GpSpanPtr<s_int_16*>;
using GpSpanPtrUI32_R   = GpSpanPtr<const u_int_32*>;
using GpSpanPtrUI32_RW  = GpSpanPtr<u_int_32*>;
using GpSpanPtrSI32_R   = GpSpanPtr<const s_int_32*>;
using GpSpanPtrSI32_RW  = GpSpanPtr<s_int_32*>;
using GpSpanPtrUI64_R   = GpSpanPtr<const u_int_64*>;
using GpSpanPtrUI64_RW  = GpSpanPtr<u_int_64*>;
using GpSpanPtrSI64_R   = GpSpanPtr<const s_int_64*>;
using GpSpanPtrSI64_RW  = GpSpanPtr<s_int_64*>;

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
