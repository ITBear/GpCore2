#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Numerics/GpNumericOps.hpp>

namespace GPlatform {

template<typename MultiKeyT,
         typename InternalContainerT>
class GpMultiKeyManager
{
public:
    using this_type                 = GpMultiKeyManager<MultiKeyT, InternalContainerT>;
    using multi_key_type            = MultiKeyT;
    using key_type                  = typename InternalContainerT::key_type;
    using index_type                = typename InternalContainerT::mapped_type;
    using internal_container_type   = InternalContainerT;

public:
                            GpMultiKeyManager   (void) noexcept = default;
                            ~GpMultiKeyManager  (void) noexcept = default;

    void                    Clear               (void);

    multi_key_type          AddKeys             (const key_type& aKeyA,
                                                 const key_type& aKeyB);

private:
    index_type              KeyToIndex          (const key_type& aKeyA);
    multi_key_type          MakeMultiKey        (index_type aIndexA,
                                                 index_type aIndexB) const;

private:
    internal_container_type iValToIdxMap;
};

template<typename MultiKeyT,
         typename InternalContainerT>
void    GpMultiKeyManager<MultiKeyT, InternalContainerT>::Clear (void)
{
    iValToIdxMap.clear();
}

template<typename MultiKeyT,
         typename InternalContainerT>
auto    GpMultiKeyManager<MultiKeyT, InternalContainerT>::AddKeys
(
    const key_type& aKeyA,
    const key_type& aKeyB
) -> multi_key_type
{
    const index_type indexA = KeyToIndex(aKeyA);
    const index_type indexB = KeyToIndex(aKeyB);

    return MakeMultiKey(indexA, indexB);
}

template<typename MultiKeyT,
         typename InternalContainerT>
auto    GpMultiKeyManager<MultiKeyT, InternalContainerT>::KeyToIndex (const key_type& aKeyA) -> index_type
{
    const index_type nextIndex = NumOps::SConvert<index_type>(std::size(iValToIdxMap));
    const auto iter = iValToIdxMap.try_emplace(aKeyA, nextIndex).first;

    return iter->second;
}

template<typename MultiKeyT,
         typename InternalContainerT>
auto    GpMultiKeyManager<MultiKeyT, InternalContainerT>::MakeMultiKey
(
    const index_type aIndexA,
    const index_type aIndexB
) const -> multi_key_type
{
    static_assert(sizeof(index_type)*2 == sizeof(multi_key_type));
    const auto [minIdx, maxIdx] = NumOps::SMinMax(aIndexA, aIndexB);

    return (multi_key_type{maxIdx} << (sizeof(index_type)*8)) | multi_key_type{minIdx};
}

}// namespace GPlatform
