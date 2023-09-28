#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_STRINGS)

#include "../../GpUtils_global.hpp"
#include "../../Macro/GpMacroClass.hpp"

#include <string_view>

namespace GPlatform {

class GP_UTILS_API GpStringUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpStringUtils)

public:
    static void             SCout       (std::string_view   aStr);
    static void             SCout       (std::u8string_view aStr);

    static void             SCerr       (std::string_view   aStr);
    static void             SCerr       (std::u8string_view aStr);
};

}//GPlatform

#endif//#if defined(GP_USE_STRINGS)
