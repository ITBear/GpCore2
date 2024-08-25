#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_STRINGS)

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>

#include <string_view>

namespace GPlatform {

class GP_UTILS_API GpStringUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpStringUtils)

public:
    static void             SCout   (std::string_view aStr) noexcept;
    static void             SCerr   (std::string_view aStr) noexcept;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_STRINGS)
