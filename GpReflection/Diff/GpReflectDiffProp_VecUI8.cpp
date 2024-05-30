#include "GpReflectDiffProp_VecUI8.hpp"

namespace GPlatform {

GpReflectDiffProp_VecUI8::~GpReflectDiffProp_VecUI8 (void) noexcept
{
}

GpReflectDiffProp_VecUI8::C::Opt::Val   GpReflectDiffProp_VecUI8::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecUI8{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
