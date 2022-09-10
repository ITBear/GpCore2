#pragma once

#include "../GpMacro.hpp"

#if defined(GP_USE_DATE_TIME)

#include "../Types/Enums/GpEnum.hpp"
#include "../Types/Strings/GpStringOps.hpp"

namespace GPlatform {

GP_ENUM(GP_UTILS_API, GpDateTimeFormat,
    ISO_8601,       //2021-01-11T20:15:31+00:00
    RFC_2822,       //Mon, 11 Jan 2021 20:15:31 +0000
    STD_DATE_TIME,  //2021-01-11 20:15:31
    STD_DATE_TIME_T,//2021-01-11T20:15:31
    STD_DATE,       //2021-01-11
    STD_TIME        //20:15:31
);

}//GPlatform

#endif//#if defined(GP_USE_DATE_TIME)
