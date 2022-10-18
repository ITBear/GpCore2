#pragma once

#include "../../GpMacro.hpp"

#if defined(GP_USE_CONTAINERS)

#include "../../GpMemOps.hpp"
#include "../../TypeTraits/GpTypeTraitsIterator.hpp"
#include "../Pointers/GpSpanPtr.hpp"
#include "GpContainersT.hpp"

namespace GPlatform {

using GpSpanPtrCharR    = GpSpanPtr<const char*>;
using GpSpanPtrCharRW   = GpSpanPtr<char*>;
using GpSpanPtrByteR    = GpSpanPtr<const u_int_8*>;
using GpSpanPtrByteRW   = GpSpanPtr<u_int_8*>;

using GpBytesArray      = std::vector<u_int_8>;

class GpBytesArrayUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBytesArrayUtils)

public:
    template<typename FROM, typename = std::enable_if_t<has_random_access_iter_v<FROM>, FROM>>
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
