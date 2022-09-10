#include "GpStringOps.hpp"

#if defined(GP_USE_STRINGS)

#include "../Numerics/GpNumericOps.hpp"
#include "../Bits/GpBitOps.hpp"

#include <charconv>
#include <sstream>

namespace GPlatform {

GpVector<std::string_view>  GpStringOps::SSplit
(
    std::string_view        aSourceStr,
    const char              aDelim,
    const size_t            aReturnPartsCountLimit,
    const size_t            aDelimCountLimit,
    const Algo::SplitMode   aSplitMode
)
{
    return Algo::Split<char, GpVector<std::string_view>>
    (
        GpSpanPtrCharR(aSourceStr),
        GpSpanPtrCharR(&aDelim, 1),
        aReturnPartsCountLimit,
        aDelimCountLimit,
        aSplitMode
    );
}

GpVector<std::string_view>  GpStringOps::SSplit
(
    std::string_view        aSourceStr,
    std::string_view        aDelim,
    const size_t            aReturnPartsCountLimit,
    const size_t            aDelimCountLimit,
    const Algo::SplitMode   aSplitMode
)
{
    return Algo::Split<char, GpVector<std::string_view>>
    (
        GpSpanPtrCharR(aSourceStr),
        GpSpanPtrCharR(aDelim),
        aReturnPartsCountLimit,
        aDelimCountLimit,
        aSplitMode
    );
}

size_t  GpStringOps::SFromUI64
(
    const u_int_64  aValue,
    GpSpanPtrCharRW aStrOut
)
{
    const size_t length = GpNumericOps::SDecDigsCountUI64(aValue);

    THROW_COND_GP
    (
        aStrOut.Count() >= length,
        "aStrOut size are too small"_sv
    );

    _SFromUI64(aValue, GpSpanPtrCharRW(aStrOut.Ptr(), length));

    return length;
}

std::string GpStringOps::SFromUI64 (const u_int_64 aValue)
{
    const size_t length = GpNumericOps::SDecDigsCountUI64(aValue);
    std::string s;
    s.resize(length);

    _SFromUI64(aValue, s);

    return s;
}

size_t  GpStringOps::SFromSI64
(
    const s_int_64  aValue,
    GpSpanPtrCharRW aStrOut
)
{
    size_t length = 0;

    if (aValue >= 0)
    {
        const u_int_64 v = std::bit_cast<u_int_64>(aValue);
        length = GpNumericOps::SDecDigsCountUI64(v);

        THROW_COND_GP
        (
            aStrOut.Count() >= length,
            "aStrOut size are too small"_sv
        );

        _SFromUI64(v, GpSpanPtrCharRW(aStrOut.Ptr(), length));
    } else
    {
        const u_int_64 v =std::bit_cast<u_int_64>(-aValue);
        length = GpNumericOps::SDecDigsCountUI64(v);

        THROW_COND_GP
        (
            aStrOut.Count() >= (length + 1),
            "StrOut size are too small"_sv
        );

        *aStrOut++ = '-';
        _SFromUI64(v, GpSpanPtrCharRW(aStrOut.Ptr(), length));
        length++;
    }

    return length;
}

std::string GpStringOps::SFromSI64 (const s_int_64 aValue)
{
    std::string s;
    size_t      length = 0;

    if (aValue >= 0)
    {
        const u_int_64 v = std::bit_cast<u_int_64>(aValue);
        length = GpNumericOps::SDecDigsCountUI64(v);
        s.resize(length);

        _SFromUI64(v, GpSpanPtrCharRW(s));
    } else
    {
        const u_int_64 v = std::bit_cast<u_int_64>(-aValue);
        length = GpNumericOps::SDecDigsCountUI64(v);
        s.resize(length + 1);

        *s.data() = '-';
        _SFromUI64(v, GpSpanPtrCharRW(s.data() + 1, length));
    }

    return s;
}

size_t  GpStringOps::SFromDouble
(
    const double    aValue,
    GpSpanPtrCharRW aStrOut
)
{
    //TODO reimplement
    std::string s = std::to_string(aValue);

    const size_t length = s.length();

    THROW_COND_GP
    (
        aStrOut.Count() >= length,
        "aMaxOutLength value are too small"_sv
    );

    std::replace(s.begin(), s.end(), ',', '.');

    MemOps::SCopy(aStrOut.Ptr(), s.data(), length);

    return length;
}

std::string GpStringOps::SFromDouble (const double aValue)
{
    //TODO reimplement
    std::string s = std::to_string(aValue);
    std::replace(s.begin(), s.end(), ',', '.');
    return s;
}

u_int_64    GpStringOps::SToUI64
(
    std::string_view                                aStr,
    std::optional<std::reference_wrapper<size_t>>   aReadCountOut
)
{
    u_int_64    result      = 0;
    const char* strPtrBegin = aStr.data();

    const std::from_chars_result r = std::from_chars
    (
        strPtrBegin,
        strPtrBegin + aStr.size(),
        result
    );

    if (r.ec == std::errc())
    {
        if (aReadCountOut.has_value())
        {
            const size_t readCount = size_t(r.ptr - strPtrBegin);
            aReadCountOut.value() += readCount;
        }

        return result;
    } else if (r.ec == std::errc::invalid_argument)
    {
        THROW_GP("Wrong string value"_sv);
    } else if (r.ec == std::errc::result_out_of_range)
    {
        THROW_GP("Out of u_int_64 range"_sv);
    } else
    {
        THROW_GP("std::errc() = "_sv + int(std::errc()));
    }

    return result;

    //UInt64        res         = 0_u_int_64;
    //const char*   str         = aStr.Ptr();
    //size_t        len         = aStr.CountLeft().As<size_t>();
    //size_t        readLength  = 0;
    //
    //THROW_COND_GP
    //(
    //  len > 0,
    //  "Empty string"_sv
    //);
    //
    //while (len-- > 0)
    //{
    //  const char ch = *str++;
    //
    //  if (('0' <= ch) && (ch <= '9'))
    //  {
    //      readLength++;
    //      res = (res * UInt64::SMake(10)) + UInt64::SMake(u_int_64(ch) - u_int_64('0'));
    //  } else
    //  {
    //      break;
    //  }
    //}
    //
    //if (aReadCountOut.has_value())
    //{
    //  aReadCountOut.value().get() += readLength;
    //} else
    //{
    //  THROW_COND_GP
    //  (
    //      readLength > 0,
    //      "Wrong string value for u_int_64"_sv
    //  );
    //}
    //
    //return res.As<u_int_64>();
}

s_int_64    GpStringOps::SToSI64
(
    std::string_view                                aStr,
    std::optional<std::reference_wrapper<size_t>>   aReadCountOut
)
{
    s_int_64    result      = 0;
    const char* strPtrBegin = aStr.data();

    const std::from_chars_result r = std::from_chars
    (
        strPtrBegin,
        strPtrBegin + aStr.size(),
        result
    );

    if (r.ec == std::errc())
    {
        if (aReadCountOut.has_value())
        {
            const size_t readCount = size_t(r.ptr - strPtrBegin);
            aReadCountOut.value() += readCount;
        }

        return result;
    } else if (r.ec == std::errc::invalid_argument)
    {
        THROW_GP("Wrong string value"_sv);
    } else if (r.ec == std::errc::result_out_of_range)
    {
        THROW_GP("Out of s_int_64 range"_sv);
    } else
    {
        THROW_GP("std::errc() = "_sv + int(std::errc()));
    }

    return result;
}

double      GpStringOps::SToDouble
(
    std::string_view                                aStr,
    std::optional<std::reference_wrapper<size_t>>   aReadCountOut
)
{
    double      result      = 0.0;
    const char* strPtrBegin = aStr.data();

    const std::from_chars_result r = std::from_chars
    (
        strPtrBegin,
        strPtrBegin + aStr.size(),
        result,
        std::chars_format::fixed
    );

    if (r.ec == std::errc())
    {
        if (aReadCountOut.has_value())
        {
            const size_t readCount = size_t(r.ptr - strPtrBegin);
            aReadCountOut.value() += readCount;
        }

        return result;
    } else if (r.ec == std::errc::invalid_argument)
    {
        THROW_GP("Wrong string value"_sv);
    } else if (r.ec == std::errc::result_out_of_range)
    {
        THROW_GP("Out of double range"_sv);
    } else
    {
        THROW_GP("std::errc() = "_sv + int(std::errc()));
    }

    return result;
}

std::variant<s_int_64, double>  GpStringOps::SToNumeric
(
    std::string_view                                aStr,
    std::optional<std::reference_wrapper<size_t>>   aReadCountOut
)
{
    size_t                          readCount   = 0;
    std::variant<s_int_64, double>  res         = SToSI64(aStr, readCount);

    if (   (aStr.length() > readCount)
        && (aStr[readCount] == '.'))
    {
        readCount = 0;
        res = SToDouble(aStr, readCount);
    }

    if (aReadCountOut.has_value())
    {
        aReadCountOut.value() += readCount;
    }

    return res;
}

size_t  GpStringOps::SFromBytesHex
(
    GpSpanPtrByteR  aData,
    GpSpanPtrCharRW aStrOut
)
{
    const size_t dataLength     = aData.Count();
    const size_t strOutLength   = aStrOut.Count();

    if (dataLength == 0)
    {
        return 0;
    }

    const size_t resSize = NumOps::SMul(dataLength, size_t(2));

    THROW_COND_GP
    (
        strOutLength >= resSize,
        "Out string size is too small"_sv
    );

    size_t                  countLeft   = dataLength;
    const std::byte* _R_    dataPtr     = aData.Ptr();
    char* _R_               strPtr          = aStrOut.Ptr();

    while (countLeft-- > 0)
    {
        const size_t b  = size_t(*dataPtr++);
        const size_t lo = (b & size_t(0x0F)) >> 0;
        const size_t hi = (b & size_t(0xF0)) >> 4;

        *strPtr++ = char((hi < size_t(10)) ? (size_t('0')+ hi) : (size_t('a') + (hi - size_t(10))));
        *strPtr++ = char((lo < size_t(10)) ? (size_t('0')+ lo) : (size_t('a') + (lo - size_t(10))));
    }

    return resSize;
}

std::string GpStringOps::SFromBytesHex (GpSpanPtrByteR aData)
{
    const size_t charsCount = NumOps::SMul(aData.Size().As<size_t>(), size_t(2));

    std::string res;
    res.resize(charsCount);

    if (SFromBytesHex(aData, res) != charsCount)
    {
        THROW_GP("Failed to convert bytes to hex string"_sv);
    }

    return res;
}

size_byte_t GpStringOps::SToBytesHex
(
    std::string_view    aStr,
    GpSpanPtrByteRW     aDataOut
)
{
    //String length
    const size_t strLength = aStr.size();

    if (strLength == 0)
    {
        return 0_byte;
    }

    THROW_COND_GP
    (
        (strLength % 2) == 0,
        "String length must be even"_sv
    );

    //Remove prefix
    std::string_view prefix = aStr.substr(0, 2);
    std::string_view strHex = aStr;
    if ((prefix == "0x"_sv) ||
        (prefix == "0X"_sv))
    {
        if (aStr.size() <= 2)
        {
            return 0_byte;
        }

        strHex = aStr.substr(2);
    }

    //
    const size_t outSize = strHex.size() / 2;
    THROW_COND_GP
    (
        aDataOut.Count() >= outSize,
        "Out data size is too small"_sv
    );

    size_t          countLeft   = strHex.size();
    const char* _R_ str         = strHex.data();

    while (countLeft > 0)
    {
        countLeft -= 2;
        GpArray<char,2> s = {*str++, *str++};
        *aDataOut++ = SToByteHex(s);
    }

    return size_byte_t::SMake(outSize);
}

GpBytesArray    GpStringOps::SToBytesHex (std::string_view aStr)
{
    const size_t size = aStr.size();

    GpBytesArray res;
    res.resize(size/2);
    res.resize(SToBytesHex(aStr, res).As<size_t>());
    return res;
}

std::string GpStringOps::SFromBits (GpSpanPtrByteR aData)
{
    std::string res;

    const size_t    bytesCount  = aData.Count();
    const size_t    bitsCount   = (size_bit_t::SMake(aData.Count()) * 8_bit).As<size_t>();
    const u_int_8*  data        = aData.PtrAs<const u_int_8*>();

    res.resize(bitsCount + bytesCount + 1);
    char* str = res.data();

    *str++ = '|';
    for (size_t byteId = 0; byteId < bytesCount; byteId++)
    {
        const u_int_8 b = *data++;

        for (size_t bitId = 0; bitId < 8; bitId++)
        {
            *str++ = (b & (1 << (7 - (bitId % 8)))) ? '1' : '0';
        }

        *str++ = '|';
    }

    return res;
}

size_t      GpStringOps::SConv_UTF16_UTF8
(
    GpArray<std::byte, 4>&      aUTF8_valueOut,
    const GpArray<std::byte, 2> aUTF16_value
)
{
    const u_int_16  utf16_val   = BitOps::N2H(std::bit_cast<u_int_16>(aUTF16_value));
    std::byte*      utf8_val    = aUTF8_valueOut.data();

    //Convert to UTF8
    if (size_t(utf16_val) < size_t(0x00000080ULL))//0x00000000 — 0x0000007F
    {
        //0xxxxxxx
        *utf8_val++ = std::byte(utf16_val);
        *utf8_val++ = std::byte(0);
        *utf8_val++ = std::byte(0);
        *utf8_val++ = std::byte(0);
        return 1;
    } else if (size_t(utf16_val) < size_t(0x00000800ULL))//0x00000080 — 0x000007FF
    {
        //110xxxxx 10xxxxxx
        *utf8_val++ = std::byte(size_t(0xC0) + ((size_t(utf16_val) >> 6) & size_t(0x1F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf16_val) >> 0) & size_t(0x3F)));
        *utf8_val++ = std::byte(0);
        *utf8_val++ = std::byte(0);
        return 2;
    } else
    {
        //1110xxxx 10xxxxxx 10xxxxxx
        *utf8_val++ = std::byte(size_t(0xE0) + ((size_t(utf16_val) >> 12) & size_t(0x0F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf16_val) >> 6)  & size_t(0x3F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf16_val) >> 0)  & size_t(0x3F)));
        *utf8_val++ = std::byte(0);
        return 3;
    }
}

size_t      GpStringOps::SConv_UTF32_UTF8
(
    GpArray<std::byte, 4>&      aUTF8_valueOut,
    const GpArray<std::byte, 4> aUTF32_value
)
{
    const u_int_32  utf32_val   = BitOps::N2H(std::bit_cast<u_int_32>(aUTF32_value));
    std::byte*      utf8_val    = aUTF8_valueOut.data();

    //Convert to UTF8
    if (size_t(utf32_val) < size_t(0x00000080ULL))//0x00000000 — 0x0000007F
    {
        //0xxxxxxx
        *utf8_val++ = std::byte(utf32_val);
        *utf8_val++ = std::byte(0);
        *utf8_val++ = std::byte(0);
        *utf8_val++ = std::byte(0);
        return 1;
    } else if (size_t(utf32_val) < size_t(0x00000800ULL))//0x00000080 — 0x000007FF
    {
        //110xxxxx 10xxxxxx
        *utf8_val++ = std::byte(size_t(0xC0) + ((size_t(utf32_val) >> 6) & size_t(0x1F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 0) & size_t(0x3F)));
        *utf8_val++ = std::byte(0);
        *utf8_val++ = std::byte(0);
        return 2;
    } else if (size_t(utf32_val) < size_t(0x00010000ULL))//0x00000800 — 0x0000FFFF
    {
        //1110xxxx 10xxxxxx 10xxxxxx
        *utf8_val++ = std::byte(size_t(0xE0) + ((size_t(utf32_val) >> 12) & size_t(0x0F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 6)  & size_t(0x3F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 0)  & size_t(0x3F)));
        *utf8_val++ = std::byte(0);
        return 3;
    } else if (size_t(utf32_val) < size_t(0x00200000ULL))//0x00010000 — 0x001FFFFF
    {
        //11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        utf8_val[0] = std::byte(size_t(0xF0) + ((size_t(utf32_val) >> 18) & size_t(0x07)));
        utf8_val[1] = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 12) & size_t(0x3F)));
        utf8_val[2] = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 6)  & size_t(0x3F)));
        utf8_val[3] = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 0)  & size_t(0x3F)));
        return 4;
    } else
    {
        THROW_GP("Wrong UTF32 value"_sv);
    }
}

size_t  GpStringOps::SLength_UTF8 (std::string_view aStr)
{
    size_t                  charsCount  = 0;
    ssize_t                 bytesLeft   = NumOps::SConvert<ssize_t>(aStr.size());
    const std::byte* _R_    utf8Str     = reinterpret_cast<const std::byte*>(aStr.data());

    while (bytesLeft > 0)
    {
        const std::byte byte        = *utf8Str;
        const size_t    charSize    = BitOps::Leading1bitCnt(byte | std::byte(0b10000000));

        utf8Str     += charSize;
        bytesLeft   -= charSize;
        charsCount  += 1;
    }

    return charsCount;
}

std::string GpStringOps::PercentEncode (std::string_view aSrc)
{
    const char*         srcPtr          = aSrc.data();
    std::string         encodedStr;
    encodedStr.resize(NumOps::SMul<size_t>(aSrc.size(), 3));
    char*               encodedStrPtr   = encodedStr.data();
    size_t              usedSize        = 0;
    size_t              unprocessedSize = aSrc.size();

    while (unprocessedSize > 0)
    {
        unprocessedSize--;
        const char ch = *srcPtr++;

        if (size_t(ch) < 127)
        {
            if (   ((ch >= '0') && (ch <= '9'))
                || ((ch >= 'A') && (ch <= 'Z'))
                || ((ch >= 'a') && (ch <= 'z'))
                || (ch == '-')
                || (ch == '_')
                || (ch == '.')
                || (ch == '~'))
            {
                *encodedStrPtr++ = ch;
                usedSize += 1;
                continue;
            }
        }

        const auto hexStr = StrOps::SFromByteHex(std::byte(ch));
        GpArray<char, 3> buff;
        buff.at(0) = '%';
        buff.at(1) = hexStr.at(0);
        buff.at(2) = hexStr.at(1);

        std::memcpy(encodedStrPtr, buff.data(), buff.size());
        encodedStrPtr   += buff.size();
        usedSize        += buff.size();
    }

    encodedStr.resize(usedSize);

    return encodedStr;
}

size_t  GpStringOps::SCountChars
(
    std::string_view            aStr,
    const std::vector<char>&    aChars
) noexcept
{
    //TODO: reimplement with SIMD

    size_t          count       = 0;
    const char* _R_ data        = aStr.data();
    const size_t    countLeft   = aStr.size();

    for (size_t id = 0; id < countLeft; ++id)
    {
        const char ch = *data++;

        for (const char targetCh: aChars)
        {
            count += 1 * size_t(ch == targetCh);
        }
    }

    return count;
}

size_t  GpStringOps::SCountCharsRange
(
    std::string_view    aStr,
    const char          aCharFrom,
    const char          aCharTo
) noexcept
{
    size_t          count       = 0;
    const char* _R_ data        = aStr.data();
    const size_t    countLeft   = aStr.size();

    for (size_t id = 0; id < countLeft; ++id)
    {
        const char ch = *data++;

        count += 1 * size_t(ch >= aCharFrom) * size_t(ch <= aCharTo);
    }

    return count;
}

/*void  GpStringOps::SToLower (GpRawPtrCharRW aStr)
{
    if (aStr.IsEmpty())
    {
        return;
    }

    char* _R_       str     = aStr.Ptr();
    const size_t    size    = aStr.SizeLeft().As<size_t>();

    for (size_t id = 0; id < size; ++id)
    {
        const char ch = *str;
        *str++ = static_cast<char>(std::tolower(ch));
    }
}

void    GpStringOps::SToUpper (GpRawPtrCharRW aStr)
{
    if (aStr.IsEmpty())
    {
        return;
    }

    char* _R_       str     = aStr.Ptr();
    const size_t    size    = aStr.SizeLeft().As<size_t>();

    for (size_t id = 0; id < size; ++id)
    {
        const char ch = *str;
        *str++ = static_cast<char>(std::toupper(ch));
    }
}*/

bool    GpStringOps::SContainsOnly
(
    std::string_view    aStr,
    const GpSet<char>&  aSet
) noexcept
{
    size_t size = aStr.size();

    if (size == 0)
    {
        return true;
    }

    const char* _R_ ptr = aStr.data();

    while (size > 0)
    {
        size--;

        if (aSet.count(*ptr++) == 0)
        {
            return false;
        }
    }

    return true;
}

std::string GpStringOps::SToString (const std::thread::id aThreadId)
{
    std::ostringstream ss;
    ss << aThreadId;
    return  ss.str();
}

std::string GpStringOps::SToString (const void* aPtr)
{
    std::ostringstream ss;
    ss << aPtr;
    return  ss.str();
}

void    GpStringOps::_SFromUI64
(
    const u_int_64  aValue,
    GpSpanPtrCharRW aStrOut
)
{
    u_int_64        value   = aValue;
    const char* _R_ digits  = SDigits().data();
    char* _R_       strPtr  = aStrOut.Ptr();

    if (value < 10)
    {
        *strPtr = char(size_t('0') + size_t(value));
        return;
    }

    {
        const size_t    countLeft   = aStrOut.Count();
        const size_t    offset      = NumOps::SSub(countLeft, size_t(2));
        strPtr += offset;
    }

    while (value >= 100)
    {
        const size_t i = size_t((value % u_int_64(100)) * 2);
        value /= 100;

        MemOps::SCopy(strPtr, digits + i, 2);
        strPtr -= 2;
    }

    // Handle last 1-2 digits
    if (value < 10)
    {
        strPtr++;
        *strPtr = char(size_t('0') + size_t(value));
    } else
    {
        const size_t i = size_t(value) * 2;
        MemOps::SCopy(strPtr, digits + i, 2);
    }
}

const GpArray<char, 201>&   GpStringOps::SDigits (void) noexcept
{
    static const GpArray<char, 201> sDigits =
    {
        "0001020304050607080910111213141516171819"
        "2021222324252627282930313233343536373839"
        "4041424344454647484950515253545556575859"
        "6061626364656667686970717273747576777879"
        "8081828384858687888990919293949596979899"
    };

    return sDigits;
}

}//GPlatform

#endif//#if defined(GP_USE_STRINGS)
