#pragma once

#include "../../GpMacro.hpp"

#if defined(GP_USE_STRINGS)

#include "GpStringLiterals.hpp"
#include "../Containers/GpBytesArray.hpp"
#include "../Units/GpUnitUtils.hpp"
#include "../../Algorithms/GpSplit.hpp"
#include "../Enums/GpEnum.hpp"
#include "../UIDs/GpUUID.hpp"

#include <variant>
#include <thread>

namespace GPlatform {

class GP_UTILS_API GpStringOps
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpStringOps)

public:
    static std::vector<std::string_view>    SSplit          (std::string_view       aSourceStr,
                                                             const char             aDelim,
                                                             const size_t           aReturnPartsCountLimit,
                                                             const size_t           aDelimCountLimit,
                                                             const Algo::SplitMode  aSplitMode);

    static std::vector<std::string_view>    SSplit          (std::string_view       aSourceStr,
                                                             std::string_view       aDelim,
                                                             const size_t           aReturnPartsCountLimit,
                                                             const size_t           aDelimCountLimit,
                                                             const Algo::SplitMode  aSplitMode);

    static inline std::string_view          SFromChar       (const char* aStrPtr);

    static inline std::string               STrimLeft       (std::string_view   aStr,
                                                             const char         aChar);
    static inline std::string               STrimRight      (std::string_view   aStr,
                                                             const char         aChar);

    //------------------------- Numeric from/to string --------------------------
    static size_t                           SFromUI64       (const u_int_64     aValue,
                                                             GpSpanPtrCharRW    aStrOut);
    static std::string                      SFromUI64       (const u_int_64     aValue);

    static size_t                           SFromSI64       (const s_int_64     aValue,
                                                             GpSpanPtrCharRW    aStrOut);
    static std::string                      SFromSI64       (const s_int_64     aValue);

    static size_t                           SFromDouble     (const double       aValue,
                                                             GpSpanPtrCharRW    aStrOut);
    static std::string                      SFromDouble     (const double aValue);

    static u_int_64                         SToUI64         (std::string_view aStr, std::optional<std::reference_wrapper<size_t>> aReadCountOut = std::nullopt);
    static s_int_64                         SToSI64         (std::string_view aStr, std::optional<std::reference_wrapper<size_t>> aReadCountOut = std::nullopt);

    //Supported format: [+-][UInt64][.[UInt64]]
    static double                           SToDouble       (std::string_view aStr, std::optional<std::reference_wrapper<size_t>> aReadCountOut = std::nullopt);

    //[+-][UInt64][.[UInt64]] - DOUBLE
    //[+-]digits - INT
    static std::variant<s_int_64, double>   SToNumeric      (std::string_view aStr, std::optional<std::reference_wrapper<size_t>> aReadCountOut = std::nullopt);

    //------------------------- Bytes from/to string --------------------------
    static size_t                           SFromBytesHex   (GpSpanPtrByteR     aData,
                                                             GpSpanPtrCharRW    aStrOut);
    static std::string                      SFromBytesHex   (GpSpanPtrByteR     aData);
    static inline std::array<char,2>        SFromByteHex    (const std::byte    aData) noexcept;

    static size_byte_t                      SToBytesHex     (std::string_view   aStr,
                                                             GpSpanPtrByteRW    aDataOut);
    static GpBytesArray                     SToBytesHex     (std::string_view   aStr);
    static constexpr std::byte              SToByteHex      (std::array<char,2> aStr);

    static std::string                      SFromBits       (GpSpanPtrByteR     aData);

    //------------------------- Unicode --------------------------
    static size_t                           SConv_UTF16_UTF8(std::array<std::byte, 4>&      aUTF8_valueOut,
                                                             const std::array<std::byte, 2> aUTF16_value);//return bytes count of aUTF8Out
    static size_t                           SConv_UTF32_UTF8(std::array<std::byte, 4>&      aUTF8_valueOut,
                                                             const std::array<std::byte, 4> aUTF32_value);//return bytes count of aUTF8Out
    static size_t                           SLength_UTF8    (std::string_view aStr);

    //------------------------- Encode --------------------------
    static std::string                      PercentEncode   (std::string_view aSrc);

    //------------------------- Register --------------------------
    //static void                           SToLower        (GpSpanPtrCharRW    aStr);
    //static void                           SToUpper        (GpSpanPtrCharRW    aStr);

    //------------------------- Count -----------------------------
    static inline constexpr size_t          SCountChars     (std::string_view   aStr,
                                                             const char         aChar) noexcept;
    static size_t                           SCountChars     (std::string_view           aStr,
                                                             const std::vector<char>&   aChars) noexcept;
    static size_t                           SCountCharsRange(std::string_view           aStr,
                                                             const char                 aCharFrom,
                                                             const char                 aCharTo) noexcept;
    static bool                             SContainsOnly   (std::string_view       aStr,
                                                             const std::set<char>&  aSet) noexcept;

    //------------------------- Auto to string -----------------------------
    static std::string_view                 SToString       (const std::string& aValue) {return aValue;}
    static std::string_view                 SToString       (std::string_view aValue) {return aValue;}
    static std::string                      SToString       (const GpBytesArray& aValue) {return SFromBytesHex(aValue);}
    static std::string_view                 SToString       (const GpEnum& aValue) {return aValue.ToString();}
    static std::string                      SToString       (const GpUUID& aValue) {return aValue.ToString();}
    static std::string                      SToString       (float aValue) {return SFromDouble(double(aValue));}
    static std::string                      SToString       (double aValue) {return SFromDouble(aValue);}
    static std::string                      SToString       (const std::thread::id aThreadId);
    static std::string                      SToString       (const void* aPtr);

    template<typename T, typename _D = void, typename = std::enable_if_t<std::is_integral_v<T>, _D>>
    static std::string                      SToString       (T aValue)
    {
        if constexpr (std::is_signed_v<T>)
        {
            return SFromSI64(NumOps::SConvert<s_int_64>(aValue));
        } else
        {
            return SFromUI64(NumOps::SConvert<u_int_64>(aValue));
        }
    }

    template<typename T, typename = std::enable_if_t<GpUnitUtils::SHasTag_GpUnit<T>()>>
    static std::string                      SToString       (T aValue)
    {
        return SToString<typename T::value_type>(aValue.Value());
    }

    template<typename E,
             typename T>
    static std::string                      SJoin           (const T&           aArray,
                                                             std::string_view   aSeparator);

    template<typename E,
             typename T>
    static std::string                      SJoin           (const T&                                       aArray,
                                                             std::function<E(typename T::const_iterator&)>  aGetterFn,
                                                             std::string_view                               aSeparator);

private:
    static void                             _SFromUI64      (const u_int_64     aValue,
                                                             GpSpanPtrCharRW    aStrOut);

private:
    static const std::array<char, 201>&     SDigits         (void) noexcept;
};

std::string_view    GpStringOps::SFromChar (const char* aStrPtr)
{
    if (aStrPtr != nullptr)
    {
        return std::string_view(aStrPtr);
    } else
    {
        return std::string_view();
    }
}

std::string GpStringOps::STrimLeft
(
    std::string_view    aStr,
    const char          aChar
)
{
    const size_t id = aStr.find_first_not_of(aChar);

    if (id != std::string::npos)
    {
        return std::string(aStr.substr(id));
    }

    return std::string();
}

std::string GpStringOps::STrimRight
(
    std::string_view    aStr,
    const char          aChar
)
{
    size_t id = aStr.find_last_not_of(aChar);

    if (id != std::string::npos)
    {
        return  std::string(aStr.substr(0, id + 1));
    }

    return std::string();
}

std::array<char,2>  GpStringOps::SFromByteHex (const std::byte aData) noexcept
{
    constexpr std::array<char, 16>  hexToChar =
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };

    return {hexToChar.data()[(size_t(aData) & size_t(0xF0)) >> 4],
            hexToChar.data()[(size_t(aData) & size_t(0x0F)) >> 0]};
}

constexpr std::byte GpStringOps::SToByteHex (std::array<char,2> aStr)
{
    //--------------------------
    char    ch      = aStr.data()[0];
    size_t  valHi   = 0;
    size_t  valLo   = 0;
    size_t  beginCh = 0;
    size_t  shift   = 0;

    if ((ch >= '0') && (ch <= '9'))     {beginCh = size_t('0'); shift = 0;}
    else if ((ch >= 'A') && (ch <= 'Z')){beginCh = size_t('A'); shift = 10;}
    else if ((ch >= 'a') && (ch <= 'z')){beginCh = size_t('a'); shift = 10;}
    else GpThrowCe<std::out_of_range>("Wrong HEX character");

    valHi = u_int_8(size_t(ch) - beginCh + shift);

    //--------------------------
    ch = aStr.data()[1];

    if ((ch >= '0') && (ch <= '9'))     {beginCh = size_t('0'); shift = 0;}
    else if ((ch >= 'A') && (ch <= 'Z')){beginCh = size_t('A'); shift = 10;}
    else if ((ch >= 'a') && (ch <= 'z')){beginCh = size_t('a'); shift = 10;}
    else GpThrowCe<std::out_of_range>("Wrong HEX character");

    valLo = u_int_8(size_t(ch) - beginCh + shift);

    return std::byte(u_int_8(valHi << 4) | u_int_8(valLo));
}

constexpr size_t    GpStringOps::SCountChars
(
    std::string_view    aStr,
    const char          aChar
) noexcept
{
    size_t          count       = 0;
    const char* _R_ data        = aStr.data();
    const size_t    countLeft   = aStr.size();

    for (size_t id = 0; id < countLeft; ++id)
    {
        if (*data++ == aChar)
        {
            count++;
        }
    }

    return count;
}

template<typename E,
         typename T>
std::string GpStringOps::SJoin
(
    const T&            aArray,
    std::string_view    aSeparator
)
{
    return SJoin<E, T>
    (
        aArray,
        [](typename T::const_iterator& aIter)->E {return *aIter;},
        aSeparator
    );
}

template<typename E,
         typename T>
std::string GpStringOps::SJoin
(
    const T&                                        aArray,
    std::function<E(typename T::const_iterator&)>   aGetterFn,
    std::string_view                                aSeparator
)
{
    const size_t elementsCount = aArray.size();
    std::string res;
    res.reserve(elementsCount * 16);

    bool isFirst = true;
    for (typename T::const_iterator iter = aArray.begin(); iter != aArray.end(); ++iter)
    {
        if (!isFirst)
        {
            res.append(aSeparator);
        } else
        {
            isFirst = false;
        }

        res.append(aGetterFn(iter));
    }

    return res;
}

using StrOps = GpStringOps;

}//GPlatform

//*********************************************************

namespace std {

inline string to_string(string_view aStrView)
{
    return string(aStrView);
}

inline std::string operator+
(
    const std::string&  aLeft,
    std::string_view    aRight
)
{
    std::string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::string operator+
(
    std::string_view aLeft,
    std::string_view aRight
)
{
    std::string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::string operator+
(
    std::string_view    aLeft,
    const std::string&  aRight
)
{
    std::string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::string operator+
(
    std::string_view    aLeft,
    const char*         aRight
)
{
    std::string_view    right(aRight);
    std::string         res;
    res.reserve(aLeft.length() + right.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(right.data(), right.length());

    return res;
}

inline std::string operator+
(
    std::string_view    aLeft,
    const char          aRight
)
{
    std::string res;
    res.reserve(aLeft.length() + 1);
    res.append(aLeft.data(), aLeft.length());
    res += aRight;

    return res;
}

template<typename T>
inline std::string operator+
(
    std::string_view    aLeft,
    T                   aRight
)
{
    const std::string value(::GPlatform::GpStringOps::SToString(aRight));

    std::string res;
    res.reserve(aLeft.length() + value.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(value.data(), value.length());

    return res;
}

}//std

#endif//#if defined(GP_USE_STRINGS)
