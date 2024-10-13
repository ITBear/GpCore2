#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecUUID.hpp>

namespace GPlatform {

GpReflectDiffProp_VecUUID::~GpReflectDiffProp_VecUUID (void) noexcept
{
}

GpReflectDiffProp_VecUUID::C::Opt::Val  GpReflectDiffProp_VecUUID::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecUUID{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
