#include "GpUUID.hpp"

#if defined(GP_USE_UUID)

#include "../Strings/GpStringOps.hpp"
#include "../Bits/GpBitOps.hpp"
#include "../../Random/GpSRandom.hpp"
#include "../../DateTime/GpDateTimeOps.hpp"

namespace GPlatform {

std::string GpUUID::ToString (void) const
{
    //    8      4    4    4       12
    //xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    std::string str;
    str.resize(36);
    char* _R_ strPtr = std::data(str);

    DataT data;
    MemOps::SCopy(data, iData);
    const u_int_8* _R_ dataPtr = std::data(data);

    for (size_t id = 0; id < std::size(data); ++id)
    {
        std::array<char, 2> h = StrOps::SFromByteHex(*dataPtr++);
        *strPtr++ = h.at(0);
        *strPtr++ = h.at(1);

        if (   (id == 3)
            || (id == 5)
            || (id == 7)
            || (id == 9))
        {
            *strPtr++ = '-';
        }
    }

    return str;
}

void    GpUUID::FromString (std::string_view aStr)
{
    THROW_COND_GP
    (
        std::size(aStr) == 36,
        "Length of UUID string must be 36"_sv
    );

    DataT data;

    const char* _R_ strPtr  = std::data(aStr);
    u_int_8* _R_    dataPtr = std::data(data);

    for (size_t id = 0; id < std::size(data); ++id)
    {
        std::array<char,2> str = {*strPtr++, *strPtr++};

        *dataPtr++ = StrOps::SToByteHex(str);

        if (   (id == 3)
            || (id == 5)
            || (id == 7)
            || (id == 9))
        {
            ++strPtr;
        }
    }

    MemOps::SCopy(iData, data);
}

GpUUID  GpUUID::SGenRandomV4 (void) noexcept
{
    return SGenRandomV4(GpSRandom::S());
}

GpUUID  GpUUID::SGenRandomV4 (GpRandomIf& aRand) noexcept
{
    const u_int_64 part_0 = aRand.UI64();
    const u_int_64 part_1 = aRand.UI64();

    DataT data;

    u_int_8*            dataPtr     = std::data(data);
    constexpr size_t    blockSize   = sizeof(u_int_64);

    std::memcpy(dataPtr, &part_0, blockSize); dataPtr += blockSize;
    std::memcpy(dataPtr, &part_1, blockSize);

    data[6] = (data[6] & u_int_8(0x0F)) | u_int_8(0x40);

    return GpUUID(data);
}

GpUUID  GpUUID::SGenRandomV7 (void) noexcept
{
    return SGenRandomV7(GpDateTimeOps::SUnixTS_ms());
}

GpUUID  GpUUID::SGenRandomV7 (const unix_ts_ms_t aUnixTS) noexcept
{
    return SGenRandomV7(aUnixTS, GpSRandom::S());
}

GpUUID  GpUUID::SGenRandomV7
(
    const unix_ts_ms_t  aUnixTS,
    GpRandomIf&         aRand
) noexcept
{
    GpUUID      uuid    = SGenRandomV4(aRand);
    u_int_8*    dataPtr = reinterpret_cast<u_int_8*>(std::data(uuid.iData));

    //48 bit big-endian unsigned number of Unix epoch timestamp (ms)
    {
        static_assert(std::is_same_v<s_int_64, unix_ts_ms_t::value_type>);

        const s_int_64 ts = aUnixTS.Value();

        THROW_COND_GP
        (
            ts >= 0,
            "Unix timestamp < 0"_sv
        );

        const u_int_64 tsn =   BitOps::Native2BigEndian(u_int_64(ts) << 16)
                             | BitOps::Native2BigEndian(*reinterpret_cast<u_int_64*>(dataPtr) & u_int_64(0x000000000000FFFF));
        std::memcpy(dataPtr, &tsn, sizeof(tsn));
    }

    //Variant (0b11)
    {
        dataPtr[8] = u_int_8(0x80) | (dataPtr[8] & u_int_8(0x3F));
    }

    //Version (0b0111)
    {
        dataPtr[6] = u_int_8(0x70) | (dataPtr[6] & u_int_8(0x0F));
    }

    return uuid;
}

GpUUID  GpUUID::SFromString (std::string_view aStr)
{
    GpUUID uuid;
    uuid.FromString(aStr);
    return uuid;
}

}// namespace GPlatform

#endif// #if defined(GP_USE_UUID)
