#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecSI32.hpp>

namespace GPlatform {

GpReflectDiffProp_VecSI32::~GpReflectDiffProp_VecSI32 (void) noexcept
{
}

GpReflectDiffProp_VecSI32::C::Opt::Val  GpReflectDiffProp_VecSI32::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecSI32{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
