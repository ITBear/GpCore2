#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../Pointers/GpRawPtr.hpp"

namespace GPlatform {

using GpRawPtrUI8_R     = GpRawPtr<const u_int_8*>;
using GpRawPtrUI8_RW    = GpRawPtr<u_int_8*>;
using GpRawPtrSI8_R     = GpRawPtr<const s_int_8*>;
using GpRawPtrSI8_RW    = GpRawPtr<s_int_8*>;
using GpRawPtrUI16_R    = GpRawPtr<const u_int_16*>;
using GpRawPtrUI16_RW   = GpRawPtr<u_int_16*>;
using GpRawPtrSI16_R    = GpRawPtr<const s_int_16*>;
using GpRawPtrSI16_RW   = GpRawPtr<s_int_16*>;
using GpRawPtrUI32_R    = GpRawPtr<const u_int_32*>;
using GpRawPtrUI32_RW   = GpRawPtr<u_int_32*>;
using GpRawPtrSI32_R    = GpRawPtr<const s_int_32*>;
using GpRawPtrSI32_RW   = GpRawPtr<s_int_32*>;
using GpRawPtrUI64_R    = GpRawPtr<const u_int_64*>;
using GpRawPtrUI64_RW   = GpRawPtr<u_int_64*>;
using GpRawPtrSI64_R    = GpRawPtr<const s_int_64*>;
using GpRawPtrSI64_RW   = GpRawPtr<s_int_64*>;

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
