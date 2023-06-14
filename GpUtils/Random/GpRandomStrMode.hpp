#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

#include "../Types/Strings/GpStringOps.hpp"
#include "../Types/Strings/GpUTF.hpp"
#include "../Types/Enums/GpEnum.hpp"

namespace GPlatform {

GP_ENUM(GP_UTILS_API, GpRandomStrMode,
    ALPHA_NUM,
    NUM
);

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
