#pragma once

#include "../GpCore_global.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

#include "../Types/Strings/GpStringOps.hpp"
#include "../Types/Enums/GpEnum.hpp"

namespace GPlatform {

GP_ENUM(GPCORE_API, GpRandomStrMode,
    ALPHA_NUM,
    NUM
);

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
