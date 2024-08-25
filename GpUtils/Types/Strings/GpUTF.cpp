#include <GpCore2/GpUtils/Types/Strings/GpUTF.hpp>

#if defined(GP_USE_STRINGS)

#include <GpCore2/GpUtils/Exceptions/GpExceptionCe.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>
#include <GpCore2/GpUtils/Types/Bits/GpBitOps.hpp>
#include <array>

namespace GPlatform {

//TODO: use fast check
//https://lemire.me/blog/2018/05/09/how-quickly-can-you-check-that-a-string-is-valid-unicode-utf-8/

std::u16string  GpUTF::S_UTF8_To_UTF16 (std::string_view aStr)
{
    std::u16string resStr;
    resStr.reserve(std::size(aStr));

    size_t          bytesLeft   = std::size(aStr);
    const char* _R_ utf8Str     = std::data(aStr);

    while (bytesLeft > 0)
    {
        const char      char8           = *utf8Str;
        const size_t    leading1bitCnt  = std::max<size_t>(BitOps::Leading1bitCnt(u_int_8(char8)), 1);

        THROW_COND_GP
        (
               (bytesLeft >= leading1bitCnt)
            && (leading1bitCnt >= 1)
            && (leading1bitCnt <= 4),
            "Invalid UTF-8 sequence"_sv
        );

        if (leading1bitCnt == 1)
        {
            resStr.push_back(char16_t(char8));
        } else if (leading1bitCnt == 2)
        {
            //Byte 0: 110xxxxx
            //Byte 1: 10yyyyyy

            const u_int_32 char32 =   u_int_32((u_int_32(char8)      & u_int_32(0b00011111)) << 6)
                                    | u_int_32((u_int_32(utf8Str[1]) & u_int_32(0b00111111)) << 0);

            resStr.push_back(char16_t(char32));
        } else if (leading1bitCnt == 3)
        {
            //Byte 0: 1110xxxx
            //Byte 1: 10yyyyyy
            //Byte 2: 10yyyyyy

            const u_int_32 char32 =   u_int_32((u_int_32(char8)      & u_int_32(0b00001111)) << 12)
                                    | u_int_32((u_int_32(utf8Str[1]) & u_int_32(0b00111111)) <<  6)
                                    | u_int_32((u_int_32(utf8Str[2]) & u_int_32(0b00111111)) <<  0);

            resStr.push_back(char16_t(char32));
        } else //if (leading1bitCnt == 4)
        {
            //Byte 0: 11110xxx
            //Byte 1: 10yyyyyy
            //Byte 2: 10yyyyyy
            //Byte 3: 10yyyyyy

            const u_int_32 char32 =   u_int_32((u_int_32(char8)      & u_int_32(0b00000111)) << 18)
                                    | u_int_32((u_int_32(utf8Str[1]) & u_int_32(0b00111111)) << 12)
                                    | u_int_32((u_int_32(utf8Str[2]) & u_int_32(0b00111111)) <<  6)
                                    | u_int_32((u_int_32(utf8Str[3]) & u_int_32(0b00111111)) <<  0);

            THROW_COND_GP
            (
                char32 >= 0x10000,
                "Wrong UTF-8 sequence"_sv
            );

            const char16_t highSurrogate    = char16_t((u_int_32((char32 & u_int_32(~0b10000000000000000)) >> 10) & u_int_32(0b1111111111))
                                                       | u_int_32(0b1101100000000000));
            const char16_t lowSurrogate     = char16_t((u_int_32((char32 & u_int_32(~0b10000000000000000)) >>  0) & u_int_32(0b1111111111))
                                                       | u_int_32(0b1101110000000000));

            resStr.push_back(highSurrogate);
            resStr.push_back(lowSurrogate);
        }

        utf8Str     += leading1bitCnt;
        bytesLeft   -= leading1bitCnt;
    }

    return resStr;
}

std::u32string  GpUTF::S_UTF8_To_UTF32 (std::string_view /*aStr*/)
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

std::string GpUTF::S_UTF16_To_UTF8 (std::u16string_view aStr)
{
    std::string resStr;

    const size_t strLegth = std::size(aStr);
    resStr.reserve(NumOps::SMul<size_t>(strLegth, 2));

    GpUtf16Type prevType    = GpUtf16Type::REGULAR;
    char32_t    code        = 0;

    for (size_t id = 0; id < strLegth; id++)
    {
        const char16_t      val0    = aStr[id];
        const GpUtf16Type   type    = SUtf16Type(val0);

        if (type == GpUtf16Type::REGULAR)
        {
            THROW_COND_GP
            (
                prevType != GpUtf16Type::HIGH_SURROGATE,
                "Invalid UTF-16 character"_sv
            );

            code        = val0;
            prevType    = type;
        } else if (type == GpUtf16Type::HIGH_SURROGATE)
        {
            THROW_COND_GP
            (
                prevType != GpUtf16Type::HIGH_SURROGATE,
                "Invalid UTF-16 high surrogate character"_sv
            );

            code        = val0;
            prevType    = type;

            continue;
        } else //val0Type == GpUtf16Type::LOW_SURROGATE
        {
            THROW_COND_GP
            (
                prevType == GpUtf16Type::HIGH_SURROGATE,
                "Invalid UTF-16 low surrogate character"_sv
            );

            code        = (code - char32_t(0xD800)) * char32_t(0x400) + (char32_t(val0) - char32_t(0xDC00)) + char32_t(0x10000);
            prevType    = type;
        }

        if (code <= char32_t(0x7F))
        {
            resStr.push_back(char(code));
        } else if (code <= char32_t(0x7FF))
        {
            resStr.push_back(char(char32_t(0xC0) | char32_t(char32_t(code >>  6) & char32_t(0xFFFFFFFF))));
            resStr.push_back(char(char32_t(0x80) | char32_t(char32_t(code >>  0) & char32_t(0x0000003F))));
        } else if (code <= char32_t(0xFFFF))
        {
            resStr.push_back(char(char32_t(0xE0) | char32_t(char32_t(code >> 12) & char32_t(0xFFFFFFFF))));
            resStr.push_back(char(char32_t(0x80) | char32_t(char32_t(code >>  6) & char32_t(0x0000003F))));
            resStr.push_back(char(char32_t(0x80) | char32_t(char32_t(code >>  0) & char32_t(0x0000003F))));
        } else
        {
            resStr.push_back(char(char32_t(0xF0) | char32_t(char32_t(code >> 18) & char32_t(0xFFFFFFFF))));
            resStr.push_back(char(char32_t(0x80) | char32_t(char32_t(code >> 12) & char32_t(0x0000003F))));
            resStr.push_back(char(char32_t(0x80) | char32_t(char32_t(code >>  6) & char32_t(0x0000003F))));
            resStr.push_back(char(char32_t(0x80) | char32_t(char32_t(code >>  0) & char32_t(0x0000003F))));
        }
    }

    return resStr;
}

std::string GpUTF::S_UTF32_To_UTF8 (std::u32string_view aStr)
{
    std::string resStr;

    const size_t strLegth = std::size(aStr);
    resStr.reserve(NumOps::SMul<size_t>(strLegth, 4));

    for (char32_t code: aStr)
    {
        if (code <= 0x7F)
        {
            resStr.push_back(static_cast<char>(code));
        } else if (code <= 0x7FF)
        {
            resStr.push_back(static_cast<char>(0xC0 | ((code >> 6) & 0x1F)));
            resStr.push_back(static_cast<char>(0x80 | (code & 0x3F)));
        } else if (code <= 0xFFFF)
        {
            resStr.push_back(static_cast<char>(0xE0 | ((code >> 12) & 0x0F)));
            resStr.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
            resStr.push_back(static_cast<char>(0x80 | (code & 0x3F)));
        } else if (code <= 0x10FFFF)
        {
            resStr.push_back(static_cast<char>(0xF0 | ((code >> 18) & 0x07)));
            resStr.push_back(static_cast<char>(0x80 | ((code >> 12) & 0x3F)));
            resStr.push_back(static_cast<char>(0x80 | ((code >> 6) & 0x3F)));
            resStr.push_back(static_cast<char>(0x80 | (code & 0x3F)));
        } else
        {
            THROW_GP("Invalid UTF-32 code point");
        }
    }

    return resStr;
}

size_t  GpUTF::SCharsCount (std::string_view aStr)
{
    size_t          charsCount  = 0;
    size_t          bytesLeft   = std::size(aStr);
    const char* _R_ utf8Str     = std::data(aStr);

    while (bytesLeft > 0)
    {
        const char      char8           = *utf8Str;
        const size_t    leading1bitCnt  = std::max<size_t>(BitOps::Leading1bitCnt(u_int_8(char8)), 1);

        THROW_COND_GP
        (
               (bytesLeft >= leading1bitCnt)
            && (leading1bitCnt >= 1)
            && (leading1bitCnt <= 4),
            "Invalid UTF-8 sequence"_sv
        );

        utf8Str     += leading1bitCnt;
        bytesLeft   -= leading1bitCnt;
        charsCount  += 1;
    }

    return charsCount;
}

size_t  GpUTF::SCharsCount (std::u16string_view aStr)
{
    size_t              charsCount  = 0;
    size_t              charsLeft   = std::size(aStr);
    const char16_t* _R_ utf16Str    = std::data(aStr);
    GpUtf16Type         prevType    = GpUtf16Type::REGULAR;

    while (charsLeft > 0)
    {
        const char16_t      char16      = *utf16Str++;
        const GpUtf16Type   charType    = SUtf16Type(char16);
        charsLeft--;

        switch (charType)
        {
            case GpUtf16Type::REGULAR:
            {
                THROW_COND_GP
                (
                    prevType != GpUtf16Type::HIGH_SURROGATE,
                    "Invalid UTF-16 high surrogate character"_sv
                );

                charsCount++;
            } break;
            case GpUtf16Type::HIGH_SURROGATE:
            {
                THROW_COND_GP
                (
                    prevType != GpUtf16Type::HIGH_SURROGATE,
                    "Invalid UTF-16 high surrogate character"_sv
                );
            } break;
            case GpUtf16Type::LOW_SURROGATE:
            {
                THROW_COND_GP
                (
                    prevType == GpUtf16Type::HIGH_SURROGATE,
                    "Invalid UTF-16 low surrogate character"_sv
                );

                charsCount++;
            } break;
            default:
            {
                // NOP
            } break;
        }

        prevType = charType;
    }

    THROW_COND_GP
    (
           (prevType == GpUtf16Type::REGULAR)
        || (prevType == GpUtf16Type::LOW_SURROGATE),
        "Invalid UTF-16 surrogate character"_sv
    );

    return charsCount;
}

std::string GpUTF::SToLower (std::string_view aStr)
{
    //TODO: reimplement with common template code with SToUpper

    std::string resStr;
    resStr.reserve(std::size(aStr));

    size_t          bytesLeft   = std::size(aStr);
    const char* _R_ utf8Str     = std::data(aStr);

    while (bytesLeft > 0)
    {
        const char      char8           = *utf8Str;
        const size_t    leading1bitCnt  = std::max<size_t>(BitOps::Leading1bitCnt(u_int_8(char8)), 1);

        THROW_COND_GP
        (
               (bytesLeft >= leading1bitCnt)
            && (leading1bitCnt >= 1)
            && (leading1bitCnt <= 4),
            "Invalid UTF-8 sequence"_sv
        );

        std::array<char, 4> charData;
        charData.fill(char(0));
        std::memcpy(std::data(charData), utf8Str, leading1bitCnt);

        utf8Str     += leading1bitCnt;
        bytesLeft   -= leading1bitCnt;

        {
            int _tmp;
            std::memcpy(&_tmp, std::data(charData), 4);
            _tmp = std::tolower(_tmp);
            std::memcpy(std::data(charData), &_tmp, 4);
        }

        resStr.append(std::string_view(std::data(charData), leading1bitCnt));
    }

    return resStr;
}

std::string GpUTF::SToUpper (std::string_view aStr)
{
    // TODO: reimplement with common template code with SToLower

    std::string resStr;
    resStr.reserve(std::size(aStr));

    size_t          bytesLeft   = std::size(aStr);
    const char* _R_ utf8Str     = std::data(aStr);

    while (bytesLeft > 0)
    {
        const char      char8           = *utf8Str;
        const size_t    leading1bitCnt  = std::max<size_t>(BitOps::Leading1bitCnt(u_int_8(char8)), 1);

        THROW_COND_GP
        (
               (bytesLeft >= leading1bitCnt)
            && (leading1bitCnt >= 1)
            && (leading1bitCnt <= 4),
            "Invalid UTF-8 sequence"_sv
        );

        std::array<char, 4> charData;
        charData.fill(char(0));
        std::memcpy(std::data(charData), utf8Str, leading1bitCnt);

        utf8Str     += leading1bitCnt;
        bytesLeft   -= leading1bitCnt;

        {
            int _tmp;
            std::memcpy(&_tmp, std::data(charData), 4);
            _tmp = std::toupper(_tmp);
            std::memcpy(std::data(charData), &_tmp, 4);
        }

        resStr.append(std::string_view(std::data(charData), leading1bitCnt));
    }

    return resStr;
}

size_t  GpUTF::SCharsCount
(
    std::string_view            /*aStr*/,
    const std::vector<char>&    /*aChars*/
) noexcept
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    //TODO: reimplement with SIMD

    /*size_t        count       = 0;
    const char* _R_ data        = std::data(aStr);
    const size_t    ountLeft    = std::size(aStr);

    for (size_t id = 0; id < countLeft; ++id)
    {
        const char ch = *data++;

        for (const char targetCh: aChars)
        {
            count += 1 * size_t{ch == targetCh};
        }
    }

    return count;*/
}

size_t  GpUTF::SCharsCountRange
(
    std::string_view    /*aStr*/,
    const char          /*aCharFrom*/,
    const char          /*aCharTo*/
) noexcept
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*
    size_t          count       = 0;
    const char* _R_ data        = std::data(aStr);
    const size_t    countLeft   = std::size(aStr);

    for (size_t id = 0; id < countLeft; ++id)
    {
        const char ch = *data++;

        count += 1 * size_t{ch >= aCharFrom} * size_t{ch <= aCharTo};
    }

    return count;
    */
}

bool    GpUTF::SContainsOnlyChars
(
    std::string_view        /*aStr*/,
    const std::set<char>&   /*aSet*/
) noexcept
{
    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();

    /*
    size_t size = std::size(aStr);

    if (size == 0)
    {
        return true;
    }

    const char* _R_ ptr = std::data(aStr);

    while (size > 0)
    {
        size--;

        if (aSet.count(*ptr++) == 0)
        {
            return false;
        }
    }

    return true;*/
}

}// namespace GPlatform

/*
size_t  GpUTF::S_UTF16_To_UTF8
(
    const char16_t          aUTF16_value0,
    const char16_t          aUTF16_value1,
    std::array<char, 4>&    aUTF8_valueOut
)
{
    char* utf8_val = std::data(aUTF8_valueOut);

    //Convert to UTF8
    if (size_t{aUTF32_value} < size_t{0x00000080ULL})//0x00000000 — 0x0000007F
    {
        //0xxxxxxx
        *utf8_val++ = char(aUTF32_value);
        *utf8_val++ = char(0);
        *utf8_val++ = char(0);
        *utf8_val++ = char(0);
        return 1;
    } else if (size_t{aUTF32_value} < size_t{0x00000800ULL})//0x00000080 — 0x000007FF
    {
        //110xxxxx 10xxxxxx
        *utf8_val++ = char(size_t{0xC0} + ((size_t{aUTF32_value} >> 6) & size_t{0x1F}));
        *utf8_val++ = char(size_t{0x80} + ((size_t{aUTF32_value} >> 0) & size_t{0x3F}));
        *utf8_val++ = char(0);
        *utf8_val++ = char(0);
        return 2;
    } else if (size_t{aUTF32_value} < size_t{0x00010000ULL})//0x00000800 — 0x0000FFFF
    {
        //1110xxxx 10xxxxxx 10xxxxxx
        *utf8_val++ = char(size_t{0xE0} + ((size_t{aUTF32_value} >> 12) & size_t{0x0F}));
        *utf8_val++ = char(size_t{0x80} + ((size_t{aUTF32_value} >> 6)  & size_t{0x3F}));
        *utf8_val++ = char(size_t{0x80} + ((size_t{aUTF32_value} >> 0)  & size_t{0x3F}));
        *utf8_val++ = char(0);
        return 3;
    } else if (size_t{aUTF32_value} < size_t{0x00200000ULL})//0x00010000 — 0x001FFFFF
    {
        //11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        utf8_val[0] = char(size_t{0xF0} + ((size_t{aUTF32_value} >> 18) & size_t{0x07}));
        utf8_val[1] = char(size_t{0x80} + ((size_t{aUTF32_value} >> 12) & size_t{0x3F}));
        utf8_val[2] = char(size_t{0x80} + ((size_t{aUTF32_value} >> 6)  & size_t{0x3F}));
        utf8_val[3] = char(size_t{0x80} + ((size_t{aUTF32_value} >> 0)  & size_t{0x3F}));
        return 4;
    } else
    {
        THROW_GP("Wrong UTF32 value"_sv);
    }
}

size_t  GpUTF::S_UTF32_To_UTF8
(
    const char32_t          aUTF32_value,
    std::array<char, 4>&    aUTF8_valueOut
)
{
    char* utf8_val = std::data(aUTF8_valueOut);

    //Convert to UTF8
    if (size_t{aUTF32_value} < size_t{0x00000080ULL})//0x00000000 — 0x0000007F
    {
        //0xxxxxxx
        *utf8_val++ = char(aUTF32_value);
        *utf8_val++ = char(0);
        *utf8_val++ = char(0);
        *utf8_val++ = char(0);
        return 1;
    } else if (size_t{aUTF32_value} < size_t{0x00000800ULL})//0x00000080 — 0x000007FF
    {
        //110xxxxx 10xxxxxx
        *utf8_val++ = char(size_t{0xC0} + ((size_t{aUTF32_value} >> 6) & size_t{0x1F}));
        *utf8_val++ = char(size_t{0x80} + ((size_t{aUTF32_value} >> 0) & size_t{0x3F}));
        *utf8_val++ = char(0);
        *utf8_val++ = char(0);
        return 2;
    } else if (size_t{aUTF32_value} < size_t{0x00010000ULL})//0x00000800 — 0x0000FFFF
    {
        //1110xxxx 10xxxxxx 10xxxxxx
        *utf8_val++ = char(size_t{0xE0} + ((size_t{aUTF32_value} >> 12) & size_t{0x0F}));
        *utf8_val++ = char(size_t{0x80} + ((size_t{aUTF32_value} >> 6)  & size_t{0x3F}));
        *utf8_val++ = char(size_t{0x80} + ((size_t{aUTF32_value} >> 0)  & size_t{0x3F}));
        *utf8_val++ = char(0);
        return 3;
    } else if (size_t{(}aUTF32_value} < size_t{0x00200000ULL})//0x00010000 — 0x001FFFFF
    {
        //11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        utf8_val[0] = char(size_t{0xF0} + ((size_t{aUTF32_value} >> 18) & size_t{0x07}));
        utf8_val[1] = char(size_t{0x80} + ((size_t{aUTF32_value} >> 12) & size_t{0x3F}));
        utf8_val[2] = char(size_t{0x80} + ((size_t{aUTF32_value} >> 6)  & size_t{0x3F}));
        utf8_val[3] = char(size_t{0x80} + ((size_t{aUTF32_value} >> 0)  & size_t{0x3F}));
        return 4;
    } else
    {
        THROW_GP("Wrong UTF32 value"_sv);
    }
}
*/

#endif// #if defined(GP_USE_STRINGS)
