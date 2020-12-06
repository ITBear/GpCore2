#include "GpUUID.hpp"

#if defined(GP_USE_UUID)

#include "../../RandomGenerators/GpSRandom.hpp"
#include "../Strings/GpStringOps.hpp"

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
        GpArray<char,2> h = StrOps::SFromByte(*dataPtr++);
        *strPtr++ = h.at(0);
        *strPtr++ = h.at(1);

        if ((id == 3) ||
            (id == 5) ||
            (id == 7) ||
            (id == 9))
        {
            *strPtr++ = '-';
        }
    }

    return str;
}

void    GpUUID::FromString (GpRawPtrCharR aStr)
{
    THROW_GPE_COND_CHECK_M(aStr.SizeTotal().As<size_t>() == 36, "Length of UUID string must be 36"_sv);

    DataT data;

    const char* _R_ strPtr  = aStr.Ptr();
    std::byte* _R_  dataPtr = data.data();

    for (size_t id = 0; id < data.size(); ++id)
    {
        GpArray<char,2> str = {*strPtr++, *strPtr++};

        *dataPtr++ = StrOps::SToByte(str);

        if ((id == 3) ||
            (id == 5) ||
            (id == 7) ||
            (id == 9))
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

GpUUID  GpUUID::SFromString (GpRawPtrCharR aStr)
{
    GpUUID uuid;
    uuid.FromString(aStr);
    return uuid;
}

}//GPlatform

#endif//#if defined(GP_USE_UUID)
