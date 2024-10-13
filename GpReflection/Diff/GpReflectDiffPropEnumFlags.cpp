#include <GpCore2/GpReflection/Diff/GpReflectDiffPropEnumFlags.hpp>

namespace GPlatform {

GpReflectDiffPropEnumFlags::~GpReflectDiffPropEnumFlags (void) noexcept
{
}

GpReflectDiffPropEnumFlags::C::Opt::Val GpReflectDiffPropEnumFlags::SCheckAndMake
(
    const GpEnumFlags& aValueFrom,
    const GpEnumFlags& aValueTo
)
{
    if (aValueFrom.Value() != aValueTo.Value())
    {
        return GpReflectDiffPropEnumFlags{aValueTo.Value()};
    }

    return std::nullopt;
}

}// namespace GPlatform
