#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "../Strings/GpStringOps.hpp"
#include "../Enums/GpEnums.hpp"

namespace GPlatform {

GP_ENUM(GPCORE_API, GpTypePropFlag,
    UNWRAP_CONTAINER,
    UNWRAP_CONTAINER_KEY
);

using GpTypePropFlags = GpEnumFlagsST<GpTypePropFlag>;

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
