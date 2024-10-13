#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <GpCore2/GpUtils/Types/Pointers/GpSpan.hpp>

namespace GPlatform {

using GpSpanCharR   = GpSpan<const char>;
using GpSpanCharRW  = GpSpan<char>;
using GpSpanByteRW  = GpSpan<std::byte>;
using GpSpanByteR   = GpSpan<const std::byte>;

using GpBytesArray  = std::vector<std_byte_no_init>;

class GpBytesArrayUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBytesArrayUtils)

public:
    template<typename TO,
             typename FROM>
    requires
           Concepts::HasContiguousIter<TO>
        && Concepts::SizeOfValueType<TO, 1>
        && (Concepts::HasContiguousIter<FROM> || GpHasTag_GpSpan<FROM>())
        && Concepts::SizeOfValueType<FROM, 1>
    static TO SMake (const FROM& aContainer)
    {
        TO res;

        const size_t size = std::size(aContainer);
        res.resize(size);

        MemOps::SCopy
        (
            reinterpret_cast<std_byte_no_init*>(std::data(res)),
            reinterpret_cast<const std_byte_no_init*>(std::data(aContainer)),
            size
        );      

        return res;
    }

    template<typename T1, typename T2>
    requires
       Concepts::HasContiguousIter<T1>
    && Concepts::SizeOfValueType<T1, 1>
    && (Concepts::HasContiguousIter<T2> || GpHasTag_GpSpan<T2>())
    && Concepts::SizeOfValueType<T2, 1>
    static T1&  SAppend
    (
        T1&         aDst,
        const T2&   aSrc
    )
    {
        const size_t oldSize = std::size(aDst);
        const size_t srcSize = std::size(aSrc);
        const size_t newSize = NumOps::SAdd(oldSize, srcSize);

        aDst.resize(newSize);

        MemOps::SCopy
        (
            reinterpret_cast<std::byte*>(std::data(aDst)) + oldSize,
            reinterpret_cast<const std::byte*>(std::data(aSrc)),
            srcSize
        );

        return aDst;
    }

    //static GpBytesArray&  SAppend
    //(
    //  GpBytesArray&   aDst,
    //  GpSpanByteR     aSrc
    //)
    //{
    //  const size_t oldSize = std::size(aDst);
    //  const size_t srcSize = aSrc.SizeInBytes();
    //  const size_t newSize = NumOps::SAdd(oldSize, srcSize);

    //  aDst.resize(newSize);

    //  MemOps::SCopy
    //  (
    //      std::data(aDst) + oldSize,
    //      reinterpret_cast<const std_byte_no_init*>(std::data(aSrc)),
    //      srcSize
    //  );

    //  return aDst;
    //}

    static GpSpanByteRW     SFillZero (GpSpanByteRW aData)
    {
        if (!aData.Empty())
        {
            std::memset
            (
                aData.Ptr(),
                0,
                aData.Count()
            );
        }

        return aData;
    }
};

}// namespace GPlatform

// --------------------- std -------------------------
namespace std {

inline bool operator<(const ::GPlatform::GpBytesArray& a, const ::GPlatform::GpBytesArray& b)
{
    const size_t sizeA = ::std::size(a);
    const size_t sizeB = ::std::size(b);

    if (sizeA < sizeB)
    {
        return true;
    }

    if (sizeA > sizeB)
    {
        return false;
    }

    if (sizeA == 0)
    {
        return false;
    }

    return std::memcmp(std::data(a), std::data(b), sizeA) < 0;
}

}// namespace std

#endif// #if defined(GP_USE_CONTAINERS)
