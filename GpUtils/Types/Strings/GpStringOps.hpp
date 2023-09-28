#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_STRINGS)

#include <vector>
#include <string>
#include <string_view>
#include <variant>
#include <thread>
#include <set>

#include "../../GpUtils_global.hpp"
#include "../../Macro/GpMacroClass.hpp"
#include "../../Algorithms/GpSplit.hpp"
#include "../Containers/GpBytesArray.hpp"
#include "../Enums/GpEnum.hpp"
#include "../UIDs/GpUUID.hpp"
#include "../Numerics/GpNumericOps.hpp"
#include "GpUTF.hpp"

namespace GPlatform {

class GP_UTILS_API GpStringOps
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpStringOps)

public:
    static std::vector<std::u8string_view>  SSplit          (std::u8string_view     aSourceStr,
                                                             const char8_t          aDelim,
                                                             const size_t           aReturnPartsCountLimit,
                                                             const size_t           aDelimCountLimit,
                                                             const Algo::SplitMode  aSplitMode);

    static std::vector<std::u8string_view>  SSplit          (std::u8string_view     aSourceStr,
                                                             std::u8string_view     aDelim,
                                                             const size_t           aReturnPartsCountLimit,
                                                             const size_t           aDelimCountLimit,
                                                             const Algo::SplitMode  aSplitMode);

    static inline std::u8string_view        SFromChar       (const char8_t* aStrPtr);

    static inline std::u8string             STrimLeft       (std::u8string_view aStr,
                                                             const char8_t      aChar);
    static inline std::u8string             STrimRight      (std::u8string_view aStr,
                                                             const char8_t      aChar);

    //-------------------------------- Compare ----------------------------------
    static bool                             SIsEqualCaseInsensitive8bit
                                                            (std::u8string_view aStr1,
                                                             std::u8string_view aStr2) noexcept;

    //------------------------- Numeric from/to string --------------------------
    static size_t                           SFromUI64       (const u_int_64     aValue,
                                                             GpSpanPtrCharRW    aStrOut);
    static std::u8string                    SFromUI64       (const u_int_64     aValue);

    static size_t                           SFromSI64       (const s_int_64     aValue,
                                                             GpSpanPtrCharRW    aStrOut);
    static std::u8string                    SFromSI64       (const s_int_64     aValue);

    static size_t                           SFromDouble     (const double       aValue,
                                                             GpSpanPtrCharRW    aStrOut);
    static std::u8string                    SFromDouble     (const double aValue);

    static u_int_64                         SToUI64         (std::u8string_view                             aStr,
                                                             std::optional<std::reference_wrapper<size_t>>  aReadCountOut = std::nullopt);
    static s_int_64                         SToSI64         (std::u8string_view                             aStr,
                                                             std::optional<std::reference_wrapper<size_t>>  aReadCountOut = std::nullopt);

    //Supported format: [+-][UInt64][.[UInt64]]
    static double                           SToDouble       (std::u8string_view                             aStr,
                                                             std::optional<std::reference_wrapper<size_t>>  aReadCountOut = std::nullopt);

    //[+-][UInt64][.[UInt64]] - DOUBLE
    //[+-]digits - INT
    static std::variant<s_int_64, double>   SToNumeric      (std::u8string_view                             aStr,
                                                             std::optional<std::reference_wrapper<size_t>>  aReadCountOut = std::nullopt);

    //------------------------- Bytes from/to string --------------------------
    static size_t                           SFromBytesHex   (GpSpanPtrByteR     aData,
                                                             GpSpanPtrCharRW    aStrOut);
    static std::u8string                    SFromBytesHex   (GpSpanPtrByteR     aData);
    static inline std::array<char8_t, 2>    SFromByteHex    (const u_int_8  aData) noexcept;

    static size_byte_t                      SToBytesHex     (std::u8string_view aStr,
                                                             GpSpanPtrByteRW    aDataOut);
    static GpBytesArray                     SToBytesHex     (std::u8string_view aStr);
    static constexpr u_int_8                SToByteHex      (std::array<char8_t, 2> aStr);

    static std::u8string                    SFromBits       (GpSpanPtrByteR     aData);

    //------------------------- Encode --------------------------
    static std::u8string                    PercentEncode   (std::u8string_view aSrc);

    //------------------------- Auto to string -----------------------------
    static std::u8string_view               SToString       (const std::u8string& aValue) {return aValue;}
    static std::u8string_view               SToString       (std::u8string_view aValue) {return aValue;}
    static std::string_view                 SToString       (const std::string& aValue) {return aValue;}
    static std::string_view                 SToString       (std::string_view aValue) {return aValue;}
    static std::u8string                    SToString       (const GpBytesArray& aValue) {return SFromBytesHex(GpSpanPtrByteR(aValue.data(), aValue.size()));}
    static std::u8string_view               SToString       (const GpEnum& aValue) {return aValue.ToString();}
    static std::u8string                    SToString       (const GpUUID& aValue) {return aValue.ToString();}
    static std::u8string                    SToString       (float aValue) {return SFromDouble(double(aValue));}
    static std::u8string                    SToString       (double aValue) {return SFromDouble(aValue);}
    static std::u8string                    SToString       (const std::thread::id aThreadId);
    static std::u8string                    SToString       (const void* aPtr);
    static std::u8string_view               SToString       (const bool aValue) noexcept {return aValue ? u8"true"_sv : u8"false"_sv;}

    template<Concepts::IsIntergal T>
    static std::u8string                    SToString       (T aValue)
    {
        if constexpr (std::is_signed_v<T>)
        {
            return SFromSI64(NumOps::SConvert<s_int_64>(aValue));
        } else
        {
            return SFromUI64(NumOps::SConvert<u_int_64>(aValue));
        }
    }

    template<Concepts::Unit::IsUnit T>
    static std::u8string                    SToString       (T aValue)
    {
        return SToString<typename T::value_type>(aValue.Value());
    }

    template<typename E,
             typename T>
    static std::u8string                    SJoin           (const T&           aArray,
                                                             std::u8string_view aSeparator);

    template<typename E,
             typename T>
    static std::u8string                    SJoin           (const T&                                       aArray,
                                                             std::function<E(typename T::const_iterator&)>  aGetterFn,
                                                             std::u8string_view                             aSeparator);

    static constexpr size_t                 SCharsCount     (std::string_view   aStr,
                                                             const char8_t      aChar) noexcept;

private:
    static void                             _SFromUI64      (const u_int_64     aValue,
                                                             GpSpanPtrCharRW    aStrOut);

private:
    static const std::array<char, 201>&     SDigits         (void) noexcept;
};

std::u8string_view  GpStringOps::SFromChar (const char8_t* aStrPtr)
{
    if (aStrPtr != nullptr)
    {
        return std::u8string_view(aStrPtr);
    } else
    {
        return std::u8string_view();
    }
}

std::u8string   GpStringOps::STrimLeft
(
    std::u8string_view  aStr,
    const char8_t       aChar
)
{
    const size_t id = aStr.find_first_not_of(aChar);

    if (id != std::u8string::npos)
    {
        return std::u8string(aStr.substr(id));
    }

    return std::u8string();
}

std::u8string   GpStringOps::STrimRight
(
    std::u8string_view  aStr,
    const char8_t       aChar
)
{
    size_t id = aStr.find_last_not_of(aChar);

    if (id != std::u8string::npos)
    {
        return  std::u8string(aStr.substr(0, id + 1));
    }

    return std::u8string();
}

std::array<char8_t, 2>  GpStringOps::SFromByteHex (const u_int_8 aData) noexcept
{
    constexpr std::array<char8_t, 16>   hexToChar =
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };

    return {hexToChar.data()[(size_t(aData) & size_t(0xF0)) >> 4],
            hexToChar.data()[(size_t(aData) & size_t(0x0F)) >> 0]};
}

constexpr u_int_8   GpStringOps::SToByteHex (std::array<char8_t, 2> aStr)
{
    //--------------------------
    char8_t ch      = aStr.data()[0];
    size_t  valHi   = 0;
    size_t  valLo   = 0;
    size_t  beginCh = 0;
    size_t  shift   = 0;

    if ((ch >= '0') && (ch <= '9'))     {beginCh = size_t('0'); shift = 0;}
    else if ((ch >= 'A') && (ch <= 'Z')){beginCh = size_t('A'); shift = 10;}
    else if ((ch >= 'a') && (ch <= 'z')){beginCh = size_t('a'); shift = 10;}
    else GpThrowCe<GpException>(u8"Wrong HEX character");

    valHi = u_int_8(size_t(ch) - beginCh + shift);

    //--------------------------
    ch = aStr.data()[1];

    if ((ch >= '0') && (ch <= '9'))     {beginCh = size_t('0'); shift = 0;}
    else if ((ch >= 'A') && (ch <= 'Z')){beginCh = size_t('A'); shift = 10;}
    else if ((ch >= 'a') && (ch <= 'z')){beginCh = size_t('a'); shift = 10;}
    else GpThrowCe<GpException>(u8"Wrong HEX character");

    valLo = u_int_8(size_t(ch) - beginCh + shift);

    return u_int_8(valHi << 4) | u_int_8(valLo);
}

template<typename E,
         typename T>
std::u8string   GpStringOps::SJoin
(
    const T&            aArray,
    std::u8string_view  aSeparator
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
std::u8string   GpStringOps::SJoin
(
    const T&                                        aArray,
    std::function<E(typename T::const_iterator&)>   aGetterFn,
    std::u8string_view                              aSeparator
)
{
    const size_t elementsCount = aArray.size();
    std::u8string res;
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

constexpr size_t    GpStringOps::SCharsCount
(
    std::string_view    aStr,
    const char8_t       aChar
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

using StrOps = GpStringOps;

}//GPlatform

//*********************************************************

namespace std {

inline string to_string(string_view aStrView)
{
    return string(aStrView);
}

//------------------ char ------------------------
//TODO: make all pairs and combinations

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
    const std::string&  aLeft,
    const char*         aRight
)
{
    return aLeft + std::string_view(aRight);
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
    std::string_view right(aRight);

    std::string res;
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

template<::GPlatform::Concepts::IsIntergal T>
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

//------------------ char8_t ---------------------
//TODO: make all pairs and combinations

inline std::u8string operator+
(
    const std::u8string&    aLeft,
    std::u8string_view      aRight
)
{
    std::u8string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::u8string operator+
(
    const std::u8string&    aLeft,
    std::string_view        aRight
)
{
    std::u8string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());

    std::u8string_view svRigth = ::GPlatform::GpUTF::S_STR_To_UTF8(aRight);
    res.append(svRigth.data(), svRigth.length());

    return res;
}

inline std::u8string operator+
(
    const std::u8string&    aLeft,
    const char8_t*          aRight
)
{
    return aLeft + std::u8string_view(aRight);
}

inline std::u8string operator+
(
    const std::u8string&    aLeft,
    const char*             aRight
)
{
    return aLeft + std::string_view(aRight);
}

inline std::u8string operator+
(
    std::u8string_view aLeft,
    std::u8string_view aRight
)
{
    std::u8string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::u8string operator+
(
    std::u8string_view      aLeft,
    const std::u8string&    aRight
)
{
    std::u8string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(aRight.data(), aRight.length());

    return res;
}

inline std::u8string operator+
(
    std::u8string_view  aLeft,
    std::string_view    aRight
)
{
    std::u8string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(GPlatform::GpUTF::S_STR_To_UTF8(aRight.data()), aRight.length());

    return res;
}

inline std::u8string operator+
(
    std::u8string_view  aLeft,
    const char8_t*      aRight
)
{
    std::u8string_view  right(aRight);
    std::u8string       res;
    res.reserve(aLeft.length() + right.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(right.data(), right.length());

    return res;
}

inline std::u8string operator+
(
    std::u8string_view  aLeft,
    const char*         aRight
)
{
    std::u8string_view  right(::GPlatform::GpUTF::S_STR_To_UTF8(aRight));
    std::u8string       res;
    res.reserve(aLeft.length() + right.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(right.data(), right.length());

    return res;
}

inline std::u8string operator+
(
    std::u8string_view  aLeft,
    const char8_t       aRight
)
{
    std::u8string   res;
    res.reserve(aLeft.length() + 1);
    res.append(aLeft.data(), aLeft.length());
    res += aRight;

    return res;
}

template<typename T>
inline std::u8string operator+
(
    std::u8string_view  aLeft,
    T                   aRight
)
{
    const std::u8string value(::GPlatform::GpStringOps::SToString(aRight));

    std::u8string res;
    res.reserve(aLeft.length() + value.length());
    res.append(aLeft.data(), aLeft.length());
    res.append(value.data(), value.length());

    return res;
}

}//std

#endif//#if defined(GP_USE_STRINGS)
