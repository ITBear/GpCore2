#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Numerics/GpNumericOps.hpp>

namespace GPlatform {

template<typename MultiKeyT,
         typename MapT>
class GpUnorderedPairIndexer
{
public:
    using this_type                 = GpUnorderedPairIndexer<MultiKeyT, MapT>;
    using multi_key_type            = MultiKeyT;
    using key_type                  = typename MapT::key_type;
    using index_type                = typename MapT::mapped_type;
    using internal_container_type   = MapT;

    static_assert(std::is_unsigned_v<index_type>,   "index_type must be unsigned");
    static_assert(std::is_unsigned_v<multi_key_type>, "multi_key_type must be unsigned");
    static_assert(sizeof(multi_key_type) == 2*sizeof(index_type), "multi_key_type must be exactly 2 * index_type width");

public:
                    GpUnorderedPairIndexer  (void) noexcept = default;
                    ~GpUnorderedPairIndexer (void) noexcept = default;

    void            Clear                   (void);

    multi_key_type  AddPair                 (const key_type& aKeyA,
                                             const key_type& aKeyB);

private:
    index_type      GetOrAssignIndex        (const key_type& aKeyA);
    multi_key_type  PackUnordered           (index_type aIndexA,
                                             index_type aIndexB) const;

private:
    internal_container_type iValToIdxMap;
};

template<typename MultiKeyT,
         typename MapT>
void    GpUnorderedPairIndexer<MultiKeyT, MapT>::Clear (void)
{
    iValToIdxMap.clear();
}

template<typename MultiKeyT,
         typename MapT>
auto    GpUnorderedPairIndexer<MultiKeyT, MapT>::AddPair
(
    const key_type& aKeyA,
    const key_type& aKeyB
) -> multi_key_type
{
    const index_type indexA = GetOrAssignIndex(aKeyA);
    const index_type indexB = GetOrAssignIndex(aKeyB);

    return PackUnordered(indexA, indexB);
}

template<typename MultiKeyT,
         typename MapT>
auto    GpUnorderedPairIndexer<MultiKeyT, MapT>::GetOrAssignIndex (const key_type& aKeyA) -> index_type
{
    const index_type nextIndex = NumOps::SConvert<index_type>(std::size(iValToIdxMap));
    const auto iter = iValToIdxMap.try_emplace(aKeyA, nextIndex).first;

    return iter->second;
}

template<typename MultiKeyT,
         typename MapT>
auto    GpUnorderedPairIndexer<MultiKeyT, MapT>::PackUnordered
(
    const index_type aIndexA,
    const index_type aIndexB
) const -> multi_key_type
{
    const auto [minIdx, maxIdx] = NumOps::SMinMax(aIndexA, aIndexB);

    return (multi_key_type{maxIdx} << (sizeof(index_type)*8)) | multi_key_type{minIdx};
}

}// namespace GPlatform
