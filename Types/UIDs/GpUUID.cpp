#include "GpUUID.hpp"

#if defined(GP_USE_UUID)

#include "../../RandomGenerators/GpSRandom.hpp"
#include "../Strings/GpStringOps.hpp"
#include "../DateTime/GpDateTimeOps.hpp"

namespace GPlatform {

std::string GpUUID::ToString (void) const
{
    //    8      4    4    4       12
    //xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx
    std::string str;
    str.resize(36);
    char* _R_ strPtr = str.data();

    DataT data;
    MemOps::SCopy(data, iData);
    const std::byte* _R_ dataPtr = data.data();

    for (size_t id = 0; id < data.size(); ++id)
    {
        GpArray<char,2> h = StrOps::SFromByteHex(*dataPtr++);
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

void    GpUUID::FromString (GpRawPtrCharR aStr)
{
    THROW_GPE_COND
    (
        aStr.SizeTotal().As<size_t>() == 36,
        "Length of UUID string must be 36"_sv
    );

    DataT data;

    const char* _R_ strPtr  = aStr.Ptr();
    std::byte* _R_  dataPtr = data.data();

    for (size_t id = 0; id < data.size(); ++id)
    {
        GpArray<char,2> str = {*strPtr++, *strPtr++};

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

void    GpUUID::FromRandom (GpRandom& aRandom)
{
    const UInt64 part_0 = aRandom.UI64();
    const UInt64 part_1 = aRandom.UI64();

    std::byte*          dataPtr     = iData.data();
    constexpr size_t    blockSize   = sizeof(u_int_64);


    std::memcpy(dataPtr, &part_0, blockSize); dataPtr += blockSize;
    std::memcpy(dataPtr, &part_1, blockSize);
}

GpUUID  GpUUID::SGenRandom (void)
{
    GpSRandom& rnd = GpSRandom::S();

    const UInt64 part_0 = rnd.UI64();
    const UInt64 part_1 = rnd.UI64();

    DataT data;

    std::byte*          dataPtr     = data.data();
    constexpr size_t    blockSize   = sizeof(u_int_64);

    std::memcpy(dataPtr, &part_0, blockSize); dataPtr += blockSize;
    std::memcpy(dataPtr, &part_1, blockSize);

    return GpUUID(data);
}

GpUUID  GpUUID::SGenRandomV7 (void)
{
    return SGenRandomV7(GpDateTimeOps::SUnixTS_ms());
}

GpUUID  GpUUID::SGenRandomV7 (const unix_ts_ms_t aUnixTS)
{
    //https://www.ietf.org/archive/id/draft-peabody-dispatch-new-uuid-format-03.html
    //https://datatracker.ietf.org/doc/html/draft-peabody-dispatch-new-uuid-format-03#section-5.2
    //https://datatracker.ietf.org/doc/html/draft-peabody-dispatch-new-uuid-format-03#appendix-B.2
    //http://www.madore.org/~david/computers/unix-leap-seconds.html

    GpUUID      uuid    = SGenRandom();
    u_int_8*    dataPtr = reinterpret_cast<u_int_8*>(uuid.iData.data());

    //48 bit big-endian unsigned number of Unix epoch timestamp (ms)
    {
        static_assert(std::is_same_v<s_int_64, unix_ts_ms_t::value_type>);

        const s_int_64 ts = aUnixTS.Value();

        THROW_GPE_COND
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

GpUUID  GpUUID::SFromString (GpRawPtrCharR aStr)
{
    GpUUID uuid;
    uuid.FromString(aStr);
    return uuid;
}

}//GPlatform

#endif//#if defined(GP_USE_UUID)
