#include "GpReflectDiffPropString.hpp"

namespace GPlatform {

GpReflectDiffPropString::~GpReflectDiffPropString (void) noexcept
{   
}

GpReflectDiffPropString::C::Opt::Val    GpReflectDiffPropString::SCheckAndMake
(
    const std::string& aValueFrom,
    const std::string& aValueTo
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake(aValueFrom, aValueTo);

    if (replaceData.has_value())
    {
        GpReflectDiffPropString{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
