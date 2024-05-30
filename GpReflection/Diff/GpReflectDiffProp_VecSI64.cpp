#include "GpReflectDiffProp_VecSI64.hpp"

namespace GPlatform {

GpReflectDiffProp_VecSI64::~GpReflectDiffProp_VecSI64 (void) noexcept
{
}

GpReflectDiffProp_VecSI64::C::Opt::Val  GpReflectDiffProp_VecSI64::SCheckAndMake
(
    const ContainerT& aValueFrom,
    const ContainerT& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecSI64{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
