#pragma once

#include "../../Config/GpConfig.hpp"

#include <bit>
#include <version>
#include <cstring>

#if !defined(__cpp_lib_bit_cast)

namespace std {
	template <class To, class From> constexpr
	typename std::enable_if_t<
		(sizeof(To) == sizeof(From)) &&
		std::is_trivially_copyable_v<From> &&
		std::is_trivial_v<To> &&
		(std::is_copy_constructible_v<To> || std::is_move_constructible_v<To>),
		To>
	bit_cast(const From& aSrc) noexcept
	{
		To dst;
		std::memcpy(&dst, &aSrc, sizeof(To));
		return dst;
	}
}//std

#endif//#if !defined(__cpp_lib_bit_cast)
