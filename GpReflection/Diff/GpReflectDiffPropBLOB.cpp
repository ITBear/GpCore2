#include "GpReflectDiffPropBLOB.hpp"

namespace GPlatform {

GpReflectDiffPropBLOB::~GpReflectDiffPropBLOB (void) noexcept
{
}

GpReflectDiffPropBLOB::C::Opt::Val  GpReflectDiffPropBLOB::SCheckAndMake
(
    const GpBytesArray& aValueFrom,
    const GpBytesArray& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffPropBLOB{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
