#pragma once

#include "../Types/Numerics/GpNumericTypes.hpp"
#include "../GpMemOps.hpp"

namespace GPlatform::Algo {

enum class SplitMode
{
    SKIP_ZERO_LENGTH_PARTS,
    COUNT_ZERO_LENGTH_PARTS
};

template<typename Element,
         typename ContainerRes,
         typename SpanPtrT
         >
[[nodiscard]]
ContainerRes    Split
(
    SpanPtrT        aElements,
    SpanPtrT        aDelim,
    const size_t    aReturnPartsCountLimit, //(0 - no limits)
    const size_t    aDelimCountLimit,       //(0 - no limits)
    const SplitMode aSplitMode
)
{
    size_t                      delimCount  = 0;
    const size_t                delimLength = aDelim.Count();
    typename SpanPtrT::pointer  partPtr     = aElements.Ptr();
    size_t                      partLength  = 0;

    ContainerRes res;

    if (aElements.Empty())
    {
        return res;
    }

    if (aDelim.Empty())
    {
        res.emplace_back(aElements.Ptr(), aElements.Count());
        return res;
    }

    while (aElements.Count() >= delimLength)
    {
        if (!MemOps::SIsEqual(aElements.Ptr(), delimLength, aDelim.Ptr(), delimLength))
        {
            aElements++;
            partLength++;

            continue;
        }

        if (    (partLength > 0)
             || (aSplitMode == SplitMode::COUNT_ZERO_LENGTH_PARTS))
        {
            res.emplace_back(partPtr, partLength);

            if (   (aReturnPartsCountLimit > 0)
                && (std::size(res) >= aReturnPartsCountLimit))
            {
                return res;
            }
        }

        delimCount++;

        if (   (aDelimCountLimit > 0)
            && (delimCount >= aDelimCountLimit))
        {
            return res;
        }

        aElements.OffsetAdd(delimLength);
        partPtr     = aElements.Ptr();
        partLength  = 0;
    }

    if (   (partLength > 0)
        || (aSplitMode == SplitMode::COUNT_ZERO_LENGTH_PARTS))
    {
        res.emplace_back(partPtr, partLength);
    }

    return res;
}

}// namespace GPlatform::Algo
