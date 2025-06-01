#pragma once

#include <GpCore2/GpUtils/GpMemOps.hpp>

namespace GPlatform::Algo {

// ---------------------------------------- SplitExt ----------------------------------------

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
ContainerRes    SplitExt
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

// ---------------------------------------- Split ----------------------------------------

template<typename ContainerT,
         typename ContainerResT,
         typename V>
ContainerResT   Split
(
    const ContainerT&   aElements,
    const V             aSplitVal,
    const V             aEscapeVal,
    const V             aSequenceVal
)
{
    ContainerResT                       result;
    typename ContainerResT::value_type  currentPart;
    currentPart.reserve(4);

    bool inSequence = false;
    bool isEscaped  = false;

    const auto* beginPart   = aElements.data();
    const auto* currentVal  = beginPart;
    const auto* stopVal = beginPart + std::size(aElements);

    while (currentVal < stopVal)
    {
        const auto val = *currentVal;

        if (isEscaped) [[unlikely]]
        {
            currentPart.push_back(val);
            currentVal++;
            isEscaped = false;
        } else if (inSequence) [[unlikely]]
        {
            if (val == aEscapeVal) [[unlikely]]
            {
                currentVal++;
                isEscaped = true;
            } else if (val == aSequenceVal) [[unlikely]]
            {
                if (!currentPart.empty())
                {
                    result.emplace_back(std::move(currentPart));
                    currentPart.reserve(4);
                }

                currentVal++;
                inSequence = false;
            } else
            {
                currentPart.push_back(val);
                currentVal++;
            }
        } else if (val == aEscapeVal) [[unlikely]]
        {
            currentVal++;
            isEscaped = true;
        } else if (val == aSequenceVal) [[unlikely]]
        {
            if (!currentPart.empty())
            {
                result.emplace_back(std::move(currentPart));
                currentPart.reserve(4);
            }

            currentVal++;
            inSequence = true;
        } else if (val == aSplitVal) [[unlikely]]
        {
            if (!currentPart.empty())
            {
                result.emplace_back(std::move(currentPart));
                currentPart.reserve(4);
            }

            currentVal++;
        } else
        {
            currentPart.push_back(val);
            currentVal++;
        }
    }

    // Add the last part
    if (!currentPart.empty())
    {
        result.emplace_back(std::move(currentPart));
    }

    return result;
}

}// namespace GPlatform::Algo
