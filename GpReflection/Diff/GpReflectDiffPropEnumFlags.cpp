#include <GpCore2/GpReflection/Diff/GpReflectDiffPropEnumFlags.hpp>

namespace GPlatform {

GpReflectDiffPropEnumFlags::~GpReflectDiffPropEnumFlags (void) noexcept
{
}

GpReflectDiffPropEnumFlags::C::Opts::Val    GpReflectDiffPropEnumFlags::SCheckAndMake
(
    const GpEnumFlags& aValueFrom,
    const GpEnumFlags& aValueTo
)
{
    if (aValueFrom.RawValue() != aValueTo.RawValue())
    {
        return GpReflectDiffPropEnumFlags{aValueTo.RawValue()};
    }

    return std::nullopt;
}

}// namespace GPlatform
