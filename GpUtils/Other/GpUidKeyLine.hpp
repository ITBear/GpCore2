#pragma once

#include <GpCore2/GpUtils/Types/Numerics/GpNumericTypes.hpp>

namespace GPlatform {

class GpUidKeyLine
{


public:
                                        GpUidKeyLine    (void) noexcept = delete;
                                        ~GpUidKeyLine   (void) noexcept = delete;

    static inline constexpr u_int_64    SMakeKey        (const char* aStr, u_int_32 aCounter) noexcept;

private:
    static inline constexpr u_int_64    SMakeKey        (u_int_32 aFileHash, u_int_32 aCounter) noexcept;
    static inline constexpr u_int_32    SFnv1a32        (const char* aStr, u_int_32 aSeed = u_int_32{0x811C9DC5u}) noexcept;
};

constexpr u_int_64  GpUidKeyLine::SMakeKey (const char* aStr, const u_int_32 aCounter) noexcept
{
    return SMakeKey(SFnv1a32(aStr), aCounter);
}

constexpr u_int_64  GpUidKeyLine::SMakeKey (u_int_32 aFileHash, const u_int_32 aCounter) noexcept
{
    return (u_int_64{aFileHash} << 32) | u_int_64{aCounter};
}

constexpr u_int_32  GpUidKeyLine::SFnv1a32 (const char* aStr, const u_int_32 aSeed) noexcept
{
    return *aStr
        ? SFnv1a32(aStr + 1, (aSeed ^ static_cast<std::uint8_t>(*aStr)) * u_int_32{16777619u})
        : aSeed;
}

} // namespace GPlatform

#define UID_KEY_LINE() ::GPlatform::GpUidKeyLine::SMakeKey(__FILE__, __COUNTER__)
