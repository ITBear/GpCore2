#include "GpStringOps.hpp"

#if defined(GP_USE_STRINGS)

#include <charconv>

namespace GPlatform {

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
        GpSpanCharR(aSourceStr),
        GpSpanCharR(&aDelim, 1),
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
        GpSpanCharR(aSourceStr),
        GpSpanCharR(aDelim),
        aReturnPartsCountLimit,
        aDelimCountLimit,
        aSplitMode
    );
}

bool    GpStringOps::SIsEqualCaseInsensitive8bit
(
    std::string_view    aStr1,
    std::string_view    aStr2
) noexcept
{
    return std::equal
    (
        std::begin(aStr1),
        std::end(aStr1),
        std::begin(aStr2),
        std::end(aStr2),
        [](const char a, const char b)
        {
            return std::tolower(int(a)) == std::tolower(int(b));
        }
    );
}

size_t  GpStringOps::SFromUI64
(
    const u_int_64  aValue,
    GpSpanCharRW    aStrOut
)
{
    const size_t length = GpNumericOps::SDecDigsCountUI64(aValue);

    THROW_COND_GP
    (
        aStrOut.Count() >= length,
        "aStrOut size are too small"_sv
    );

    _SFromUI64(aValue, GpSpanCharRW(aStrOut.Ptr(), length));

    return length;
}

std::string GpStringOps::SFromUI64 (const u_int_64 aValue)
{
    const size_t    length = GpNumericOps::SDecDigsCountUI64(aValue);
    std::string     s;
    s.resize(length);

    _SFromUI64(aValue, s);

    return s;
}

size_t  GpStringOps::SFromSI64
(
    const s_int_64  aValue,
    GpSpanCharRW    aStrOut
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

        _SFromUI64(v, GpSpanCharRW(aStrOut.Ptr(), length));
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
        _SFromUI64(v, GpSpanCharRW(aStrOut.Ptr(), length));
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

        _SFromUI64(v, GpSpanCharRW(s));
    } else
    {
        const u_int_64 v = std::bit_cast<u_int_64>(-aValue);
        length = GpNumericOps::SDecDigsCountUI64(v);
        s.resize(length + 1);

        *std::data(s) = '-';
        _SFromUI64(v, GpSpanCharRW(std::data(s) + 1, length));
    }

    return s;
}

size_t  GpStringOps::SFromDouble
(
    const double    aValue,
    GpSpanCharRW    aStrOut
)
{
    //TODO reimplement
    const std::string   tmpS    = std::to_string(aValue);
    std::string         s       = std::string(reinterpret_cast<const char*>(std::data(tmpS)), std::size(tmpS));

    const size_t length = s.length();

    THROW_COND_GP
    (
        aStrOut.Count() >= length,
        "aMaxOutLength value are too small"_sv
    );

    std::replace(std::begin(s), std::end(s), ',', '.');

    MemOps::SCopy(aStrOut.Ptr(), std::data(s), length);

    return length;
}

std::string GpStringOps::SFromDouble (const double aValue)
{
    //TODO reimplement
    const std::string   tmpS    = std::to_string(aValue);
    std::string         s       = std::string(reinterpret_cast<const char*>(std::data(tmpS)), std::size(tmpS));

    std::replace(std::begin(s), std::end(s), ',', '.');
    return s;
}

u_int_64    GpStringOps::SToUI64
(
    std::string_view                                aStr,
    std::optional<std::reference_wrapper<size_t>>   aReadCountOut
)
{
    u_int_64    result      = 0;
    const char* strPtrBegin = reinterpret_cast<const char*>(std::data(aStr));

    const std::from_chars_result r = std::from_chars
    (
        strPtrBegin,
        strPtrBegin + std::size(aStr),
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
        THROW_GP("Wrong source string value '"_sv + aStr + "'"_sv);
    } else if (r.ec == std::errc::result_out_of_range)
    {
        THROW_GP("Out of u_int_64 range: '"_sv + aStr + "'"_sv);
    } else
    {
        THROW_GP("std::errc() = "_sv + int(std::errc()) + ". Source string '"_sv + aStr + "'"_sv);
    }

    return result;
}

s_int_64    GpStringOps::SToSI64
(
    std::string_view                                aStr,
    std::optional<std::reference_wrapper<size_t>>   aReadCountOut
)
{
    s_int_64    result      = 0;
    const char* strPtrBegin = reinterpret_cast<const char*>(std::data(aStr));
    size_t      strSize     = std::size(aStr);

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
        THROW_GP("Wrong source string value '"_sv + aStr + "'"_sv);
    } else if (r.ec == std::errc::result_out_of_range)
    {
        THROW_GP("Out of s_int_64 range: '"_sv + aStr + "'"_sv);
    } else
    {
        THROW_GP("std::errc() = "_sv + int(std::errc()) + ". Source string '"_sv + aStr + "'"_sv);
    }

    return result;
}

double      GpStringOps::SToDouble
(
    std::string_view                                aStr,
    std::optional<std::reference_wrapper<size_t>>   aReadCountOut
)
{
    double result = 0.0;

#if __cplusplus < CPP_VERSION_23
    const std::string s(reinterpret_cast<const char*>(std::data(aStr)), std::size(aStr));
    char* end{};
    result = std::strtod(std::data(s), &end);

    const size_t readCount = size_t(end - std::data(s));

    if (readCount != std::size(s))
    {
        THROW_GP("Wrong source string value '"_sv + aStr + "'"_sv);
    }

    if (aReadCountOut.has_value())
    {
        aReadCountOut.value() += readCount;
    }
#else
    const char* strPtrBegin = std::data(aStr);

    const std::from_chars_result r = std::from_chars
    (
        strPtrBegin,
        strPtrBegin + std::size(aStr),
        result,
        std::chars_format::fixed
    );

    if (r.ec == std::errc())
    {
        if (aReadCountOut.has_value())
        {
            const size_t readCount = size_t{r.ptr - strPtrBegin};
            aReadCountOut.value() += readCount;
        }

        return result;
    } else if (r.ec == std::errc::invalid_argument)
    {
        THROW_GP("Wrong source string value '"_sv + aStr + "'"_sv);
    } else if (r.ec == std::errc::result_out_of_range)
    {
        THROW_GP("Out of double range: '"_sv + aStr + "'"_sv);
    } else
    {
        THROW_GP("std::errc() = "_sv + int(std::errc()) + ". Source string '"_sv + aStr + "'"_sv);
    }
#endif//

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
    GpSpanByteR     aData,
    GpSpanCharRW    aStrOut
)
{
    const size_t dataLength     = aData.Count();
    const size_t strOutLength   = aStrOut.Count();

    if (dataLength == 0)
    {
        return 0;
    }

    const size_t resSize = NumOps::SMul(dataLength, size_t{2});

    THROW_COND_GP
    (
        strOutLength >= resSize,
        "Out string size is too small"_sv
    );

    size_t              countLeft   = dataLength;
    const u_int_8* _R_  dataPtr     = aData.PtrAs<const u_int_8*>();
    char* _R_           strPtr      = aStrOut.Ptr();

    THROW_COND_GP
    (
        strPtr != nullptr,
        "aStrOut is null"_sv
    );

    while (countLeft-- > 0)
    {
        const size_t b  = size_t{*dataPtr++};
        const size_t lo = (b & size_t{0x0F}) >> 0;
        const size_t hi = (b & size_t{0xF0}) >> 4;

        *strPtr++ = char((hi < size_t{10}) ? (size_t{'0'} + hi) : (size_t{'a'} + (hi - size_t{10})));
        *strPtr++ = char((lo < size_t{10}) ? (size_t{'0'} + lo) : (size_t{'a'} + (lo - size_t{10})));
    }

    return resSize;
}

std::string GpStringOps::SFromBytesHex (GpSpanByteR aData)
{
    const size_t charsCount = NumOps::SMul(aData.SizeInBytes(), size_t{2});

    std::string res;
    res.resize(charsCount);

    if (SFromBytesHex(aData, res) != charsCount)
    {
        THROW_GP("Failed to convert bytes to hex string"_sv);
    }

    return res;
}

size_t  GpStringOps::SToBytesHex
(
    std::string_view    aStr,
    GpSpanByteRW        aDataOut
)
{
    //String length
    const size_t strLength = std::size(aStr);

    if (strLength == 0)
    {
        return 0;
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
        if (std::size(aStr) <= 2)
        {
            return 0;
        }

        strHex = aStr.substr(2);
    }

    //
    const size_t outSize = std::size(strHex) / 2;
    THROW_COND_GP
    (
        aDataOut.Count() >= outSize,
        "Out data size is too small"_sv
    );

    size_t          countLeft   = std::size(strHex);
    const char* _R_ str         = std::data(strHex);

    while (countLeft > 0)
    {
        countLeft -= 2;
        std::array<char, 2> s = {*str++, *str++};
        *aDataOut++ = std::byte(SToByteHex(s));
    }

    return outSize;
}

GpBytesArray    GpStringOps::SToBytesHex (std::string_view aStr)
{
    const size_t size = std::size(aStr);

    GpBytesArray res;
    res.resize(size/2);
    res.resize(SToBytesHex(aStr, GpSpanByteRW(std::data(res), std::size(res))));
    return res;
}

std::string GpStringOps::SPercentEncode (std::string_view aSrc)
{
    std::string encodedStr;
    encodedStr.resize(NumOps::SMul<size_t>(std::size(aSrc), 3));

    char*       encodedStrPtr   = std::data(encodedStr);
    size_t      usedSize        = 0;
    size_t      unprocessedSize = std::size(aSrc);
    const char* srcPtr          = std::data(aSrc);

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

        const auto hexStr = StrOps::SFromByteHex(u_int_8(ch));
        std::array<char, 3> buff;
        buff.at(0) = '%';
        buff.at(1) = hexStr.at(0);
        buff.at(2) = hexStr.at(1);

        std::memcpy(encodedStrPtr, std::data(buff), std::size(buff));
        encodedStrPtr   += std::size(buff);
        usedSize        += std::size(buff);
    }

    encodedStr.resize(usedSize);

    return encodedStr;
}

void    GpStringOps::_SFromUI64
(
    const u_int_64  aValue,
    GpSpanCharRW    aStrOut
)
{
    u_int_64        value   = aValue;
    const char* _R_ digits  = std::data(SDigits());
    char* _R_       strPtr  = aStrOut.Ptr();

    if (value < 10)
    {
        *strPtr = char(size_t{'0'} + size_t{value});
        return;
    }

    {
        const size_t    countLeft   = aStrOut.Count();
        const size_t    offset      = NumOps::SSub(countLeft, size_t{2});
        strPtr += offset;
    }

    while (value >= 100)
    {
        const size_t i = size_t{(value % u_int_64(100)) * 2};
        value /= 100;

        MemOps::SCopy(strPtr, digits + i, 2);
        strPtr -= 2;
    }

    // Handle last 1-2 digits
    if (value < 10)
    {
        strPtr++;
        *strPtr = char(size_t{'0'} + size_t{value});
    } else
    {
        const size_t i = size_t{value} * 2;
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

}// namespace GPlatform

#endif// #if defined(GP_USE_STRINGS)
