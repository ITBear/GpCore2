#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_STRINGS)

#include "../../GpUtils_global.hpp"
#include "../../Macro/GpMacroClass.hpp"
#include "../../../Config/GpCompilerFeatures.hpp"
#include <set>
#include <string>
#include <string_view>
#include <vector>
#include <cstring>

namespace GPlatform {

enum class GpUtf16Type
{
    REGULAR,
    HIGH_SURROGATE,
    LOW_SURROGATE
};

class GP_UTILS_API GpUTF
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpUTF)

public:
    static std::u16string               S_UTF8_To_UTF16     (std::u8string_view aStr);
    static std::u32string               S_UTF8_To_UTF32     (std::u8string_view aStr);
    static std::u8string                S_UTF16_To_UTF8     (std::u16string_view aStr);
    static std::u8string                S_UTF32_To_UTF8     (std::u32string_view aStr);

    static inline std::u8string_view    S_As_UTF8           (std::string_view aStr) noexcept;
    static inline std::u8string_view    S_As_UTF8           (std::u8string_view aStr) noexcept;
    static inline std::u8string_view    S_As_UTF8           (const char* aStr) noexcept;
    static inline std::u8string_view    S_As_UTF8           (const char*    aStr,
                                                             const size_t   aSize) noexcept;

    static inline std::u8string         S_To_UTF8           (std::string_view aStr);
    static inline std::u8string         S_To_UTF8           (std::string aStr);
    static inline std::u8string         S_To_UTF8           (std::u8string_view aStr);
    static inline std::u8string         S_To_UTF8           (std::u8string aStr);
    static inline std::u8string         S_To_UTF8           (const char* aStr);
    static inline std::u8string         S_To_UTF8           (const char*    aStr,
                                                             const size_t   aSize);

    static inline std::string_view      S_As_STR            (std::string_view aStr) noexcept;
    static inline std::string_view      S_As_STR            (std::u8string_view aStr) noexcept;
    static inline std::string_view      S_As_STR            (const char* aStr) noexcept;
    static inline std::string_view      S_As_STR            (const char*    aStr,
                                                             const size_t   aSize) noexcept;

    static inline std::string           S_To_STR            (std::string_view aStr);
    static inline std::string           S_To_STR            (std::string aStr);
    static inline std::string           S_To_STR            (std::u8string_view aStr);
    static inline std::string           S_To_STR            (std::u8string aStr);
    static inline std::string           S_To_STR            (const char* aStr);
    static inline std::string           S_To_STR            (const char*    aStr,
                                                             const size_t   aSize);

    static size_t                       SCharsCount         (std::u8string_view aStr);
    static size_t                       SCharsCount         (std::u16string_view aStr);

    static std::u8string                SToLower            (std::u8string_view aStr);
    static std::u8string                SToUpper            (std::u8string_view aStr);

    static inline constexpr size_t      SCharsCount         (std::u8string_view aStr,
                                                             const char8_t      aChar) noexcept;
    static size_t                       SCharsCount         (std::u8string_view             aStr,
                                                             const std::vector<char8_t>&    aChars) noexcept;
    static size_t                       SCharsCountRange    (std::u8string_view         aStr,
                                                             const char8_t              aCharFrom,
                                                             const char8_t              aCharTo) noexcept;
    static bool                         SContainsOnlyChars  (std::u8string_view         aStr,
                                                             const std::set<char8_t>&   aSet) noexcept;

    constexpr inline static GpUtf16Type SUtf16Type          (const char16_t aUTF16_char);
};

std::u8string_view  GpUTF::S_As_UTF8 (std::string_view aStr) noexcept
{
    return std::u8string_view
    (
        reinterpret_cast<const char8_t*>(aStr.data()),
        aStr.size()
    );
}

std::u8string_view  GpUTF::S_As_UTF8 (std::u8string_view aStr) noexcept
{
    return aStr;
}

std::u8string_view  GpUTF::S_As_UTF8 (const char* aStr) noexcept
{
    if (aStr == nullptr) [[unlikely]]
    {
        return {};
    }

    return std::u8string_view
    (
        reinterpret_cast<const char8_t*>(aStr),
        std::strlen(aStr)
    );
}

std::u8string_view  GpUTF::S_As_UTF8
(
    const char*     aStr,
    const size_t    aSize
) noexcept
{
    if (aStr == nullptr) [[unlikely]]
    {
        return {};
    }

    return std::u8string_view
    (
        reinterpret_cast<const char8_t*>(aStr),
        aSize
    );
}

std::u8string   GpUTF::S_To_UTF8 (std::string_view aStr)
{
    return std::u8string
    (
        reinterpret_cast<const char8_t*>(aStr.data()),
        aStr.size()
    );
}

std::u8string   GpUTF::S_To_UTF8 (std::string aStr)
{
    std::u8string& s = reinterpret_cast<std::u8string&>(aStr);// Dirty hack (UB)
    return std::u8string(std::move(s));
}

std::u8string   GpUTF::S_To_UTF8 (std::u8string_view aStr)
{
    return std::u8string(aStr);
}

std::u8string   GpUTF::S_To_UTF8 (std::u8string aStr)
{
    return std::u8string(std::move(aStr));
}

std::u8string   GpUTF::S_To_UTF8 (const char* aStr)
{
    if (aStr == nullptr) [[unlikely]]
    {
        return {};
    }

    return std::u8string
    (
        reinterpret_cast<const char8_t*>(aStr),
        std::strlen(aStr)
    );
}

std::u8string   GpUTF::S_To_UTF8
(
    const char*     aStr,
    const size_t    aSize
)
{
    if (aStr == nullptr) [[unlikely]]
    {
        return {};
    }

    return std::u8string
    (
        reinterpret_cast<const char8_t*>(aStr),
        aSize
    );
}

std::string_view    GpUTF::S_As_STR (std::string_view aStr) noexcept
{
    return aStr;
}

std::string_view    GpUTF::S_As_STR (std::u8string_view aStr) noexcept
{
    return std::string_view
    (
        reinterpret_cast<const char*>(aStr.data()),
        aStr.size()
    );
}

std::string_view    GpUTF::S_As_STR (const char* aStr) noexcept
{
    if (aStr == nullptr) [[unlikely]]
    {
        return {};
    }

    return std::string_view
    (
        reinterpret_cast<const char*>(aStr),
        std::strlen(aStr)
    );
}

std::string_view    GpUTF::S_As_STR
(
    const char*     aStr,
    const size_t    aSize
) noexcept
{
    if (aStr == nullptr) [[unlikely]]
    {
        return {};
    }

    return std::string_view
    (
        reinterpret_cast<const char*>(aStr),
        aSize
    );
}

std::string GpUTF::S_To_STR (std::string_view aStr)
{
    return std::string(aStr);
}

std::string GpUTF::S_To_STR (std::string aStr)
{
    return aStr;
}

std::string GpUTF::S_To_STR (std::u8string_view aStr)
{
    return std::string
    (
        reinterpret_cast<const char*>(aStr.data()),
        aStr.size()
    );
}

std::string GpUTF::S_To_STR (std::u8string aStr)
{
    std::string& s = reinterpret_cast<std::string&>(aStr);// Dirty hack (UB)
    return std::string(std::move(s));
}

std::string GpUTF::S_To_STR (const char* aStr)
{
    if (aStr == nullptr) [[unlikely]]
    {
        return {};
    }

    return std::string
    (
        reinterpret_cast<const char*>(aStr),
        std::strlen(aStr)
    );
}

std::string GpUTF::S_To_STR
(
    const char*     aStr,
    const size_t    aSize
)
{
    if (aStr == nullptr) [[unlikely]]
    {
        return {};
    }

    return std::string
    (
        reinterpret_cast<const char*>(aStr),
        aSize
    );
}

constexpr size_t    GpUTF::SCharsCount
(
    std::u8string_view  aStr,
    const char8_t       aChar
) noexcept
{
    //TODO: change aChar type from 'char8_t' to 'std::<char8_t, 4>'

    size_t              count       = 0;
    const char8_t* _R_  data        = aStr.data();
    const size_t        countLeft   = aStr.size();

    for (size_t id = 0; id < countLeft; ++id)
    {
        if (*data++ == aChar) [[unlikely]]
        {
            count++;
        }
    }

    return count;
}

constexpr GpUtf16Type   GpUTF::SUtf16Type (const char16_t   aUTF16_char)
{
    if (aUTF16_char >= char16_t(0xD800) && aUTF16_char <= char16_t(0xDBFF))
    {
        return GpUtf16Type::HIGH_SURROGATE;
    } else if (aUTF16_char >= char16_t(0xDC00) && aUTF16_char <= char16_t(0xDFFF))
    {
        return GpUtf16Type::LOW_SURROGATE;
    } else
    {
        return GpUtf16Type::REGULAR;
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_STRINGS)
