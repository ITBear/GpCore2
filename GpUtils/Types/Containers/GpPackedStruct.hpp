#pragma once

#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>
#include <GpCore2/GpUtils/Other/GpMethodAccessGuard.hpp>
#include <bitset>

namespace GPlatform {

template
<
    typename    EnumPartsT,
    u_int_8...  PartsSizeT
>
class GpPackedStruct
{
public:
    static constexpr size_t     SSizeInBits     (void);
    static constexpr size_t     SSizeInBytes    (void);

    using SizeVecT          = std::array<u_int_8, sizeof...(PartsSizeT)>;
    using OffsetVecT        = std::array<size_t, sizeof...(PartsSizeT)>;
    using this_type         = GpPackedStruct<EnumPartsT, PartsSizeT...>;
    using InternalStorageT  = std::array<std_byte_no_init, GpPackedStruct<EnumPartsT, PartsSizeT...>::SSizeInBytes()>;
    using StdBitsetT        = std::bitset<GpPackedStruct<EnumPartsT, PartsSizeT...>::SSizeInBits()>;

public:
                                GpPackedStruct      (void) noexcept = default;
                                GpPackedStruct      (const GpPackedStruct& aPackedStruct) noexcept;
                                ~GpPackedStruct     (void) noexcept = default;

    void                        FillZero            (void) noexcept {iStorage.fill(std::byte(0));}

    const InternalStorageT&     Storage             (void) const noexcept {return iStorage;}
    InternalStorageT&           Storage             (void) noexcept {return iStorage;}
    GpSpanByteR                 StorageAsSpan       (void) const noexcept {return GpSpanByteR{iStorage};}
    StdBitsetT                  StorageToStdBitset  (void) const;

    template<EnumPartsT EV,
             typename   T>
    void                        Pack                (T aValue);

    template<EnumPartsT EV,
             typename   T>
    T                           Unpack              (void) const noexcept;

    static constexpr SizeVecT   SPartsSize          (void);
    static constexpr OffsetVecT SPartsOffset        (void);

private:
    InternalStorageT            iStorage;
};

template
<
    typename    EnumPartsT,
    u_int_8...  PartsSizeT
>
GpPackedStruct<EnumPartsT, PartsSizeT...>::GpPackedStruct (const GpPackedStruct& aPackedStruct) noexcept:
iStorage{aPackedStruct.iStorage}
{
}

template
<
    typename    EnumPartsT,
    u_int_8...  PartsSizeT
>
auto    GpPackedStruct<EnumPartsT, PartsSizeT...>::StorageToStdBitset (void) const -> StdBitsetT
{
    StdBitsetT  bitset;
    size_t      bitIndex = 0;

    for (size_t id = 0; id < iStorage.size(); ++id)
    {
        const std::byte currentByte = iStorage[id].value;

        for (size_t b = 0; b < 8; ++b)
        {
            bitset[bitIndex++] = static_cast<bool>(currentByte & std::byte(1 << b));

            if (bitIndex == SSizeInBits())
            {
                break;
            }
        }
    }

    return bitset;
}

template
<
    typename    EnumPartsT,
    u_int_8...  PartsSizeT
>
template
<
    EnumPartsT  EV,
    typename    T
>
void    GpPackedStruct<EnumPartsT, PartsSizeT...>::Pack (T aValue)
{
    constexpr auto      partsOffset = SPartsOffset();
    constexpr auto      partsSize   = SPartsSize();
    constexpr size_t    partOffset  = partsOffset[size_t(EV)];
    constexpr size_t    partSize    = partsSize[size_t(EV)];

    static_assert(partSize <= (sizeof(u_int_64)*8), "All values in the parameter pack must be less than or equal to 64 bits");
    static_assert((sizeof(T)*8) <= (sizeof(u_int_64)*8), "sizeof(T)*8 must be less than or equal to 64 bits");
    static_assert(partSize <= (sizeof(T)*8), "partSize <= (sizeof(T)*8)");

    constexpr size_t    startBitId          = partOffset;
    constexpr size_t    endBitId            = partOffset + partSize - size_t{1};
    constexpr size_t    startByteId         = startBitId / 8;
    constexpr size_t    endByteId           = endBitId / 8;
    constexpr size_t    storageBytesCount   = endByteId - startByteId + 1;
    constexpr size_t    tmpBuffOffset       = partOffset % 8;

    using TmpBuffT = std::conditional<storageBytesCount <= sizeof(u_int_64), u_int_64, u_int_128>::type;

    // Copy from storage to tmp buffer 1
    TmpBuffT tmpBuff1;

    if constexpr(sizeof(tmpBuff1) != storageBytesCount)
    {
        tmpBuff1 = 0;
    }

    std::memcpy(&tmpBuff1, std::data(iStorage) + startByteId, storageBytesCount);

    // Copy from value to tmp buffer 2
    TmpBuffT tmpBuff2;

    if constexpr(sizeof(tmpBuff2) != sizeof(T))
    {
        tmpBuff2 = 0;
    }

    std::memcpy(&tmpBuff2, &aValue, sizeof(T));

    // Check if value fit to part
    constexpr TmpBuffT valueMask = BitOps::MakeMask1<TmpBuffT>(partSize);
    VERIFY
    (
        (tmpBuff2 & ~valueMask) == 0,
        "The value does not fit the part"
    );

    // Combine values   
    tmpBuff1 = (tmpBuff1 & ~(valueMask << tmpBuffOffset)) | (tmpBuff2 << tmpBuffOffset);

    // Copy to storage from tmp buffer 1
    std::memcpy(std::data(iStorage) + startByteId, &tmpBuff1, storageBytesCount);
}

template
<
    typename    EnumPartsT,
    u_int_8...  PartsSizeT
>
template
<
    EnumPartsT  EV,
    typename    T
>
T   GpPackedStruct<EnumPartsT, PartsSizeT...>::Unpack (void) const noexcept
{
    constexpr auto      partsOffset = SPartsOffset();
    constexpr auto      partsSize   = SPartsSize();
    constexpr size_t    partOffset  = partsOffset[size_t(EV)];
    constexpr size_t    partSize    = partsSize[size_t(EV)];

    static_assert(partSize <= (sizeof(u_int_64)*8), "All values in the parameter pack must be less than or equal to 64 bits");
    static_assert((sizeof(T)*8) <= (sizeof(u_int_64)*8), "sizeof(T)*8 must be less than or equal to 64 bits");
    static_assert(partSize <= (sizeof(T)*8), "partSize <= (sizeof(T)*8)");

    constexpr size_t    startBitId          = partOffset;
    constexpr size_t    endBitId            = partOffset + partSize - size_t{1};
    constexpr size_t    startByteId         = startBitId / 8;
    constexpr size_t    endByteId           = endBitId / 8;
    constexpr size_t    storageBytesCount   = endByteId - startByteId + 1;
    constexpr size_t    tmpBuffOffset       = partOffset % 8;

    using TmpBuffT = std::conditional<storageBytesCount <= sizeof(u_int_64), u_int_64, u_int_128>::type;

    T unpackedValue;

    if constexpr(sizeof(T) >= storageBytesCount)
    {
        constexpr T valueMask = BitOps::MakeMask1<T>(partSize);

        std::memcpy(&unpackedValue, std::data(iStorage) + startByteId, storageBytesCount);
        unpackedValue = (unpackedValue >> tmpBuffOffset) & valueMask;
    } else
    {
        constexpr TmpBuffT valueMask = BitOps::MakeMask1<TmpBuffT>(partSize);

        TmpBuffT tmpBuff;
        std::memcpy(&tmpBuff, std::data(iStorage) + startByteId, storageBytesCount);
        tmpBuff = (tmpBuff >> tmpBuffOffset) & valueMask;

        std::memcpy(&unpackedValue, &tmpBuff, sizeof(T));
    }

    return unpackedValue;
}

template
<
    typename    EnumPartsT,
    u_int_8...  PartsSizeT
>
constexpr size_t    GpPackedStruct<EnumPartsT, PartsSizeT...>::SSizeInBits (void)
{
    static_assert(size_t(EnumPartsT::_COUNT) == sizeof...(PartsSizeT));

    return (... + size_t{PartsSizeT});
}

template
<
    typename    EnumPartsT,
    u_int_8...  PartsSizeT
>
constexpr size_t    GpPackedStruct<EnumPartsT, PartsSizeT...>::SSizeInBytes (void)
{
    constexpr size_t sizeInBits     = SSizeInBits();
    constexpr size_t sizeInBytes    = (sizeInBits / 8) + (((sizeInBits % 8) > 0) ? 1 : 0);

    return sizeInBytes;
}

template
<
    typename    EnumPartsT,
    u_int_8...  PartsSizeT
>
constexpr auto  GpPackedStruct<EnumPartsT, PartsSizeT...>::SPartsSize (void) -> SizeVecT
{
    constexpr SizeVecT partsSize = {PartsSizeT...};

    return partsSize;
}

template
<
    typename    EnumPartsT,
    u_int_8...  PartsSizeT
>
constexpr auto  GpPackedStruct<EnumPartsT, PartsSizeT...>::SPartsOffset (void) -> OffsetVecT
{
    constexpr SizeVecT  partsSize   = SPartsSize();
    OffsetVecT          partsOffset;
    size_t              id          = 0;
    size_t              sum         = 0;

    for (const auto s: partsSize)
    {
        partsOffset[id++] = sum;
        sum += s;
    }

    return partsOffset;
}

}// namespace GPlatform
