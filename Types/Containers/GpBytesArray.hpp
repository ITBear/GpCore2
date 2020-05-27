#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "GpContainersT.hpp"

namespace GPlatform {

using GpBytesArray = GpVector<std::byte>;

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
