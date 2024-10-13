#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecUI64.hpp>

namespace GPlatform {

GpReflectDiffProp_VecUI64::~GpReflectDiffProp_VecUI64 (void) noexcept
{
}

GpReflectDiffProp_VecUI64::C::Opt::Val  GpReflectDiffProp_VecUI64::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecUI64{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
