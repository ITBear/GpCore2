#include "GpStringOps.hpp"

#if defined(GP_USE_STRINGS)

#include "../Numerics/GpNumericOps.hpp"
#include "../Bits/GpBitOps.hpp"

namespace GPlatform {

GpVector<GpRawPtrCharR> GpStringOps::SSplit (GpRawPtrCharR          aSourceStr,
                                             const char             aDelim,
                                             const count_t          aReturnPartsCountLimit,
                                             const count_t          aDelimCountLimit,
                                             const Algo::SplitMode  aSplitMode)
{
    auto a = GpRawPtrCharR(aSourceStr);
    auto b = GpRawPtrCharR(&aDelim, 1_cnt);

    return Algo::Split<char, GpVector<GpRawPtrCharR>>(a,
                                                      b,
                                                      aReturnPartsCountLimit,
                                                      aDelimCountLimit,
                                                      aSplitMode);
}

GpVector<GpRawPtrCharR> GpStringOps::SSplit (GpRawPtrCharR          aSourceStr,
                                             GpRawPtrCharR          aDelim,
                                             const count_t          aReturnPartsCountLimit,
                                             const count_t          aDelimCountLimit,
                                             const Algo::SplitMode  aSplitMode)
{
    return Algo::Split<char, GpVector<GpRawPtrCharR>>(GpRawPtrCharR(aSourceStr),
                                                      GpRawPtrCharR(aDelim),
                                                      aReturnPartsCountLimit,
                                                      aDelimCountLimit,
                                                      aSplitMode);
}

count_t GpStringOps::SFromUI64 (const u_int_64  aValue,
                                GpRawPtrCharRW  aStrOut)
{
    const count_t length = count_t::SMake(GpNumericOps::SDecDigsCountUI64(aValue));

    THROW_GPE_COND_CHECK_M(aStrOut.CountLeft() >= length, "aStrOut size are too small"_sv);

    _SFromUI64(aValue, GpRawPtrCharRW(aStrOut.Ptr(), length));

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

count_t GpStringOps::SFromSI64 (const s_int_64  aValue,
                                GpRawPtrCharRW  aStrOut)
{
    count_t length = 0_cnt;

    if (aValue >= 0)
    {
        const u_int_64 v = std::bit_cast<u_int_64>(aValue);
        length = count_t::SMake(GpNumericOps::SDecDigsCountUI64(v));

        THROW_GPE_COND_CHECK_M(aStrOut.CountLeft() >= length, "aStrOut size are too small"_sv);

        _SFromUI64(v, GpRawPtrCharRW(aStrOut.Ptr(), length));
    } else
    {
        const u_int_64 v =std::bit_cast<u_int_64>(-aValue);
        length = count_t::SMake(GpNumericOps::SDecDigsCountUI64(v));

        THROW_GPE_COND_CHECK_M(aStrOut.CountLeft() >= (length + 1_cnt), "StrOut size are too small"_sv);

        *aStrOut++ = '-';
        _SFromUI64(v, GpRawPtrCharRW(aStrOut.Ptr(), length));
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

        _SFromUI64(v, GpRawPtrCharRW(s));
    } else
    {
        const u_int_64 v = std::bit_cast<u_int_64>(-aValue);
        length = GpNumericOps::SDecDigsCountUI64(v);
        s.resize(length + 1);

        *s.data() = '-';
        _SFromUI64(v, GpRawPtrCharRW(s.data() + 1, length));
    }

    return s;
}

count_t GpStringOps::SFromDouble (const double      aValue,
                                  GpRawPtrCharRW    aStrOut)
{
    //TODO reimplement
    std::string s = std::to_string(aValue);

    const count_t length = count_t::SMake(s.length());

    THROW_GPE_COND_CHECK_M(aStrOut.CountLeft() >= length, "aMaxOutLength value are too small"_sv);

    SReplace(s, ',', '.');

    aStrOut.CopyFrom(s.data(), length);

    return length;
}

std::string GpStringOps::SFromDouble (const double aValue)
{
    //TODO reimplement
    std::string s = std::to_string(aValue);
    SReplace(s, ',', '.');
    return s;
}

u_int_64    GpStringOps::SToUI64 (GpRawPtrCharR aStr)
{
    UInt64          res = 0_u_int_64;
    const char*     str = aStr.Ptr();
    size_t          len = aStr.CountLeftV<size_t>();

    while (len-- > 0)
    {
        const char ch = *str++;

        if ((ch >= '0') && (ch <= '9'))
        {
            res = (res * UInt64::SMake(10)) + UInt64::SMake(u_int_64(ch) - u_int_64('0'));
        } else
        {
            THROW_GPE("Value "_sv + aStr.AsStringView() + " contains wrong character '"_sv + ch + "'"_sv);
        }
    }

    return res.ValueAs<u_int_64>();
}

s_int_64    GpStringOps::SToSI64 (GpRawPtrCharR aStr)
{
    GpRawPtrCharR str(aStr);

    const char signChar = *str;
    s_int_64 sign = 1;

    if (signChar == '-')
    {
        ++str;
        sign = -1;
    } else if (signChar == '+')
    {
        ++str;
        sign = 1;
    }

    const u_int_64 valueWithoutSign = SToUI64(str);

    if (sign >= 0)
    {
        if (valueWithoutSign > 9223372036854775807ULL)
        {
            THROW_GPE("Value "_sv + aStr.AsStringView() + " is out of range SInt64"_sv);
        } else
        {
            return SInt64::SBitCast(valueWithoutSign).ValueAs<s_int_64>();
        }
    } else
    {
        if (valueWithoutSign > 9223372036854775808ULL)
        {
            THROW_GPE("Value "_sv + aStr.AsStringView() + " is out of range SInt64"_sv);
        } else if (valueWithoutSign == 9223372036854775808ULL)
        {
            return std::bit_cast<s_int_64>(valueWithoutSign);//trick
        } else
        {
            return std::bit_cast<s_int_64>(valueWithoutSign) * sign;
        }
    }
}

double      GpStringOps::SToDouble_fast (GpRawPtrCharR aStr)
{
    //TODO: reimplement with http://www.netlib.org/fp/. Use dtoa.c
    //Supported format: [+-][UInt64][.[UInt64]]

    //----------------- Find decimal separator (radix character) -----------------
    GpVector<GpRawPtrCharR> parts = SSplit(aStr, '.', 2_cnt, 1_cnt, Algo::SplitMode::COUNT_ZERO_LENGTH_PARTS);
    GpRawPtrCharR integerPart;
    GpRawPtrCharR fractionalPart;

    if (parts.size() == 1)
    {
        integerPart = aStr;
    } else
    {
        integerPart     = parts.at(0);
        fractionalPart  = parts.at(1);
    }

    //----------------- Parse integer and fractional parts -----------------
    double res  = 0.0;
    double sign = 1.0;

    if (!integerPart.IsEmpty())
    {
        const char signChar = integerPart.At(0_cnt);
        if (signChar == '-')
        {
            sign = -1.0;
            integerPart = integerPart.SubrangeBeginOffset(1_cnt);//GpRawPtrCharR(integerPart.data() + 1, integerPart.size() - 1);
        } else if (signChar == '+')
        {
            sign = 1.0;
            integerPart = integerPart.SubrangeBeginOffset(1_cnt);//GpRawPtrCharR(integerPart.data() + 1, integerPart.size() - 1);
        }

        const u_int_64 v = (!integerPart.IsEmpty()) ? SToUI64(integerPart) : 0;
        res = double(v);
    }

    //Parse fractional part (to the right of the radix point)
    if (!fractionalPart.IsEmpty())
    {
        const u_int_64 v = SToUI64(fractionalPart);
        res += double(v)/pow(10.0, double(fractionalPart.CountLeftV<size_t>()));
    }

    return res * sign;
}

std::variant<s_int_64, double>  GpStringOps::SToNumeric (GpRawPtrCharR aStr)
{
    //[+-][UInt64][.[UInt64]] - DOUBLE
    //[+-]digits - INT

    std::string_view str = aStr.AsStringView();

    if (str.find_first_of('.') == std::string::npos)//int
    {
        const s_int_64 val = SToSI64(aStr);
        return {val};
    } else //double
    {
        const double val = SToDouble_fast(aStr);
        return {val};
    }
}

count_t GpStringOps::SFromBytes (GpRawPtrByteR  aData,
                                 GpRawPtrCharRW aStrOut)
{
    const count_t dataLength    = aData.CountLeft();
    const count_t strOutLength  = aStrOut.CountLeft();

    if (dataLength == 0_cnt)
    {
        return 0_cnt;
    }

    THROW_GPE_COND_CHECK_M(strOutLength >= (dataLength * 2_cnt), "Out string size is too small"_sv);

    size_t                  countLeft   = aData.CountLeftV<size_t>();
    const std::byte* _R_    data        = aData.Ptr();
    char* _R_               str         = aStrOut.Ptr();

    while (countLeft-- > 0)
    {
        const size_t b  = size_t(*data++);
        const size_t lo = (b & size_t(0x0F)) >> 0;
        const size_t hi = (b & size_t(0xF0)) >> 4;

        *str++ = char((hi < size_t(10)) ? (size_t('0')+ hi) : (size_t('a') + (hi - size_t(10))));
        *str++ = char((lo < size_t(10)) ? (size_t('0')+ lo) : (size_t('a') + (lo - size_t(10))));
    }

    return dataLength * 2_cnt;
}

std::string GpStringOps::SFromBytes (GpRawPtrByteR aData)
{
    const count_t charsCount = count_t::SMake(aData.SizeLeft().Value()) * 2_cnt;

    std::string res;
    res.resize(charsCount.ValueAs<size_t>());

    if (SFromBytes(aData, res) != charsCount)
    {
        THROW_GPE("Failed to convert bytes to hex string"_sv);
    }

    return res;
}

size_byte_t GpStringOps::SToBytes (GpRawPtrCharR    aStr,
                                   GpRawPtrByteRW   aDataOut)
{
    //String length
    const count_t strLength = aStr.CountLeft();

    if (strLength == 0_cnt)
    {
        return 0_byte;
    }

    THROW_GPE_COND_CHECK_M((strLength % 2_cnt) == 0_cnt, "String length must be even"_sv);

    //Remove prefix
    GpRawPtrCharR prefix    = aStr.Subrange(0_cnt, 2_cnt);
    GpRawPtrCharR strHex    = aStr;
    if ((prefix == "0x"_sv) ||
        (prefix == "0X"_sv))
    {
        if (aStr.CountLeft() <= 2_cnt)
        {
            return 0_byte;
        }

        strHex = aStr.SubrangeBeginOffset(2_cnt);
    }

    //
    GpRawPtrCharR strHexPtr(strHex);

    const count_t outSize = strHexPtr.CountLeft() / 2_cnt;
    THROW_GPE_COND_CHECK_M(aDataOut.CountLeft() >= outSize, "Out data size is too small"_sv);

    size_t          countLeft   = strHexPtr.CountLeftV<size_t>();
    const char* _R_ str         = strHexPtr.Ptr();

    while (countLeft > 0)
    {
        countLeft -= 2;
        GpArray<char,2> s = {*str++, *str++};
        *aDataOut++ = SToByte(s);
    }

    return size_byte_t::SMake(outSize.Value());
}

GpBytesArray    GpStringOps::SToBytes (GpRawPtrCharR aStr)
{
    const size_t size = aStr.CountLeftV<size_t>();

    GpBytesArray res;
    res.resize(size/2);
    res.resize(SToBytes(aStr, res).ValueAs<size_t>());
    return res;
}

count_t     GpStringOps::SConv_UTF16_UTF8 (GpArray<std::byte, 4>&       aUTF8_valueOut,
                                           const GpArray<std::byte, 2>  aUTF16_value)
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
        return 1_cnt;
    } else if (size_t(utf16_val) < size_t(0x00000800ULL))//0x00000080 — 0x000007FF
    {
        //110xxxxx 10xxxxxx
        *utf8_val++ = std::byte(size_t(0xC0) + ((size_t(utf16_val) >> 6) & size_t(0x1F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf16_val) >> 0) & size_t(0x3F)));
        *utf8_val++ = std::byte(0);
        *utf8_val++ = std::byte(0);
        return 2_cnt;
    } else
    {
        //1110xxxx 10xxxxxx 10xxxxxx
        *utf8_val++ = std::byte(size_t(0xE0) + ((size_t(utf16_val) >> 12) & size_t(0x0F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf16_val) >> 6)  & size_t(0x3F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf16_val) >> 0)  & size_t(0x3F)));
        *utf8_val++ = std::byte(0);
        return 3_cnt;
    }
}

count_t     GpStringOps::SConv_UTF32_UTF8 (GpArray<std::byte, 4>&       aUTF8_valueOut,
                                           const GpArray<std::byte, 4>  aUTF32_value)
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
        return 1_cnt;
    } else if (size_t(utf32_val) < size_t(0x00000800ULL))//0x00000080 — 0x000007FF
    {
        //110xxxxx 10xxxxxx
        *utf8_val++ = std::byte(size_t(0xC0) + ((size_t(utf32_val) >> 6) & size_t(0x1F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 0) & size_t(0x3F)));
        *utf8_val++ = std::byte(0);
        *utf8_val++ = std::byte(0);
        return 2_cnt;
    } else if (size_t(utf32_val) < size_t(0x00010000ULL))//0x00000800 — 0x0000FFFF
    {
        //1110xxxx 10xxxxxx 10xxxxxx
        *utf8_val++ = std::byte(size_t(0xE0) + ((size_t(utf32_val) >> 12) & size_t(0x0F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 6)  & size_t(0x3F)));
        *utf8_val++ = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 0)  & size_t(0x3F)));
        *utf8_val++ = std::byte(0);
        return 3_cnt;
    } else if (size_t(utf32_val) < size_t(0x00200000ULL))//0x00010000 — 0x001FFFFF
    {
        //11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
        utf8_val[0] = std::byte(size_t(0xF0) + ((size_t(utf32_val) >> 18) & size_t(0x07)));
        utf8_val[1] = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 12) & size_t(0x3F)));
        utf8_val[2] = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 6)  & size_t(0x3F)));
        utf8_val[3] = std::byte(size_t(0x80) + ((size_t(utf32_val) >> 0)  & size_t(0x3F)));
        return 4_cnt;
    } else
    {
        THROW_GPE("Wrong UTF32 value"_sv);
    }
}

std::string&    GpStringOps::SReplace (std::string& aStr, const char aChar, const char aNewChar)
{
    const size_t    len     = aStr.length();
    char* _R_       data    = aStr.data();

    for (size_t id = 0; id < len; ++id)
    {
        if (*data == aChar)
        {
            *data++ = aNewChar;
        } else
        {
            ++data;
        }
    }

    return aStr;
}

void    GpStringOps::_SFromUI64 (const u_int_64 aValue,
                                 GpRawPtrCharRW aStrOut)
{
    u_int_64        value   = aValue;
    const char* _R_ digits  = SDigits().data();
    char* _R_       str     = aStrOut.Ptr();

    if (value < 10)
    {
        *str = char(size_t('0') + size_t(value));
        return;
    }

    {
        const count_t countLeft = aStrOut.CountLeft();
        const count_t offset    = countLeft - 2_cnt;
        str += offset.ValueAs<size_t>();
    }

    while (value >= 100)
    {
        const size_t i = size_t((value % u_int_64(100)) * 2);
        value /= 100;

        MemOps::SCopy(str, digits + i, 2_cnt);
        str -= 2;
    }

    // Handle last 1-2 digits
    if (value < 10)
    {
        str++;
        *str = char(size_t('0') + size_t(value));
    } else
    {
        const size_t i = size_t(value) * 2;
        MemOps::SCopy(str, digits + i, 2_cnt);
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
