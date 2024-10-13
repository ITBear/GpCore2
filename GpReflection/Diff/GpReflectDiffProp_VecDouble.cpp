#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecDouble.hpp>

namespace GPlatform {

GpReflectDiffProp_VecDouble::~GpReflectDiffProp_VecDouble (void) noexcept
{
}

GpReflectDiffProp_VecDouble::C::Opt::Val    GpReflectDiffProp_VecDouble::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecDouble{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
