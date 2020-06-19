#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_PIPELINE)

#include "../Types/Enums/GpEnums.hpp"

namespace GPlatform {

GP_ENUM(GPCORE_API, GpPipelineNodeSocketDir,
    IN,
    OUT
);

}//GPlatform

#endif//#if defined(GP_USE_PIPELINE)