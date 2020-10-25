#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_UUID)

#include "../Numerics/GpNumerics.hpp"
#include "../Containers/GpContainersT.hpp"
#include "../Containers/GpRawPtrByte.hpp"

namespace GPlatform {

class GpRandom;

class GPCORE_API GpUUID
{
public:
    CLASS_DECLARE_DEFAULTS(GpUUID)

    using DataT = GpArray<std::byte, 16>;

public:
    constexpr                           GpUUID          (void) noexcept:iData(CE_Zero()) {}
    constexpr                           GpUUID          (const DataT& aData) noexcept:iData(aData) {}
    constexpr                           GpUUID          (const GpUUID& aUUID) noexcept:iData(aUUID.iData) {}
    constexpr                           GpUUID          (GpUUID&& aUUID) noexcept:iData(std::move(aUUID.iData)) {}
                                        ~GpUUID         (void) noexcept = default;

    [[nodiscard]] const DataT&          Data            (void) const noexcept {return iData;}
    [[nodiscard]] DataT&                Data            (void) noexcept {return iData;}
    [[nodiscard]] std::string_view      AsStringView    (void) const noexcept {return std::string_view(reinterpret_cast<const char*>(Data().data()), Data().size());}

    [[nodiscard]] std::string           ToString        (void) const;
    void                                FromString      (GpRawPtrCharR aStr);

    inline void                         Set             (const GpUUID& aUUID) noexcept;
    inline void                         Set             (GpUUID&& aUUID) noexcept;
    inline constexpr void               Set             (const DataT& aData) noexcept;
    inline constexpr void               Set             (DataT&& aData) noexcept;
    [[nodiscard]] inline bool           IsEqual         (const GpUUID& aUUID) const noexcept;
    [[nodiscard]] inline constexpr bool IsEqual         (const DataT& aData) const noexcept;
    [[nodiscard]] inline constexpr bool IsZero          (void) const noexcept;
    [[nodiscard]] inline constexpr bool IsNotZero       (void) const noexcept;
    inline constexpr void               Zero            (void) noexcept;

    inline GpUUID&                      operator=       (const GpUUID& aUUID) noexcept;
    inline GpUUID&                      operator=       (GpUUID&& aUUID) noexcept;
    inline constexpr GpUUID&            operator=       (const DataT& aData) noexcept;
    inline constexpr GpUUID&            operator=       (DataT&& aData) noexcept;

    inline bool                         operator>       (const GpUUID& aUUID) const noexcept;
    inline constexpr bool               operator>       (const DataT& aData) const noexcept;
    inline bool                         operator<       (const GpUUID& aUUID) const noexcept;
    inline constexpr bool               operator<       (const DataT& aData) const noexcept;
    inline bool                         operator==      (const GpUUID& aUUID) const noexcept;
    inline constexpr bool               operator==      (const DataT& aData) const noexcept;
    inline bool                         operator!=      (const GpUUID& aUUID) const noexcept;
    inline constexpr bool               operator!=      (const DataT& aData) const noexcept;

    void                                FromRandom      (GpRandom& aRandom);

    static GpUUID                       SGenRandom      (void);
    static GpUUID                       SFromString     (GpRawPtrCharR aStr);

    inline static consteval DataT       CE_FromString   (std::string_view aStr);
    inline static consteval std::byte   SToByte         (GpArray<char,2> aStr);
    inline static consteval DataT       CE_Zero         (void);

private:
    DataT                               iData;
};

void    GpUUID::Set (const GpUUID& aUUID) noexcept
{
    MemOps::SCopy(iData, aUUID.iData);
}

void    GpUUID::Set (GpUUID&& aUUID) noexcept
{
    MemOps::SConstructAndMove(iData, std::move(aUUID.iData));
}

constexpr void  GpUUID::Set (const DataT& aData) noexcept
{
    MemOps::SCopy(iData, aData);
}

constexpr void  GpUUID::Set (DataT&& aData) noexcept
{
    MemOps::SConstructAndMove(iData, std::move(aData));
}

bool    GpUUID::IsEqual (const GpUUID& aUUID) const noexcept
{
    return MemOps::SCompare(iData, aUUID.iData) == 0;
}

constexpr bool  GpUUID::IsEqual (const DataT& aData) const noexcept
{
    return MemOps::SCompare(iData, aData) == 0;
}

constexpr bool  GpUUID::IsZero (void) const noexcept
{
    return IsEqual(CE_Zero());
}

constexpr bool  GpUUID::IsNotZero (void) const noexcept
{
    return !IsZero();
}

constexpr void  GpUUID::Zero (void) noexcept
{
    MemOps::SCopy(iData, CE_Zero());
}

GpUUID& GpUUID::operator= (const GpUUID& aUUID) noexcept
{
    Set(aUUID);
    return *this;
}

GpUUID& GpUUID::operator= (GpUUID&& aUUID) noexcept
{
    Set(aUUID);
    return *this;
}

constexpr GpUUID&   GpUUID::operator= (const DataT& aData) noexcept
{
    Set(aData);
    return *this;
}

constexpr GpUUID&   GpUUID::operator= (DataT&& aData) noexcept
{
    Set(std::move(aData));
    return *this;
}

bool    GpUUID::operator> (const GpUUID& aUUID) const noexcept
{
    return MemOps::SCompare(iData, aUUID.iData) > 0;
}

constexpr bool  GpUUID::operator> (const DataT& aData) const noexcept
{
    return MemOps::SCompare(iData, aData) > 0;
}

bool    GpUUID::operator< (const GpUUID& aUUID) const noexcept
{
    return MemOps::SCompare(iData, aUUID.iData) < 0;
}

constexpr bool  GpUUID::operator< (const DataT& aData) const noexcept
{
    return MemOps::SCompare(iData, aData) < 0;
}

bool    GpUUID::operator== (const GpUUID& aUUID) const noexcept
{
    return MemOps::SCompare(iData, aUUID.iData) == 0;
}

constexpr bool  GpUUID::operator== (const DataT& aData) const noexcept
{
    return MemOps::SCompare(iData, aData) == 0;
}

bool    GpUUID::operator!= (const GpUUID& aUUID) const noexcept
{
    return MemOps::SCompare(iData, aUUID.iData) != 0;
}

constexpr bool  GpUUID::operator!= (const DataT& aData) const noexcept
{
    return MemOps::SCompare(iData, aData) != 0;
}

consteval GpUUID::DataT GpUUID::CE_FromString (std::string_view aStr)
{
    THROW_GPE_COND_CHECK_M(aStr.size() == 36, "Length of UUID string must be 36"_sv);

    DataT data = {};

    const char* _R_ strPtr  = aStr.data();
    std::byte* _R_  dataPtr = data.data();

    for (size_t id = 0; id < data.size(); ++id)
    {
        GpArray<char,2> str = {*strPtr++, *strPtr++};

        *dataPtr++ = SToByte(str);

        if ((id == 3) ||
            (id == 5) ||
            (id == 7) ||
            (id == 9))
        {
            ++strPtr;
        }
    }

    return data;
}

consteval std::byte GpUUID::SToByte (GpArray<char,2> aStr)
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

consteval GpUUID::DataT GpUUID::CE_Zero (void)
{
    return DataT {std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0),
                  std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0), std::byte(0)};
}

}//GPlatform

//*******************************************
namespace std {

inline string to_string(const GPlatform::GpUUID& aUUID)
{
    return aUUID.ToString();
}

}//std

#endif//#if defined(GP_USE_UUID)
