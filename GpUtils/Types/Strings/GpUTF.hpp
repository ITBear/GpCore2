#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Exceptions/GpExceptionCe.hpp>

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
    inline static std::string           S_WStr_To_Str       (std::wstring_view aStr);
    static std::u16string               S_UTF8_To_UTF16     (std::string_view aStr);
    static std::u32string               S_UTF8_To_UTF32     (std::string_view aStr);
    static std::string                  S_UTF16_To_UTF8     (std::u16string_view aStr);
    static std::string                  S_UTF32_To_UTF8     (std::u32string_view aStr);

    static size_t                       SCharsCount         (std::string_view aStr);
    static size_t                       SCharsCount         (std::u16string_view aStr);

    static std::string                  SToLower            (std::string_view   aStr);
    static std::string                  SToUpper            (std::string_view   aStr);

    static inline constexpr size_t      SCharsCount         (std::string_view   aStr,
                                                             const char         aChar) noexcept;
    static size_t                       SCharsCount         (std::string_view           aStr,
                                                             const std::vector<char>&   aChars) noexcept;
    static size_t                       SCharsCountRange    (std::string_view   aStr,
                                                             const char         aCharFrom,
                                                             const char         aCharTo) noexcept;
    static bool                         SContainsOnlyChars  (std::string_view       aStr,
                                                             const std::set<char>&  aSet) noexcept;

    constexpr inline static GpUtf16Type SUtf16Type          (const char16_t aUTF16_char);
};

std::string GpUTF::S_WStr_To_Str (std::wstring_view aStr)
{
    if constexpr (sizeof(wchar_t) == 1)
    {
        return std::string(reinterpret_cast<const char*>(std::data(aStr)), std::size(aStr));
    } else if constexpr (sizeof(wchar_t) == 2)
    {
        return S_UTF16_To_UTF8(std::u16string_view(reinterpret_cast<const char16_t*>(std::data(aStr)), std::size(aStr)));
    } else if constexpr (sizeof(wchar_t) == 4)
    {
        return S_UTF32_To_UTF8(std::u32string_view(reinterpret_cast<const char32_t*>(std::data(aStr)), std::size(aStr)));
    } else
    {
        GpThrowCe<int>(0);
    }
}

/*
std::string_view    GpUTF::S_As_UTF8 (std::string_view aStr) noexcept
{
    return std::string_view
    (
        reinterpret_cast<const char*>(std::data(aStr)),
        std::size(aStr)
    );
}

std::string_view    GpUTF::S_As_UTF8 (std::string_view aStr) noexcept
{
    return aStr;
}

std::string_view    GpUTF::S_As_UTF8 (const char* aStr) noexcept
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

std::string_view    GpUTF::S_As_UTF8
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

std::string GpUTF::S_To_UTF8 (std::string_view aStr)
{
    return std::string
    (
        reinterpret_cast<const char*>(std::data(aStr)),
        std::size(aStr)
    );
}

std::string GpUTF::S_To_UTF8 (std::string aStr)
{
    std::string& s = reinterpret_cast<std::string&>(aStr);// Dirty hack (UB)
    return std::string(std::move(s));
}

std::string GpUTF::S_To_UTF8 (std::string_view aStr)
{
    return std::string(aStr);
}

std::string GpUTF::S_To_UTF8 (std::string aStr)
{
    return std::string(std::move(aStr));
}

std::string GpUTF::S_To_UTF8 (const char* aStr)
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

std::string GpUTF::S_To_UTF8
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

std::string_view    GpUTF::S_As_STR (std::string_view aStr) noexcept
{
    return aStr;
}

std::string_view    GpUTF::S_As_STR (std::string_view aStr) noexcept
{
    return std::string_view
    (
        reinterpret_cast<const char*>(std::data(aStr)),
        std::size(aStr)
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

std::string GpUTF::S_To_STR (std::string_view aStr)
{
    return std::string
    (
        reinterpret_cast<const char*>(std::data(aStr)),
        std::size(aStr)
    );
}

std::string GpUTF::S_To_STR (std::string aStr)
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
*/

constexpr size_t    GpUTF::SCharsCount
(
    std::string_view    aStr,
    const char          aChar
) noexcept
{
    //TODO: change aChar type from 'char' to 'std::<char, 4>'

    size_t          count       = 0;
    const char* _R_ data        = std::data(aStr);
    const size_t    countLeft   = std::size(aStr);

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
