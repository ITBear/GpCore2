#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../Pointers/GpSpanPtr.hpp"

namespace GPlatform {

using GpSpanPtrCharU8R  = GpSpanPtr<const char8_t*>;
using GpSpanPtrCharU8RW = GpSpanPtr<char8_t*>;
using GpSpanPtrCharR    = GpSpanPtr<const char*>;
using GpSpanPtrCharRW   = GpSpanPtr<char*>;
using GpSpanPtrByteR    = GpSpanPtr<const u_int_8*>;
using GpSpanPtrByteRW   = GpSpanPtr<u_int_8*>;

using GpBytesArray      = std::vector<u_int_8>;//TODO: test and reimplement with no default value type

class GpBytesArrayUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBytesArrayUtils)

public:
    template<typename FROM>
    requires
           Concepts::HasRandomAccessIter<FROM>
        && Concepts::SizeOfValueType<FROM, 1>
    static GpBytesArray SConvert (const FROM& aContainer)
    {
        GpBytesArray res;

        const size_t size = aContainer.size();
        res.resize(size);
        MemOps::SCopy
        (
            res.data(),
            reinterpret_cast<const std::byte*>(aContainer.data()),
            size
        );

        return res;
    }

    template<typename T1, typename T2>
    requires
       Concepts::HasRandomAccessIter<T1>
    && Concepts::SizeOfValueType<T1, 1>
    && Concepts::HasRandomAccessIter<T2>
    && Concepts::SizeOfValueType<T2, 1>
    static T1&  SAppend
    (
        T1&         aDst,
        const T2&   aSrc
    )
    {
        const size_t oldSize = aDst.size();
        const size_t srcSize = aSrc.size();
        const size_t newSize = NumOps::SAdd(oldSize, srcSize);

        aDst.resize(newSize);
        MemOps::SCopy
        (
            aDst.data() + oldSize,
            reinterpret_cast<const std::byte*>(aSrc.data()),
            srcSize
        );

        return aDst;
    }

    static GpBytesArray&    SAppend
    (
        GpBytesArray&   aDst,
        GpSpanPtrByteR  aSrc
    )
    {
        const size_t oldSize = aDst.size();
        const size_t srcSize = aSrc.Size().Value();
        const size_t newSize = NumOps::SAdd(oldSize, srcSize);

        aDst.resize(newSize);
        MemOps::SCopy
        (
            aDst.data() + oldSize,
            aSrc.Ptr(),
            srcSize
        );

        return aDst;
    }
};

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
