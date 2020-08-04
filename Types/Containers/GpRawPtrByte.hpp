#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../Pointers/GpRawPtr.hpp"

namespace GPlatform {

using GpRawPtrCharR     = GpRawPtr<const char*>;
using GpRawPtrCharRW    = GpRawPtr<char*>;
using GpRawPtrByteR     = GpRawPtr<const std::byte*>;
using GpRawPtrByteRW    = GpRawPtr<std::byte*>;

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
