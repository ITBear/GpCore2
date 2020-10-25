#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_STRINGS)

#include "GpStringLiterals.hpp"
#include "../Classes/GpClassesDefines.hpp"
#include "../Containers/GpBytesArray.hpp"
#include "../Containers/GpRawPtrByte.hpp"
#include "../Units/Other/count_t.hpp"
#include "../Units/Other/size_byte_t.hpp"
#include "../../Algorithms/GpSplit.hpp"
#include "../Enums/GpEnum.hpp"
#include "../UIDs/GpUUID.hpp"

#include <variant>

namespace GPlatform {

class GPCORE_API GpStringOps
{
    CLASS_REMOVE_CTRS(GpStringOps)

public:
    static GpVector<GpRawPtrCharR>          SSplit          (GpRawPtrCharR          aSourceStr,
                                                             const char             aDelim,
                                                             const count_t          aReturnPartsCountLimit,
                                                             const count_t          aDelimCountLimit,
                                                             const Algo::SplitMode  aSplitMode);

    static GpVector<GpRawPtrCharR>          SSplit          (GpRawPtrCharR          aSourceStr,
                                                             GpRawPtrCharR          aDelim,
                                                             const count_t          aReturnPartsCountLimit,
                                                             const count_t          aDelimCountLimit,
                                                             const Algo::SplitMode  aSplitMode);

    //------------------------- Numeric from/to string --------------------------
    static count_t                          SFromUI64       (const u_int_64     aValue,
                                                             GpRawPtrCharRW     aStrOut);
    static std::string                      SFromUI64       (const u_int_64     aValue);

    static count_t                          SFromSI64       (const s_int_64     aValue,
                                                             GpRawPtrCharRW     aStrOut);
    static std::string                      SFromSI64       (const s_int_64     aValue);

    static count_t                          SFromDouble     (const double       aValue,
                                                             GpRawPtrCharRW     aStrOut);
    static std::string                      SFromDouble     (const double aValue);

    static u_int_64                         SToUI64         (GpRawPtrCharR aStr);
    static s_int_64                         SToSI64         (GpRawPtrCharR aStr);
    static double                           SToDouble_fast  (GpRawPtrCharR aStr);//Supported format: [+-][UInt64][.[UInt64]]

    //[+-][UInt64][.[UInt64]] - DOUBLE
    //[+-]digits - INT
    static std::variant<s_int_64, double>   SToNumeric      (GpRawPtrCharR aStr);

    //------------------------- Bytes from/to string --------------------------
    static count_t                          SFromBytes      (GpRawPtrByteR  aData,
                                                             GpRawPtrCharRW aStrOut);
    static std::string                      SFromBytes      (GpRawPtrByteR  aData);
    static inline GpArray<char,2>           SFromByte       (const std::byte    aData) noexcept;

    static size_byte_t                      SToBytes        (GpRawPtrCharR  aStr,
                                                             GpRawPtrByteRW aDataOut);
    static GpBytesArray                     SToBytes        (GpRawPtrCharR      aStr);
    static constexpr std::byte              SToByte         (GpArray<char,2>    aStr);

    static std::string                      SFromBits       (GpRawPtrByteR  aData);

    //------------------------- Unicode --------------------------
    static count_t                          SConv_UTF16_UTF8(GpArray<std::byte, 4>&         aUTF8_valueOut,
                                                             const GpArray<std::byte, 2>    aUTF16_value);//return bytes count of aUTF8Out
    static count_t                          SConv_UTF32_UTF8(GpArray<std::byte, 4>&         aUTF8_valueOut,
                                                             const GpArray<std::byte, 4>    aUTF32_value);//return bytes count of aUTF8Out

    //------------------------- Replace --------------------------
    static std::string&                     SReplace        (std::string& aStr, const char aChar, const char aNewChar);

    //------------------------- Count -----------------------------
    static inline constexpr count_t         SCountChars     (std::string_view aStr, const char aChar) noexcept;

    //------------------------- Auto to string -----------------------------
    static std::string_view                 SToString       (const std::string& aValue) {return aValue;}
    static std::string_view                 SToString       (std::string_view aValue) {return aValue;}
    static std::string                      SToString       (const GpBytesArray& aValue) {return SFromBytes(aValue);}
    //static std::string_view               SToString       (GpRawPtrCharR aValue) {return aValue.AsStringView();}
    //static std::string_view               SToString       (GpRawPtrCharRW aValue) {return aValue.AsStringView();}
    //static std::string                    SToString       (GpRawPtrByteR aValue) {return SFromBytes(aValue);}
    //static std::string                    SToString       (GpRawPtrByteRW aValue) {return SFromBytes(aValue);}
    static std::string_view                 SToString       (const GpEnum& aValue) {return aValue.ToString();}
    static std::string                      SToString       (const GpUUID& aValue) {return aValue.ToString();}
    static std::string                      SToString       (float aValue) {return SFromDouble(double(aValue));}
    static std::string                      SToString       (double aValue) {return SFromDouble(aValue);}

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
    static void                             _SFromUI64      (const u_int_64 aValue,
                                                             GpRawPtrCharRW aStrOut);

private:
    static const GpArray<char, 201>&        SDigits         (void) noexcept;
};


GpArray<char,2> GpStringOps::SFromByte (const std::byte aData) noexcept
{
    constexpr GpArray<char, 16> hexToChar =
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };

    return {hexToChar.data()[(size_t(aData) & size_t(0xF0)) >> 4],
            hexToChar.data()[(size_t(aData) & size_t(0x0F)) >> 0]};
}

constexpr std::byte GpStringOps::SToByte (GpArray<char,2> aStr)
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

constexpr count_t   GpStringOps::SCountChars (std::string_view aStr, const char aChar) noexcept
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

    return count_t::SMake(count);
}

template<typename E,
         typename T>
std::string GpStringOps::SJoin (const T& aArray, std::string_view aSeparator)
{
    return SJoin<E, T>(aArray,
                      [](typename T::const_iterator& aIter)->E {return *aIter;},
                      aSeparator);
}

template<typename E,
         typename T>
std::string GpStringOps::SJoin (const T&                                        aArray,
                                std::function<E(typename T::const_iterator&)>   aGetterFn,
                                std::string_view                                aSeparator)
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

}//GPlatform

//*********************************************************

namespace std {

inline string to_string(string_view aStrView)
{
    return string(aStrView);
}

/*inline string to_string(GPlatform::GpRawPtrByteR aStrPtr)
{
    return string(aStrPtr.AsStringView());
}*/

inline std::string operator+ (const std::string& aLeft, std::string_view aRight)
{
    std::string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::string operator+ (std::string_view aLeft, std::string_view aRight)
{
    std::string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::string operator+ (std::string_view aLeft, const std::string& aRight)
{
    std::string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::string operator+ (std::string_view aLeft, const char* aRight)
{
    std::string_view    right(aRight);
    std::string         res;
    res.reserve(aLeft.length() + right.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(right.data(), right.length());

    return res;
}

template<typename T>
inline std::string operator+ (std::string_view aLeft, T aRight)
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
