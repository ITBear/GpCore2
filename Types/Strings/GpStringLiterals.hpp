#pragma once

#include "../../Config/GpConfig.hpp"
#include "../../GpCore_global.hpp"

#if defined(GP_USE_STRINGS)

#include <string>
#include <string_view>

using namespace std::literals::string_literals;

namespace GPlatform {

inline constexpr std::string_view operator"" _sv (const char* aStr, const size_t aLen) noexcept
{
	return std::string_view(aStr, aLen);
}

inline std::string operator"" _s (const char* aStr, const size_t aLen)
{
	return std::string(aStr, aLen);
}

}//GPlatform

#endif//#if defined(GP_USE_STRINGS)
