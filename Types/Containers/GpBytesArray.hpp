#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_CONTAINERS)

#include "GpContainersT.hpp"
#include <iterator>
#include <concepts>

namespace GPlatform {

using GpBytesArray = GpVector<std::byte>;

class GpBytesArrayUtils
{
    CLASS_REMOVE_CTRS(GpBytesArrayUtils)

public:
    template<class C>
    using has_iterator_v                        = typename C::iterator;

    template<typename C>
    using iter_t                                = std::experimental::detected_or_t<void, has_iterator_v, C>;

    template <typename C>
    static constexpr bool is_ra_container_v     = std::random_access_iterator<iter_t<C>>;

public:
    template<typename FROM, typename = std::enable_if_t<is_ra_container_v<FROM>, FROM>> constexpr
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
};

}//GPlatform

#endif//#if defined(GP_USE_CONTAINERS)
