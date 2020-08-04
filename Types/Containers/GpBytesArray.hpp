#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "GpContainersT.hpp"
#include "../../Constexpr/GpConstexprIterator.hpp"
#include "../Pointers/GpRawPtr.hpp"

namespace GPlatform {

using GpBytesArray = GpVector<std::byte>;

class GpBytesArrayUtils
{
    CLASS_REMOVE_CTRS(GpBytesArrayUtils)

public:
    template<typename FROM, typename = std::enable_if_t<has_random_access_iter_v<FROM>, FROM>>
    static GpBytesArray         SMake   (const FROM& aContainer)
    {
        GpBytesArray res;

        const size_t size = aContainer.size();
        res.resize(size);
        MemOps::SCopy(res.data(),
                      reinterpret_cast<const std::byte*>(aContainer.data()),
                      count_t::SMake(size));

        return res;
    }

    static GpBytesArray         SMake   (GpRawPtr<const std::byte*> aData)
    {
        GpBytesArray res;
        res.resize(aData.CountLeftV<size_t>());
        GpRawPtr<std::byte*> resPtr(res);
        resPtr.CopyFrom(aData);
        return res;
    }
};

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
