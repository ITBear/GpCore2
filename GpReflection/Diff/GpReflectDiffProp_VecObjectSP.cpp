#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecObjectSP.hpp>

namespace GPlatform {

GpReflectDiffProp_VecObjectSP::~GpReflectDiffProp_VecObjectSP (void) noexcept
{
}

GpReflectDiffProp_VecObjectSP::C::Opt::Val  GpReflectDiffProp_VecObjectSP::SCheckAndMake
(
    const ContainerT&           aValueFrom,
    const ContainerT&           aValueTo,
    IsEqualCacheRefOptT&        aIsEqualCache,
    TotalMemoryUseCacheRefOptT& aTotalMemoryUseCache
)
{
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake
    (
        aValueFrom,
        aValueTo,
        aIsEqualCache,
        aTotalMemoryUseCache
    );

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecObjectSP{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
