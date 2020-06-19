#pragma once

#include "../Types/Units/GpUnits.hpp"
#include "../Types/Strings/GpStringOpsGlob.hpp"
#include "../Types/Strings/GpStringLiterals.hpp"
#include "../Types/Pointers/GpRawPtrR.hpp"
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
         typename ContainerRes
         >
[[nodiscard]]
ContainerRes    Split (GpRawPtrR<const Element*>    aElements,
                       GpRawPtrR<const Element*>    aDelim,
                       const count_t                aReturnPartsCountLimit, //(0 - no limits)
                       const count_t                aDelimCountLimit,       //(0 - no limits)
                       const SplitMode              aSplitMode)
{
    const Element*  partBegin   = aElements.Ptr();
    count_t         partLength  = 0_cnt;
    count_t         delimCount  = 0_cnt;
    const count_t   delimLength = aDelim.LengthLeft();
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

/*template<typename Element,
         typename ContainerRes
         >
[[nodiscard]]
ContainerRes    Split (const Element* _R_   aElements,
                       const count_t        aElementsCount,
                       const Element        aDelim,
                       const Element        aEscGroupBegin,
                       const Element        aEscGroupEnd,
                       const count_t        aReturnPartsCountLimit, //(0 - no limits)
                       const count_t        aDelimCountLimit,       //(0 - no limits)
                       const SplitMode      aSplitMode)
{
    const Element* _R_  partBegin   = aElements;
    size_t              partLength  = 0;
    size_t              delimCount  = 0;

    //Результат
    ContainerRes res;

    bool            isInEscGroup    = false;
    const size_t    elementsCount   = aElementsCount.ValueAs<size_t>();
    const size_t    delimCountLimit = aDelimCountLimit.ValueAs<size_t>();

    for (size_t id = 0; id < elementsCount; ++id)
    {
        Element currentElement = *aElements++;

        if (   (currentElement == aEscGroupBegin)
            && (isInEscGroup == false))
        {
            isInEscGroup = true;
            ++partLength;
        } else if (   (currentElement == aEscGroupEnd)
                   && (isInEscGroup == true))
        {
            isInEscGroup = false;
            ++partLength;
        } else if (   (currentElement == aDelim)
                   && (isInEscGroup == false))
        {
            if (   (aSplitMode == SplitMode::COUNT_ZERO_LENGTH_PARTS)
                || (partLength > 0))
            {
                res.emplace_back(partBegin, partLength);

                if (   (aReturnPartsCountLimit > 0_cnt)
                    && (res.size() >= aReturnPartsCountLimit.Value()))
                {
                    return res;
                }
            }

            ++delimCount;
            if (   (delimCountLimit > 0)
                && (delimCount >= delimCountLimit))
            {
                return res;
            }

            partBegin   = aElements;
            partLength  = 0;
        } else
        {
            ++partLength;
        }
    }

    if (aElementsCount > 0_cnt)
    {
        if (   (partLength > 0)
            || (aSplitMode == SplitMode::COUNT_ZERO_LENGTH_PARTS))
        {
            res.emplace_back(partBegin, partLength);
        }
    }

    return res;
}*/

}//Algo
}//GPlatform
