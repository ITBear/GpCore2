#include "GpReflectDiffProp_VecFloat.hpp"

namespace GPlatform {

GpReflectDiffProp_VecFloat::~GpReflectDiffProp_VecFloat (void) noexcept
{
}

GpReflectDiffProp_VecFloat::C::Opt::Val GpReflectDiffProp_VecFloat::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecFloat{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
