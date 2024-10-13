#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecString.hpp>

namespace GPlatform {

GpReflectDiffProp_VecString::~GpReflectDiffProp_VecString (void) noexcept
{
}

GpReflectDiffProp_VecString::C::Opt::Val    GpReflectDiffProp_VecString::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{   
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecString{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
