#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecSI8.hpp>

namespace GPlatform {

GpReflectDiffProp_VecSI8::~GpReflectDiffProp_VecSI8 (void) noexcept
{
}

GpReflectDiffProp_VecSI8::C::Opt::Val   GpReflectDiffProp_VecSI8::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecSI8{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
