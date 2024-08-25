#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_UUID)

#include <GpCore2/Config/IncludeExt/fmt.hpp>
#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Random/GpRandomIf.hpp>
#include <GpCore2/GpUtils/GpMemOps.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Units/Other/unix_ts_t.hpp>

#include <bit>
#include <string_view>

namespace GPlatform {

class GP_UTILS_API GpUUID
{
public:
    CLASS_DD(GpUUID)

    using DataT = std::array<u_int_8, 16>;

public:
    constexpr                       GpUUID          (void) noexcept:iData(CE_Zero()) {}
    explicit constexpr              GpUUID          (const u_int_128 aRaw) noexcept:iData(std::bit_cast<DataT>(aRaw)) {}
    constexpr                       GpUUID          (const DataT& aData) noexcept:iData(aData) {}
    constexpr                       GpUUID          (const GpUUID& aUUID) noexcept:iData(aUUID.iData) {}
    constexpr                       GpUUID          (GpUUID&& aUUID) noexcept:iData(std::move(aUUID.iData)) {}
                                    ~GpUUID         (void) noexcept = default;

    constexpr const DataT&          Data            (void) const noexcept {return iData;}
    constexpr DataT&                Data            (void) noexcept {return iData;}
    std::string_view                AsStringView    (void) const noexcept {return std::string_view(reinterpret_cast<const char*>(std::data(Data())), std::size(Data()));}
    constexpr u_int_128             AsUInt128       (void) const noexcept {return std::bit_cast<u_int_128>(iData);}

    std::string                     ToString        (void) const;
    void                            FromString      (std::string_view aStr);

    inline void                     Set             (const GpUUID& aUUID) noexcept;
    inline void                     Set             (GpUUID&& aUUID) noexcept;
    inline constexpr void           Set             (const DataT& aData) noexcept;
    inline constexpr void           Set             (DataT&& aData) noexcept;
    inline bool                     IsEqual         (const GpUUID& aUUID) const noexcept;
    inline constexpr bool           IsEqual         (const DataT& aData) const noexcept;
    inline constexpr bool           IsZero          (void) const noexcept;
    inline constexpr bool           IsNotZero       (void) const noexcept;
    inline constexpr void           Zero            (void) noexcept;

    inline GpUUID&                  operator=       (const GpUUID& aUUID) noexcept;
    inline GpUUID&                  operator=       (GpUUID&& aUUID) noexcept;
    inline constexpr GpUUID&        operator=       (const DataT& aData) noexcept;
    inline constexpr GpUUID&        operator=       (DataT&& aData) noexcept;

    inline bool                     operator>       (const GpUUID& aUUID) const noexcept;
    inline constexpr bool           operator>       (const DataT& aData) const noexcept;
    inline bool                     operator<       (const GpUUID& aUUID) const noexcept;
    inline constexpr bool           operator<       (const DataT& aData) const noexcept;
    inline bool                     operator==      (const GpUUID& aUUID) const noexcept;
    inline constexpr bool           operator==      (const DataT& aData) const noexcept;
    inline bool                     operator!=      (const GpUUID& aUUID) const noexcept;
    inline constexpr bool           operator!=      (const DataT& aData) const noexcept;

    inline GpUUID                   operator^       (const GpUUID& aUUID) const noexcept;

    static GpUUID                   SGenRandomV4    (void) noexcept;
    static GpUUID                   SGenRandomV4    (GpRandomIf& aRand) noexcept;
    static GpUUID                   SGenRandomV7    (void) noexcept;
    static GpUUID                   SGenRandomV7    (const unix_ts_ms_t aUnixTS) noexcept;
    static GpUUID                   SGenRandomV7    (const unix_ts_ms_t aUnixTS,
                                                     GpRandomIf&        aRand) noexcept;

    static GpUUID                   SFromString     (std::string_view aStr);

    inline static consteval DataT   CE_FromString   (std::string_view aStr);
    inline static consteval u_int_8 SToByte         (std::array<char, 2> aStr);
    inline static constexpr DataT   CE_Zero         (void) noexcept;

private:
    DataT                           iData;
};

void    GpUUID::Set (const GpUUID& aUUID) noexcept
{
    MemOps::SCopy(iData, aUUID.iData);
}

void    GpUUID::Set (GpUUID&& aUUID) noexcept
{
    MemOps::SCopy(iData, std::move(aUUID.iData));
}

constexpr void  GpUUID::Set (const DataT& aData) noexcept
{
    MemOps::SCopy(iData, aData);
}

constexpr void  GpUUID::Set (DataT&& aData) noexcept
{
    MemOps::SCopy(iData, std::move(aData));
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

GpUUID GpUUID::operator^ (const GpUUID& aUUID) const noexcept
{
    u_int_128 a;
    std::memcpy(&a, std::data(iData), sizeof(decltype(iData)));

    u_int_128 b;
    std::memcpy(&b, std::data(aUUID.iData), sizeof(decltype(iData)));

    u_int_128 c = a ^ b;

    decltype(iData) r;
    std::memcpy(std::data(r), &c, sizeof(decltype(iData)));

    return GpUUID(r);
}

consteval GpUUID::DataT GpUUID::CE_FromString (std::string_view aStr)
{
    if (std::size(aStr) != 36)
    {
        GpThrowCe<GpException>("Length of UUID string must be 36");
    }

    DataT data = {};

    const char* _R_ strPtr  = std::data(aStr);
    u_int_8* _R_    dataPtr = std::data(data);

    for (size_t id = 0; id < std::size(data); ++id)
    {
        std::array<char, 2> str = {*strPtr++, *strPtr++};

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

consteval u_int_8   GpUUID::SToByte (std::array<char, 2> aStr)
{
    //--------------------------
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

    //--------------------------
    ch = std::data(aStr)[1];

    if ((ch >= '0') && (ch <= '9'))     {beginCh = size_t{'0'}; shift = 0;}
    else if ((ch >= 'A') && (ch <= 'Z')){beginCh = size_t{'A'}; shift = 10;}
    else if ((ch >= 'a') && (ch <= 'z')){beginCh = size_t{'a'}; shift = 10;}
    else GpThrowCe<GpException>("Wrong HEX character");

    valLo = u_int_8(size_t(ch) - beginCh + shift);

    return u_int_8(valHi << 4) | u_int_8(valLo);
}

constexpr GpUUID::DataT GpUUID::CE_Zero (void) noexcept
{
    return DataT {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
}

}// namespace GPlatform

//*******************************************
namespace std {

inline ::std::string to_string(const ::GPlatform::GpUUID& aUUID)
{
    return aUUID.ToString();
}

template<>
struct hash<GPlatform::GpUUID>
{
    std::size_t operator()(const GPlatform::GpUUID& aUuid) const noexcept
    {
        static_assert
        (
               (sizeof(std::size_t) == sizeof(u_int_64))
            || (sizeof(std::size_t) == sizeof(u_int_32))
        );

        if constexpr(sizeof(std::size_t) == sizeof(u_int_64))
        {
            u_int_64 p1;
            u_int_64 p2;

            std::memcpy(&p1, std::data(aUuid.Data()) + 0, sizeof(u_int_64));
            std::memcpy(&p2, std::data(aUuid.Data()) + 8, sizeof(u_int_64));

            const std::size_t h1 = std::hash<u_int_64>{}(p1);
            const std::size_t h2 = std::hash<u_int_64>{}(p2);
            return h1 ^ (h2 << 1);
        } else if constexpr(sizeof(std::size_t) == sizeof(u_int_32))
        {
            u_int_32 p1;
            u_int_32 p2;
            u_int_32 p3;
            u_int_32 p4;

            std::memcpy(&p1, std::data(aUuid.Data()) +  0, sizeof(u_int_32));
            std::memcpy(&p2, std::data(aUuid.Data()) +  4, sizeof(u_int_32));
            std::memcpy(&p3, std::data(aUuid.Data()) +  8, sizeof(u_int_32));
            std::memcpy(&p4, std::data(aUuid.Data()) + 12, sizeof(u_int_32));

            const std::size_t h1 = std::hash<u_int_32>{}(p1);
            const std::size_t h2 = std::hash<u_int_32>{}(p2);
            const std::size_t h3 = std::hash<u_int_32>{}(p3);
            const std::size_t h4 = std::hash<u_int_32>{}(p4);

            return ((h1 ^ (h2 << 1)) ^ (h3 << 2)) ^ (h4 << 3);
        }
    }
};

}// std

using namespace std::literals::string_literals;

consteval inline ::GPlatform::GpUUID operator"" _uuid (const char* aStr, const size_t aLen)
{
    return ::GPlatform::GpUUID::CE_FromString(std::string_view(aStr, aLen));
}

// ------------------------------------ fmt ------------------------------------

namespace FMT_NAMESPASE {

using namespace GPlatform;

template<>
struct formatter<GpUUID>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& aCtx)
    {
        return std::begin(aCtx);
    }

    template<typename FormatContext>
    auto format(const GpUUID& aUUID, FormatContext& aCtx) const
    {
        const std::string str = aUUID.ToString();

        return ::fmt::format_to
        (
            aCtx.out(),
            "{}",
            str
        );
    }
};

}// namespace std

#endif// #if defined(GP_USE_UUID)
