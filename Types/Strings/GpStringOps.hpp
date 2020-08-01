#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_STRINGS)

#include "GpStringLiterals.hpp"
#include "GpStringOpsGlob.hpp"
#include "../Classes/GpClassesDefines.hpp"
#include "../Containers/GpContainersT.hpp"
#include "../Containers/GpBytesArray.hpp"
#include "../Containers/GpRawPtrByte.hpp"
#include "../Units/Other/count_t.hpp"
#include "../Units/Other/size_byte_t.hpp"
#include "../../Algorithms/GpSplit.hpp"

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
    static count_t                          SFromUI64       (const UInt64       aValue,
                                                             GpRawPtrCharRW     aStrOut);
    static std::string                      SFromUI64       (const UInt64       aValue);

    static count_t                          SFromSI64       (const SInt64       aValue,
                                                             GpRawPtrCharRW     aStrOut);
    static std::string                      SFromSI64       (const SInt64       aValue);

    static count_t                          SFromDouble     (const double       aValue,
                                                             GpRawPtrCharRW     aStrOut);
    static std::string                      SFromDouble     (const double aValue);

    static UInt64                           SToUI64         (GpRawPtrCharR aStr);
    static SInt64                           SToSI64         (GpRawPtrCharR aStr);
    static double                           SToDouble_fast  (GpRawPtrCharR aStr);//Supported format: [+-][UInt64][.[UInt64]]

    //[+-][UInt64][.[UInt64]] - DOUBLE
    //[+-]digits - INT
    static std::variant<SInt64, double>     SToNumeric      (GpRawPtrCharR aStr);

    //------------------------- Bytes from/to string --------------------------
    static count_t                          SFromBytes      (GpRawPtrByteR  aData,
                                                             GpRawPtrCharRW aStrOut);
    static std::string                      SFromBytes      (GpRawPtrByteR  aData);
    static inline GpArray<char,2>           SFromByte       (const std::byte    aData) noexcept;

    static size_byte_t                      SToBytes        (GpRawPtrCharR  aStr,
                                                             GpRawPtrByteRW aDataOut);
    static GpBytesArray                     SToBytes        (GpRawPtrCharR      aStr);
    static constexpr std::byte              SToByte         (GpArray<char,2>    aStr);

    //------------------------- Unicode --------------------------
    static count_t                          SConv_UTF16_UTF8(GpArray<std::byte, 4>&         aUTF8_valueOut,
                                                             const GpArray<std::byte, 2>    aUTF16_value);//return bytes count of aUTF8Out
    static count_t                          SConv_UTF32_UTF8(GpArray<std::byte, 4>&         aUTF8_valueOut,
                                                             const GpArray<std::byte, 4>    aUTF32_value);//return bytes count of aUTF8Out

    //------------------------- Replace --------------------------
    static std::string&                     SReplace        (std::string& aStr, const char aChar, const char aNewChar);

    //------------------------- Count -----------------------------
    static inline constexpr count_t         SCountChars     (std::string_view aStr, const char aChar) noexcept;

private:
    static void                             _SFromUI64      (const UInt64   aValue,
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

    return {hexToChar.data()[(size_t(aData) & size_t(0x0F)) >> 0],
            hexToChar.data()[(size_t(aData) & size_t(0xF0)) >> 4]};
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

}//GPlatform

#endif//#if defined(GP_USE_STRINGS)
