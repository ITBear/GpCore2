#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_STRINGS)

#include <vector>
#include <string>
#include <string_view>
#include <variant>
#include <thread>
#include <set>

#include "../../Macro/GpMacroClass.hpp"
#include "../../Algorithms/GpSplit.hpp"
#include "../Containers/GpBytesArray.hpp"

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

    // -------------------------------- Compare ----------------------------------
    static bool                             SIsEqualCaseInsensitive8bit
                                                            (std::string_view   aStr1,
                                                             std::string_view   aStr2) noexcept;

    // ------------------------- Numeric from/to string --------------------------
    static size_t                           SFromUI64       (const u_int_64 aValue,
                                                             GpSpanCharRW   aStrOut);
    static std::string                      SFromUI64       (const u_int_64 aValue);

    static size_t                           SFromSI64       (const s_int_64 aValue,
                                                             GpSpanCharRW   aStrOut);
    static std::string                      SFromSI64       (const s_int_64 aValue);

    static size_t                           SFromDouble     (const double   aValue,
                                                             GpSpanCharRW   aStrOut);
    static std::string                      SFromDouble     (const double aValue);

    static u_int_64                         SToUI64         (std::string_view                               aStr,
                                                             std::optional<std::reference_wrapper<size_t>>  aReadCountOut = std::nullopt);
    static s_int_64                         SToSI64         (std::string_view                               aStr,
                                                             std::optional<std::reference_wrapper<size_t>>  aReadCountOut = std::nullopt);

    // Supported format: [+-][UInt64][.[UInt64]]
    static double                           SToDouble       (std::string_view                               aStr,
                                                             std::optional<std::reference_wrapper<size_t>>  aReadCountOut = std::nullopt);

    // [+-][UInt64][.[UInt64]] - DOUBLE
    // [+-]digits - INT
    static std::variant<s_int_64, double>   SToNumeric      (std::string_view                               aStr,
                                                             std::optional<std::reference_wrapper<size_t>>  aReadCountOut = std::nullopt);

    // ------------------------- Bytes from/to string --------------------------
    static size_t                           SFromBytesHex   (GpSpanByteR    aData,
                                                             GpSpanCharRW   aStrOut);
    static std::string                      SFromBytesHex   (GpSpanByteR    aData);
    static inline std::array<char, 2>       SFromByteHex    (const u_int_8  aData) noexcept;

    static size_t                           SToBytesHex     (std::string_view   aStr,
                                                             GpSpanByteRW       aDataOut);
    static GpBytesArray                     SToBytesHex     (std::string_view   aStr);
    static constexpr u_int_8                SToByteHex      (std::array<char, 2>aStr);

    // ------------------------- Encode --------------------------
    static std::string                      SPercentEncode  (std::string_view aSrc);

    // -----------------------------------------------------------
    template<typename E,
             typename T>
    static std::string                      SJoin           (const T&           aArray,
                                                             std::string_view   aSeparator);

    template<typename E,
             typename T>
    static std::string                      SJoin           (const T&                                       aArray,
                                                             std::function<E(typename T::const_iterator&)>  aGetterFn,
                                                             std::string_view                               aSeparator);

    static constexpr size_t                 SCharsCount     (std::string_view   aStr,
                                                             const char         aChar) noexcept;

private:
    static void                             _SFromUI64      (const u_int_64 aValue,
                                                             GpSpanCharRW   aStrOut);

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

std::array<char, 2> GpStringOps::SFromByteHex (const u_int_8 aData) noexcept
{
    constexpr std::array<char, 16>  hexToChar =
    {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
    };

    return {std::data(hexToChar)[(size_t{aData} & size_t{0xF0}) >> 4],
            std::data(hexToChar)[(size_t{aData} & size_t{0x0F}) >> 0]};
}

constexpr u_int_8   GpStringOps::SToByteHex (std::array<char, 2> aStr)
{
    // --------------------------
    char    ch      = std::data(aStr)[0];
    size_t  valHi   = 0;
    size_t  valLo   = 0;
    size_t  beginCh = 0;
    size_t  shift   = 0;

    if ((ch >= '0') && (ch <= '9'))     {beginCh = size_t{'0'}; shift = 0;}
    else if ((ch >= 'A') && (ch <= 'Z')){beginCh = size_t{'A'}; shift = 10;}
    else if ((ch >= 'a') && (ch <= 'z')){beginCh = size_t{'a'}; shift = 10;}
    else GpThrowCe<GpException>("Wrong HEX character");

    valHi = u_int_8(size_t(ch) - beginCh + shift);

    // --------------------------
    ch = std::data(aStr)[1];

    if ((ch >= '0') && (ch <= '9'))     {beginCh = size_t{'0'}; shift = 0;}
    else if ((ch >= 'A') && (ch <= 'Z')){beginCh = size_t{'A'}; shift = 10;}
    else if ((ch >= 'a') && (ch <= 'z')){beginCh = size_t{'a'}; shift = 10;}
    else GpThrowCe<GpException>("Wrong HEX character");

    valLo = u_int_8(size_t(ch) - beginCh + shift);

    return u_int_8(valHi << 4) | u_int_8(valLo);
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
    const size_t elementsCount = std::size(aArray);
    std::string res;
    res.reserve(elementsCount * 16);

    bool isFirst = true;
    for (typename T::const_iterator iter = std::begin(aArray); iter != std::end(aArray); ++iter)
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
    const char          aChar
) noexcept
{
    size_t          count       = 0;
    const char* _R_ data        = std::data(aStr);
    const size_t    countLeft   = std::size(aStr);

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

}// namespace GPlatform

// *********************************************************

namespace std {

inline ::std::string to_string(::std::string_view aStrView)
{
    return ::std::string(aStrView);
}

inline ::std::string to_string(::std::string aStrView)
{
    return ::std::string(std::move(aStrView));
}

inline ::std::string to_string(const void* aPtr)
{
    std::array<std::byte, sizeof(void*)> ptrData;
    std::memcpy(std::data(ptrData), &aPtr, sizeof(void*));
    return ::GPlatform::StrOps::SFromBytesHex(ptrData);
}

template<::GPlatform::Concepts::IsIntergal T>
::std::string   to_string (const T aValue)
{
    if constexpr (std::is_signed_v<T>)
    {
        return ::GPlatform::GpStringOps::SFromSI64(::GPlatform::NumOps::SConvert<s_int_64>(aValue));
    } else
    {
        return ::GPlatform::GpStringOps::SFromUI64(::GPlatform::NumOps::SConvert<u_int_64>(aValue));
    }
}

template<::GPlatform::Concepts::IsFloatingPoint T>
::std::string   to_string (const T aValue)
{
    return ::GPlatform::GpStringOps::SFromDouble(double{aValue});
}

inline ::std::string    to_string (const ::std::thread::id aThreadId)
{
    using integralT = std::conditional_t
    <
        sizeof(::std::thread::id) == sizeof(u_int_32),
        u_int_32,
        u_int_64
    >;

    const integralT val = ::std::bit_cast<integralT>(aThreadId);

    return ::std::to_string(val);
}

inline ::std::string_view   to_string (const bool aValue) noexcept
{
    return aValue ? "true"_sv : "false"_sv;
}

inline ::std::string    to_string (const ::GPlatform::GpBytesArray& aValue)
{
    return ::GPlatform::StrOps::SFromBytesHex(aValue);
}

// --------------------------------- operator+ ---------------------------------

template<::GPlatform::Concepts::IsIntergal T>
inline ::std::string operator+
(
    ::std::string_view  aLeft,
    T                   aRight
)
{
    const ::std::string value(::std::to_string(aRight));

    ::std::string res;
    res.reserve(aLeft.length() + value.length());
    res.append(std::data(aLeft), aLeft.length());
    res.append(std::data(value), value.length());

    return res;
}

inline ::std::string operator+
(
    ::std::string_view  aLeft,
    ::std::string_view  aRight
)
{
    ::std::string res;
    res.reserve(aLeft.length() + aRight.length());
    res.append(std::data(aLeft), aLeft.length());
    res.append(std::data(aRight), aRight.length());

    return res;
}

inline ::std::string operator+
(
    ::std::string_view  aLeft,
    const char*         aRight
)
{
    return aLeft + ::std::string_view(aRight);
}

inline ::std::string operator+
(
    const char*         aRight,
    ::std::string_view  aLeft
)
{
    return ::std::string_view(aLeft) + aRight;
}

// --------------------------------- string_view_equal ---------------------------------

struct string_view_equal
{
    using is_transparent = std::true_type;

    bool operator()(std::string_view a, std::string_view b) const noexcept
    {
        return a == b;
    }
};

// --------------------------------- string_view_hash ---------------------------------

struct string_view_hash
{
    using is_transparent = std::true_type;

    auto operator()(std::string_view aStr) const noexcept
    {
        return std::hash<std::string_view>()(aStr);
    }
};

}// namespace std

#endif// #if defined(GP_USE_STRINGS)
