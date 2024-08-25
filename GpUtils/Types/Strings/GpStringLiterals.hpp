#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_STRINGS)

#include <string_view>

using namespace std::literals::string_literals;

inline constexpr std::string_view operator"" _sv (const char* aStr, const size_t aLen) noexcept
{
    return std::string_view(aStr, aLen);
}

#endif// #if defined(GP_USE_STRINGS)
