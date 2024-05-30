#include "GpReflectDiffProp_VecUI32.hpp"

namespace GPlatform {

GpReflectDiffProp_VecUI32::~GpReflectDiffProp_VecUI32 (void) noexcept
{
}

GpReflectDiffProp_VecUI32::C::Opt::Val  GpReflectDiffProp_VecUI32::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecUI32{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
