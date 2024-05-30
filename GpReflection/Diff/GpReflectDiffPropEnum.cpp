#include "GpReflectDiffPropEnum.hpp"

namespace GPlatform {

GpReflectDiffPropEnum::~GpReflectDiffPropEnum (void) noexcept
{
}

GpReflectDiffPropEnum::C::Opt::Val  GpReflectDiffPropEnum::SCheckAndMake
(
    const GpEnum& aValueFrom,
    const GpEnum& aValueTo
)
{
    if (aValueFrom != aValueTo)
    {
        return GpReflectDiffPropEnum{aValueTo.ID()};
    }

    return std::nullopt;
}

}// namespace GPlatform
