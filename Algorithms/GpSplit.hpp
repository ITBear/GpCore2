#pragma once

#include "../Types/Units/GpUnits.hpp"
#include "../Types/Strings/GpStringOpsGlob.hpp"
#include "../Types/Strings/GpStringLiterals.hpp"
#include "../Types/Pointers/GpRawPtr.hpp"
#include "../Memory/GpMemOps.hpp"
#include "../Exceptions/GpExceptions.hpp"

namespace GPlatform {
namespace Algo {

enum class SplitMode
{
    SKIP_ZERO_LENGTH_PARTS,
    COUNT_ZERO_LENGTH_PARTS
};

template<typename Element,
         typename ContainerRes,
         typename RawPtrT
         >
[[nodiscard]]
ContainerRes    Split (RawPtrT          aElements,
                       RawPtrT          aDelim,
                       const count_t    aReturnPartsCountLimit, //(0 - no limits)
                       const count_t    aDelimCountLimit,       //(0 - no limits)
                       const SplitMode  aSplitMode)
{
    const Element*  partBegin   = aElements.Ptr();
    count_t         partLength  = 0_cnt;
    count_t         delimCount  = 0_cnt;
    const count_t   delimLength = aDelim.CountLeft();
    count_t         elementsLeft= aElements.CountLeft();

    ContainerRes res;

    while (elementsLeft > 0_cnt)
    {
        if (aElements.IsEqualByArgLen(aDelim))
        {
            if (   (aSplitMode == SplitMode::COUNT_ZERO_LENGTH_PARTS)
                || (partLength > 0_cnt))
            {               
                res.emplace_back(partBegin, partLength.ValueAs<size_t>());

                if (   (aReturnPartsCountLimit > 0_cnt)
                    && (res.size() >= aReturnPartsCountLimit.ValueAs<size_t>()))
                {
                    return res;
                }
            }

            ++delimCount;
            if (   (aDelimCountLimit > 0_cnt)
                && (delimCount >= aDelimCountLimit))
            {
                return res;
            }

            aElements.OffsetAdd(delimLength);
            elementsLeft-= delimLength;
            partBegin   = aElements.Ptr();
            partLength  = 0_cnt;
        } else
        {
            aElements++;
            elementsLeft-= 1_cnt;
            ++partLength;
        }
    }

    if (   (partLength > 0_cnt)
        || (aSplitMode == SplitMode::COUNT_ZERO_LENGTH_PARTS))
    {
        res.emplace_back(partBegin, partLength.ValueAs<size_t>());
    }

    return res;
}

}//Algo
}//GPlatform
