#include "GpReflectDiffProp_VecBLOB.hpp"

namespace GPlatform {

GpReflectDiffProp_VecBLOB::~GpReflectDiffProp_VecBLOB (void) noexcept
{
}

GpReflectDiffProp_VecBLOB::C::Opt::Val  GpReflectDiffProp_VecBLOB::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecBLOB{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
