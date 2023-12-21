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
    static inline std::u8string_view    S_STR_To_UTF8       (std::string_view aStr) noexcept;
    static inline std::u8string_view    S_STR_To_UTF8       (const char* aStr) noexcept;
    static inline std::u8string_view    S_STR_To_UTF8       (const char*    aStr,
                                                             const size_t   aSize) noexcept;
    static inline std::string_view      S_UTF8_To_STR       (std::u8string_view aStr) noexcept;
    static inline std::u8string_view    S_To_UTF8           (std::u8string_view aStr) noexcept;
    static inline std::u8string_view    S_To_UTF8           (std::string_view aStr) noexcept;

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

std::u8string_view  GpUTF::S_STR_To_UTF8 (std::string_view aStr) noexcept
{
    return std::u8string_view
    (
        reinterpret_cast<const char8_t*>(aStr.data()),
        aStr.size()
    );
}

std::u8string_view  GpUTF::S_STR_To_UTF8 (const char* aStr) noexcept
{
    if (aStr == nullptr)
    {
        return {};
    }

    return std::u8string_view
    (
        reinterpret_cast<const char8_t*>(aStr),
        std::strlen(aStr)
    );
}

std::u8string_view  GpUTF::S_STR_To_UTF8
(
    const char*     aStr,
    const size_t    aSize
) noexcept
{
    if (aStr == nullptr)
    {
        return {};
    }

    return std::u8string_view
    (
        reinterpret_cast<const char8_t*>(aStr),
        aSize
    );
}

std::string_view    GpUTF::S_UTF8_To_STR (std::u8string_view aStr) noexcept
{
    return std::string_view
    (
        reinterpret_cast<const char*>(aStr.data()),
        aStr.size()
    );
}

std::u8string_view  GpUTF::S_To_UTF8 (std::u8string_view aStr) noexcept
{
    return aStr;
}

std::u8string_view  GpUTF::S_To_UTF8 (std::string_view aStr) noexcept
{
    return S_STR_To_UTF8(aStr);
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
        if (*data++ == aChar)
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
