#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_STRINGS)

#include <string>
#include <string_view>

using namespace std::literals::string_literals;

inline constexpr std::u8string_view operator"" _sv (const char8_t* aStr, const size_t aLen) noexcept
{
    return std::u8string_view(aStr, aLen);
}

inline constexpr std::string_view operator"" _sv (const char* aStr, const size_t aLen) noexcept
{
    return std::string_view(aStr, aLen);
}

inline std::u8string operator"" _s (const char8_t* aStr, const size_t aLen)
{
    return std::u8string(aStr, aLen);
}

inline std::string operator"" _s (const char* aStr, const size_t aLen)
{
    return std::string(aStr, aLen);
}

#endif//#if defined(GP_USE_STRINGS)
