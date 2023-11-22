#include "GpStringOps.hpp"

#if defined(GP_USE_STRINGS)

#include <charconv>
#include <sstream>

namespace GPlatform {

std::vector<std::u8string_view> GpStringOps::SSplit
(
    std::u8string_view      aSourceStr,
    const char8_t           aDelim,
    const size_t            aReturnPartsCountLimit,
    const size_t            aDelimCountLimit,
    const Algo::SplitMode   aSplitMode
)
{
    return Algo::Split<char, std::vector<std::u8string_view>>
    (
        GpSpanPtrCharU8R(aSourceStr),
        GpSpanPtrCharU8R(&aDelim, 1),
        aReturnPartsCountLimit,
        aDelimCountLimit,
        aSplitMode
    );
}

std::vector<std::u8string_view> GpStringOps::SSplit
(
    std::u8string_view      aSourceStr,
    std::u8string_view      aDelim,
    const size_t            aReturnPartsCountLimit,
    const size_t            aDelimCountLimit,
    const Algo::SplitMode   aSplitMode
)
{
    return Algo::Split<char, std::vector<std::u8string_view>>
    (
        GpSpanPtrCharU8R(aSourceStr),
        GpSpanPtrCharU8R(aDelim),
        aReturnPartsCountLimit,
        aDelimCountLimit,
        aSplitMode
    );
}

std::vector<std::string_view>   GpStringOps::SSplit
(
    std::string_view        aSourceStr,
    const char              aDelim,
    const size_t            aReturnPartsCountLimit,
    const size_t            aDelimCountLimit,
    const Algo::SplitMode   aSplitMode
)
{
    return Algo::Split<char, std::vector<std::string_view>>
    (
        GpSpanPtrCharR(aSourceStr),
        GpSpanPtrCharR(&aDelim, 1),
        aReturnPartsCountLimit,
        aDelimCountLimit,
        aSplitMode
    );
}

std::vector<std::string_view>   GpStringOps::SSplit
(
    std::string_view        aSourceStr,
    std::string_view        aDelim,
    const size_t            aReturnPartsCountLimit,
    const size_t            aDelimCountLimit,
    const Algo::SplitMode   aSplitMode
)
{
    return Algo::Split<char, std::vector<std::string_view>>
    (
        GpSpanPtrCharR(aSourceStr),
        GpSpanPtrCharR(aDelim),
        aReturnPartsCountLimit,
        aDelimCountLimit,
        aSplitMode
    );
}

bool    GpStringOps::SIsEqualCaseInsensitive8bit
(
    std::u8string_view  aStr1,
    std::u8string_view  aStr2
) noexcept
{
    return std::equal
    (
        aStr1.begin(),
        aStr1.end(),
        aStr2.begin(),
        aStr2.end(),
        [](const char8_t a, const char8_t b)
        {
            return std::tolower(int(a)) == std::tolower(int(b));
        }
    );
}

size_t  GpStringOps::SFromUI64
(
    const u_int_64      aValue,
    GpSpanPtrCharU8RW   aStrOut
)
{
    const size_t length = GpNumericOps::SDecDigsCountUI64(aValue);

    THROW_COND_GP
    (
        aStrOut.Count() >= length,
        u8"aStrOut size are too small"_sv
    );

    _SFromUI64(aValue, GpSpanPtrCharU8RW(aStrOut.Ptr(), length));

    return length;
}

std::u8string   GpStringOps::SFromUI64 (const u_int_64 aValue)
{
    const size_t length = GpNumericOps::SDecDigsCountUI64(aValue);
    std::u8string s;
    s.resize(length);

    _SFromUI64(aValue, s);

    return s;
}

size_t  GpStringOps::SFromSI64
(
    const s_int_64      aValue,
    GpSpanPtrCharU8RW   aStrOut
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
            u8"aStrOut size are too small"_sv
        );

        _SFromUI64(v, GpSpanPtrCharU8RW(aStrOut.Ptr(), length));
    } else
    {
        const u_int_64 v =std::bit_cast<u_int_64>(-aValue);
        length = GpNumericOps::SDecDigsCountUI64(v);

        THROW_COND_GP
        (
            aStrOut.Count() >= (length + 1),
            u8"StrOut size are too small"_sv
        );

        *aStrOut++ = '-';
        _SFromUI64(v, GpSpanPtrCharU8RW(aStrOut.Ptr(), length));
        length++;
    }

    return length;
}

std::u8string   GpStringOps::SFromSI64 (const s_int_64 aValue)
{
    std::u8string   s;
    size_t          length = 0;

    if (aValue >= 0)
    {
        const u_int_64 v = std::bit_cast<u_int_64>(aValue);
        length = GpNumericOps::SDecDigsCountUI64(v);
        s.resize(length);

        _SFromUI64(v, GpSpanPtrCharU8RW(s));
    } else
    {
        const u_int_64 v = std::bit_cast<u_int_64>(-aValue);
        length = GpNumericOps::SDecDigsCountUI64(v);
        s.resize(length + 1);

        *s.data() = '-';
        _SFromUI64(v, GpSpanPtrCharU8RW(s.data() + 1, length));
    }

    return s;
}

size_t  GpStringOps::SFromDouble
(
    const double        aValue,
    GpSpanPtrCharU8RW   aStrOut
)
{
    //TODO reimplement
    const std::string   tmpS    = std::to_string(aValue);
    std::u8string       s       = std::u8string(reinterpret_cast<const char8_t*>(tmpS.data()), tmpS.size());

    const size_t length = s.length();

    THROW_COND_GP
    (
        aStrOut.Count() >= length,
        u8"aMaxOutLength value are too small"_sv
    );

    std::replace(s.begin(), s.end(), ',', '.');

    MemOps::SCopy(aStrOut.Ptr(), s.data(), length);

    return length;
}

std::u8string   GpStringOps::SFromDouble (const double aValue)
{
    //TODO reimplement
    const std::string   tmpS    = std::to_string(aValue);
    std::u8string       s       = std::u8string(reinterpret_cast<const char8_t*>(tmpS.data()), tmpS.size());

    std::replace(s.begin(), s.end(), ',', '.');
    return s;
}

u_int_64    GpStringOps::SToUI64
(
    std::u8string_view                              aStr,
    std::optional<std::reference_wrapper<size_t>>   aReadCountOut
)
{
    u_int_64    result      = 0;
    const char* strPtrBegin = reinterpret_cast<const char*>(aStr.data());

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
        THROW_GP(u8"Wrong source string value '"_sv + aStr + u8"'"_sv);
    } else if (r.ec == std::errc::result_out_of_range)
    {
        THROW_GP(u8"Out of u_int_64 range: '"_sv + aStr + u8"'"_sv);
    } else
    {
        THROW_GP(u8"std::errc() = "_sv + int(std::errc()) + u8". Source string '"_sv + aStr + u8"'"_sv);
    }

    return result;
}

s_int_64    GpStringOps::SToSI64
(
    std::u8string_view                              aStr,
    std::optional<std::reference_wrapper<size_t>>   aReadCountOut
)
{
    s_int_64    result      = 0;
    const char* strPtrBegin = reinterpret_cast<const char*>(aStr.data());
    size_t      strSize     = aStr.size();

    if ((strSize > 0) && (strPtrBegin != nullptr) && (*strPtrBegin == '+'))
    {
        strPtrBegin++;
        strSize--;
    }

    const std::from_chars_result r = std::from_chars
    (
        strPtrBegin,
        strPtrBegin + strSize,
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
        THROW_GP(u8"Wrong source string value '"_sv + aStr + u8"'"_sv);
    } else if (r.ec == std::errc::result_out_of_range)
    {
        THROW_GP(u8"Out of s_int_64 range: '"_sv + aStr + u8"'"_sv);
    } else
    {
        THROW_GP(u8"std::errc() = "_sv + int(std::errc()) + u8". Source string '"_sv + aStr + u8"'"_sv);
    }

    return result;
}

double      GpStringOps::SToDouble
(
    std::u8string_view                              aStr,
    std::optional<std::reference_wrapper<size_t>>   aReadCountOut
)
{
    double result = 0.0;

#if __cplusplus < CPP_VERSION_23
    const std::string s(reinterpret_cast<const char*>(aStr.data()), aStr.size());
    char* end{};
    result = std::strtod(s.data(), &end);

    const size_t readCount = size_t(end - s.data());

    if (readCount != s.size())
    {
        THROW_GP(u8"Wrong source string value '"_sv + aStr + u8"'"_sv);
    }

    if (aReadCountOut.has_value())
    {
        aReadCountOut.value() += readCount;
    }
#else
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
        THROW_GP(u8"Wrong source string value '"_sv + aStr + u8"'"_sv);
    } else if (r.ec == std::errc::result_out_of_range)
    {
        THROW_GP(u8"Out of double range: '"_sv + aStr + u8"'"_sv);
    } else
    {
        THROW_GP(u8"std::errc() = "_sv + int(std::errc()) + u8". Source string '"_sv + aStr + u8"'"_sv);
    }
#endif//

    return result;
}

std::variant<s_int_64, double>  GpStringOps::SToNumeric
(
    std::u8string_view                              aStr,
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
    GpSpanPtrByteR      aData,
    GpSpanPtrCharU8RW   aStrOut
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
        u8"Out string size is too small"_sv
    );

    size_t              countLeft   = dataLength;
    const u_int_8* _R_  dataPtr     = aData.Ptr();
    char8_t* _R_        strPtr      = aStrOut.Ptr();

    THROW_COND_GP
    (
        strPtr != nullptr,
        u8"aStrOut is null"_sv
    );

    while (countLeft-- > 0)
    {
        const size_t b  = size_t(*dataPtr++);
        const size_t lo = (b & size_t(0x0F)) >> 0;
        const size_t hi = (b & size_t(0xF0)) >> 4;

        *strPtr++ = char8_t((hi < size_t(10)) ? (size_t('0')+ hi) : (size_t('a') + (hi - size_t(10))));
        *strPtr++ = char8_t((lo < size_t(10)) ? (size_t('0')+ lo) : (size_t('a') + (lo - size_t(10))));
    }

    return resSize;
}

std::u8string   GpStringOps::SFromBytesHex (GpSpanPtrByteR aData)
{
    const size_t charsCount = NumOps::SMul(aData.Size().As<size_t>(), size_t(2));

    std::u8string res;
    res.resize(charsCount);

    if (SFromBytesHex(aData, res) != charsCount)
    {
        THROW_GP(u8"Failed to convert bytes to hex string"_sv);
    }

    return res;
}

size_byte_t GpStringOps::SToBytesHex
(
    std::u8string_view  aStr,
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
        u8"String length must be even"_sv
    );

    //Remove prefix
    std::u8string_view prefix   = aStr.substr(0, 2);
    std::u8string_view strHex = aStr;
    if ((prefix == u8"0x"_sv) ||
        (prefix == u8"0X"_sv))
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
        u8"Out data size is too small"_sv
    );

    size_t              countLeft   = strHex.size();
    const char8_t* _R_  str         = strHex.data();

    while (countLeft > 0)
    {
        countLeft -= 2;
        std::array<char8_t, 2> s = {*str++, *str++};
        *aDataOut++ = u_int_8(SToByteHex(s));
    }

    return size_byte_t::SMake(outSize);
}

GpBytesArray    GpStringOps::SToBytesHex (std::u8string_view aStr)
{
    const size_t size = aStr.size();

    GpBytesArray res;
    res.resize(size/2);
    res.resize(SToBytesHex(aStr, GpSpanPtrByteRW(res.data(), res.size())).As<size_t>());
    return res;
}

std::u8string   GpStringOps::SFromBits (GpSpanPtrByteR aData)
{
    std::u8string res;

    const size_t    bytesCount  = aData.Count();
    const size_t    bitsCount   = (size_bit_t::SMake(aData.Count()) * 8_bit).As<size_t>();
    const u_int_8*  data        = aData.PtrAs<const u_int_8*>();

    THROW_COND_GP
    (
        data != nullptr,
        u8"aData is nullptr"_sv
    );

    res.resize(bitsCount + bytesCount + 1);
    char8_t* str = res.data();

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

std::u8string   GpStringOps::PercentEncode (std::u8string_view aSrc)
{
    const char8_t*      srcPtr          = aSrc.data();
    std::u8string       encodedStr;
    encodedStr.resize(NumOps::SMul<size_t>(aSrc.size(), 3));
    char8_t*            encodedStrPtr   = encodedStr.data();
    size_t              usedSize        = 0;
    size_t              unprocessedSize = aSrc.size();

    while (unprocessedSize > 0)
    {
        unprocessedSize--;
        const char8_t ch = *srcPtr++;

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

        const auto hexStr = StrOps::SFromByteHex(u_int_8(ch));
        std::array<char8_t, 3> buff;
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

std::u8string   GpStringOps::SToString (const std::thread::id aThreadId)
{
    //TODO: reimplement

    std::ostringstream ss;
    ss << aThreadId;

    const auto s = ss.str();
    return {reinterpret_cast<const char8_t*>(s.data()), s.size()};
}

std::u8string   GpStringOps::SToString (const void* aPtr)
{
    //TODO: reimplement

    std::ostringstream ss;
    ss << aPtr;

    const auto s = ss.str();
    return {reinterpret_cast<const char8_t*>(s.data()), s.size()};
}

void    GpStringOps::_SFromUI64
(
    const u_int_64      aValue,
    GpSpanPtrCharU8RW   aStrOut
)
{
    u_int_64        value   = aValue;
    const char* _R_ digits  = SDigits().data();
    char8_t* _R_    strPtr  = aStrOut.Ptr();

    if (value < 10)
    {
        *strPtr = char8_t(size_t('0') + size_t(value));
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
        *strPtr = char8_t(size_t('0') + size_t(value));
    } else
    {
        const size_t i = size_t(value) * 2;
        MemOps::SCopy(strPtr, digits + i, 2);
    }
}

const std::array<char, 201>&    GpStringOps::SDigits (void) noexcept
{
    static const std::array<char, 201> sDigits =
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
