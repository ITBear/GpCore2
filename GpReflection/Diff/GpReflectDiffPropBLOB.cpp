#include <GpCore2/GpReflection/Diff/GpReflectDiffPropBLOB.hpp>

namespace GPlatform {

GpReflectDiffPropBLOB::~GpReflectDiffPropBLOB (void) noexcept
{
}

GpReflectDiffPropBLOB::C::Opts::Val GpReflectDiffPropBLOB::SCheckAndMake
(
    const GpByteArray& aValueFrom,
    const GpByteArray& aValueTo
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
