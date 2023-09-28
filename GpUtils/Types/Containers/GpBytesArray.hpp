#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../Pointers/GpSpanPtr.hpp"

namespace GPlatform {

using GpSpanPtrCharR    = GpSpanPtr<const char8_t*>;
using GpSpanPtrCharRW   = GpSpanPtr<char8_t*>;
using GpSpanPtrByteR    = GpSpanPtr<const u_int_8*>;
using GpSpanPtrByteRW   = GpSpanPtr<u_int_8*>;

using GpBytesArray      = std::vector<u_int_8>;//TODO: test and reimplement with no default value type

class GpBytesArrayUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBytesArrayUtils)

public:
    template<typename FROM>
    requires Concepts::HasRandomAccessIter<FROM>
    static GpBytesArray SMake (const FROM& aContainer)
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
};

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
