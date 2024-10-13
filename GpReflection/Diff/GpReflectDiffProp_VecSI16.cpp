#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecSI16.hpp>

namespace GPlatform {

GpReflectDiffProp_VecSI16::~GpReflectDiffProp_VecSI16 (void) noexcept
{
}

GpReflectDiffProp_VecSI16::C::Opt::Val  GpReflectDiffProp_VecSI16::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecSI16{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
