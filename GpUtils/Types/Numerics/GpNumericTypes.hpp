#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <cstdint>
#include <cstddef>
#include <bit>

using s_int_8   = std::int8_t;
using u_int_8   = std::uint8_t;
using s_int_16  = std::int16_t;
using u_int_16  = std::uint16_t;
using s_int_32  = std::int32_t;
using u_int_32  = std::uint32_t;
using s_int_64  = std::int64_t;
using u_int_64  = std::uint64_t;

struct std_byte_no_init
{
    std_byte_no_init (void) noexcept
    {
        static_assert(sizeof(std_byte_no_init) == sizeof(std::byte));
        static_assert(alignof(std_byte_no_init) == alignof(std::byte));
    }

    std_byte_no_init (std::byte aValue) noexcept: value{aValue}
    {
    }

    std_byte_no_init (s_int_8 aValue) noexcept: value{std::bit_cast<std::byte>(aValue)}
    {
    }

    std_byte_no_init (u_int_8 aValue) noexcept: value{std::bit_cast<std::byte>(aValue)}
    {
    }

    friend bool operator== (const std_byte_no_init aValA, const std::byte aValB)
    {
        return aValA.value == aValB;
    }

    friend bool operator== (const std::byte aValA, const std_byte_no_init aValB)
    {
        return aValA == aValB.value;
    }

    friend bool operator== (const std_byte_no_init aValA, const u_int_8 aValB)
    {
        return aValA.value == std::bit_cast<std::byte>(aValB);
    }

    friend bool operator== (const u_int_8 aValA, const std_byte_no_init aValB)
    {
        return std::bit_cast<std::byte>(aValA) == aValB.value;
    }

    friend bool operator== (const std_byte_no_init aValA, const std_byte_no_init aValB)
    {
        return aValA.value == aValB.value;
    }

    std::byte value;
};

#if defined(GP_COMPILER_CLANG) || defined(GP_COMPILER_GCC)
    using s_int_128 = signed __int128;
    using u_int_128 = unsigned __int128;
#elif defined(GP_COMPILER_MSVC)
#   include <__msvc_int128.hpp>
#   include <BaseTsd.h>
    using s_int_128 = std::_Signed128;
    using u_int_128 = std::_Unsigned128;
    using ssize_t   = SSIZE_T;
#else
#   error Unsupported compiler
#endif

namespace std {

#if defined GP_COMPILER_MSVC
template<>
struct hash<u_int_128>
{
    std::size_t operator()(const u_int_128& aValue) const noexcept
    {
        static_assert
        (
               (sizeof(std::size_t) == sizeof(u_int_64))
            || (sizeof(std::size_t) == sizeof(u_int_32))
        );

        if constexpr(sizeof(std::size_t) == sizeof(u_int_64))
        {
            u_int_64 p1 = u_int_64(aValue >> 64);
            u_int_64 p2 = u_int_64(aValue >>  0);

            size_t hashValue;
            hashValue  = std::hash<u_int_64>()(p1);
            hashValue ^= std::hash<u_int_64>()(p2) + size_t{0x9e3779b97f4a7c15} + (hashValue << 6) + (hashValue >> 2);

            return hashValue;
        } else if constexpr(sizeof(std::size_t) == sizeof(u_int_32))
        {
            u_int_32 p1 = u_int_32(aValue >> 96);
            u_int_32 p2 = u_int_32(aValue >> 64);
            u_int_32 p3 = u_int_32(aValue >> 32);
            u_int_32 p4 = u_int_32(aValue >>  0);

            size_t hashValue;
            hashValue  = std::hash<u_int_32>()(p1);
            hashValue ^= std::hash<u_int_32>()(p2) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            hashValue ^= std::hash<u_int_32>()(p3) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
            hashValue ^= std::hash<u_int_32>()(p4) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);

            return hashValue;
        }
    }
};

template<>
struct hash<s_int_128>
{
    std::size_t operator()(const s_int_128& aValue) const noexcept
    {
        u_int_128 v = std::bit_cast<u_int_128>(aValue);
        return hash<u_int_128>{}(v);
    }
};

#endif// #if defined GP_COMPILER_MSVC

}// namespace std
