#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>

#include <variant>

namespace GPlatform {

template<typename       SrcContainerT,
         typename       ReplaceValueT,
         typename       HelpersT,
         typename...    ArgsT>
class GpReflectDiffArray
{
public:
    using IndexT                = u_int_32;
    using SizeT                 = u_int_32;
    using SrcValueT             = typename SrcContainerT::value_type;
    using ReplaceElementT       = std::tuple<IndexT/*index*/, ReplaceValueT/*value*/>;
    using ReplaceVectorT        = std::tuple<SizeT/*new size*/, std::vector<ReplaceElementT>/*elements to replace*/>;
    using FullReplaceVectorT    = std::vector<ReplaceValueT>;
    using ReplaceVariantT       = std::variant
    <
        FullReplaceVectorT, // Full replace
        ReplaceVectorT      // Resize and replace
    >;

    using DistanceT = std::tuple
    <
        size_t/*distance for full replace*/,
        size_t/*distance for replace selected values(by index)*/,
        size_t/*count of elements to replace*/
    >;

public:
    static std::optional<ReplaceVariantT>   SCheckAndMake   (const SrcContainerT&   aContainerFrom,
                                                             const SrcContainerT&   aContainerTo,
                                                             ArgsT&...              aArgs);

private:
    static DistanceT                        SDistance       (const SrcContainerT&   aContainerFrom,
                                                             const SrcContainerT&   aContainerTo,
                                                             ArgsT&...              aArgs);
    static ReplaceVectorT                   SCalcReplace    (const SrcContainerT&   aContainerFrom,
                                                             const SrcContainerT&   aContainerTo,
                                                             size_t                 aCountOfElementsToReplace,
                                                             ArgsT&...              aArgs);
    static FullReplaceVectorT               SFullReplace    (const SrcContainerT&   aSrcContainer,
                                                             ArgsT&...              aArgs);
};

template<typename       SrcContainerT,
         typename       ReplaceValueT,
         typename       HelpersT,
         typename...    ArgsT>
auto    GpReflectDiffArray<SrcContainerT, ReplaceValueT, HelpersT, ArgsT...>::SCheckAndMake
(
    const SrcContainerT&    aContainerFrom,
    const SrcContainerT&    aContainerTo,
    ArgsT&...               aArgs
) -> std::optional<ReplaceVariantT>
{
    const auto[distanceFullReplace, distancePartialReplace, countOfElementsToReplace] = SDistance
    (
        aContainerFrom,
        aContainerTo,
        std::forward<ArgsT&>(aArgs)...
    );

    // Check if distance is 0
    if (countOfElementsToReplace == 0)
    {
        return std::nullopt;
    }

    if (distanceFullReplace <= distancePartialReplace)
    {
        // Full replace
        return ReplaceVariantT
        {
            SFullReplace
            (
                aContainerTo,
                std::forward<ArgsT&>(aArgs)...
            )
        };
    } else
    {
        // Replace
        return ReplaceVariantT
        {
            SCalcReplace
            (
                aContainerFrom,
                aContainerTo,
                countOfElementsToReplace,
                std::forward<ArgsT&>(aArgs)...
            )
        };
    }
}

template<typename       SrcContainerT,
         typename       ReplaceValueT,
         typename       HelpersT,
         typename...    ArgsT>
auto    GpReflectDiffArray<SrcContainerT, ReplaceValueT, HelpersT, ArgsT...>::SDistance
(
    const SrcContainerT&    aContainerFrom,
    const SrcContainerT&    aContainerTo,
    ArgsT&...               aArgs
) -> DistanceT
{
    const size_t        sizeFrom                    = std::size(aContainerFrom);
    const size_t        sizeTo                      = std::size(aContainerTo);
    const size_t        minSize                     = std::min(sizeFrom, sizeTo);
    const SrcValueT*    ptrFrom                     = std::data(aContainerFrom);
    const SrcValueT*    ptrTo                       = std::data(aContainerTo);
    size_t              distanceFullReplace         = 0;
    size_t              distancePartialReplace      = 0;
    size_t              countOfElementsToReplace    = 0;

    // Get distance from common part
    for (size_t id = 0; id < minSize; id++)
    {
        if constexpr(std::is_arithmetic_v<SrcValueT> || std::is_same_v<SrcValueT, GpUUID> || std::is_same_v<SrcValueT, std_byte_no_init>)
        {
            const auto& valueFrom   = *ptrFrom++;
            const auto& valueTo     = *ptrTo++;

            bool isEqual;

            if constexpr(std::is_arithmetic_v<SrcValueT>)
            {
                isEqual = NumOps::SIsEqual(valueFrom, valueTo);
            } else
            {
                isEqual = valueFrom == valueTo;
            }

            const size_t    isEqualInt  = std::size_t{isEqual};

            countOfElementsToReplace    += isEqualInt;
            distancePartialReplace      += isEqualInt * sizeof(SrcValueT);
            distanceFullReplace         += sizeof(SrcValueT);
        } else
        {
            const auto& valueFrom   = HelpersT::GetValAndIncPtrFrom(ptrFrom, std::forward<ArgsT&>(aArgs)...);
            const auto& valueTo     = HelpersT::GetValAndIncPtrTo(ptrTo, std::forward<ArgsT&>(aArgs)...);

            const bool      isEqual     = HelpersT::Compare(valueFrom, valueTo, std::forward<ArgsT&>(aArgs)...);
            const size_t    isEqualInt  = std::size_t{isEqual};
            const size_t    valueToSize = HelpersT::SizeInBytes(valueTo, std::forward<ArgsT&>(aArgs)...);

            countOfElementsToReplace    += isEqualInt;
            distancePartialReplace      += isEqualInt * valueToSize;
            distanceFullReplace         += valueToSize;
        }
    }

    // Get distance from tail
    if (sizeTo > sizeFrom)
    {
        if constexpr(std::is_arithmetic_v<SrcValueT> || std::is_same_v<SrcValueT, GpUUID> || std::is_same_v<SrcValueT, std_byte_no_init>)
        {
            const size_t tailSize = sizeTo - sizeFrom;

            countOfElementsToReplace    += tailSize;
            distancePartialReplace      += tailSize * sizeof(SrcValueT);
            distanceFullReplace         += tailSize * sizeof(SrcValueT);
        } else
        {
            const size_t tailSize = sizeTo - sizeFrom;
            countOfElementsToReplace += tailSize;

            for (size_t id = minSize; id < sizeTo; id++)
            {
                const auto&     valueTo     = HelpersT::GetValAndIncPtrTo(ptrTo, std::forward<ArgsT&>(aArgs)...);
                const size_t    valueToSize = HelpersT::SizeInBytes(valueTo, std::forward<ArgsT&>(aArgs)...);

                distancePartialReplace  += valueToSize;
                distanceFullReplace     += valueToSize;
            }
        }
    }

    return
    {
        distanceFullReplace,
        distancePartialReplace,
        countOfElementsToReplace
    };
}

template<typename       SrcContainerT,
         typename       ReplaceValueT,
         typename       HelpersT,
         typename...    ArgsT>
auto    GpReflectDiffArray<SrcContainerT, ReplaceValueT, HelpersT, ArgsT...>::SCalcReplace
(
    const SrcContainerT&    aContainerFrom,
    const SrcContainerT&    aContainerTo,
    const size_t            aCountOfElementsToReplace,
    ArgsT&...               aArgs
) -> ReplaceVectorT
{
    ReplaceVectorT replace;

    // Set target size
    std::get<0>(replace) = NumOps::SConvert<SizeT>(std::size(aContainerTo));

    // Fill indexes and values
    std::vector<ReplaceElementT>& elementsToReplace = std::get<1>(replace);
    elementsToReplace.resize(aCountOfElementsToReplace);
    size_t elementsToReplaceId = 0;

    const size_t        sizeFrom    = std::size(aContainerFrom);
    const size_t        sizeTo      = std::size(aContainerTo);
    const size_t        minSize     = std::min(sizeFrom, sizeTo);
    const SrcValueT*    ptrFrom     = std::data(aContainerFrom);
    const SrcValueT*    ptrTo       = std::data(aContainerTo);

    // Process common part for both arrays
    for (size_t idx = 0; idx < minSize; idx++)
    {
        if constexpr(std::is_arithmetic_v<SrcValueT> || std::is_same_v<SrcValueT, GpUUID> || std::is_same_v<SrcValueT, std_byte_no_init>)
        {
            const auto& valueFrom   = *ptrFrom++;
            const auto& valueTo     = *ptrTo++;

            bool isEqual;

            if constexpr(std::is_arithmetic_v<SrcValueT>)
            {
                isEqual = NumOps::SIsEqual(valueFrom, valueTo);
            } else
            {
                isEqual = valueFrom == valueTo;
            }

            if (!isEqual)
            {
                elementsToReplace[elementsToReplaceId++] =
                {
                    NumOps::SConvert<IndexT>(idx),
                    valueTo
                };
            }
        } else
        {
            const auto& valueFrom   = HelpersT::GetValAndIncPtrFrom(ptrFrom, std::forward<ArgsT&>(aArgs)...);
            const auto& valueTo     = HelpersT::GetValAndIncPtrTo(ptrTo, std::forward<ArgsT&>(aArgs)...);

            const bool isEqual = HelpersT::Compare(valueFrom, valueTo, std::forward<ArgsT&>(aArgs)...);

            if (!isEqual)
            {
                elementsToReplace[elementsToReplaceId++] =
                {
                    NumOps::SConvert<IndexT>(idx),
                    HelpersT::Copy(valueTo)
                };
            }
        }
    }

    // Process tail of aContainerTo
    if (sizeTo > sizeFrom)
    {
        for (size_t idx = minSize; idx < sizeTo; idx++)
        {
            if constexpr(std::is_arithmetic_v<SrcValueT> || std::is_same_v<SrcValueT, GpUUID> || std::is_same_v<SrcValueT, std_byte_no_init>)
            {
                const auto& valueTo = *ptrTo++;

                elementsToReplace[elementsToReplaceId++] =
                {
                    NumOps::SConvert<IndexT>(idx),
                    valueTo
                };
            } else
            {
                const auto& valueTo = HelpersT::GetValAndIncPtrTo(ptrTo, std::forward<ArgsT&>(aArgs)...);

                elementsToReplace[elementsToReplaceId++] =
                {
                    NumOps::SConvert<IndexT>(idx),
                    HelpersT::Copy(valueTo)
                };
            }
        }
    }

    return replace;
}

template<typename       SrcContainerT,
         typename       ReplaceValueT,
         typename       HelpersT,
         typename...    ArgsT>
auto    GpReflectDiffArray<SrcContainerT, ReplaceValueT, HelpersT, ArgsT...>::SFullReplace
(
    const SrcContainerT&    aSrcContainer,
    ArgsT&...               aArgs
) -> FullReplaceVectorT
{
    FullReplaceVectorT  replaceContainer;
    const size_t        containerSize = std::size(aSrcContainer);

    replaceContainer.resize(containerSize);

    if constexpr(std::is_arithmetic_v<SrcValueT> || std::is_same_v<SrcValueT, GpUUID> || std::is_same_v<SrcValueT, std_byte_no_init>)
    {
        std::memcpy
        (
            std::data(replaceContainer),
            std::data(aSrcContainer),
            containerSize * sizeof(SrcValueT)
        );
    } else
    {
        auto*           replaceElementPtr   = std::data(replaceContainer);
        const auto*     srcElementPtr       = std::data(aSrcContainer);
        const size_t    size                = std::size(aSrcContainer);

        for (size_t id = 0; id < size; id++)
        {
            const auto& valueTo     = HelpersT::GetValAndIncPtrTo(srcElementPtr, std::forward<ArgsT&>(aArgs)...);
            *replaceElementPtr++    = HelpersT::Copy(valueTo);
        }
    }

    return replaceContainer;
}

}// namespace GPlatform
