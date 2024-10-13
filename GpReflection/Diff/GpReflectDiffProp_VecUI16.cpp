#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecUI16.hpp>

namespace GPlatform {

GpReflectDiffProp_VecUI16::~GpReflectDiffProp_VecUI16 (void) noexcept
{
}

GpReflectDiffProp_VecUI16::C::Opt::Val  GpReflectDiffProp_VecUI16::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecUI16{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
