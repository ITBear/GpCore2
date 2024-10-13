#include <GpCore2/GpReflection/Diff/GpReflectDiffProp_VecObject.hpp>

namespace GPlatform {

GpReflectDiffProp_VecObject::~GpReflectDiffProp_VecObject (void) noexcept
{
}

GpReflectDiffProp_VecObject::C::Opt::Val    GpReflectDiffProp_VecObject::SCheckAndMake
(
    const ContainerT&           aValueFrom,
    const ContainerT&           aValueTo,
    IsEqualCacheRefOptT&        aIsEqualCache,
    TotalMemoryUseCacheRefOptT& aTotalMemoryUseCache
)
{
    const size_t strideFrom = aValueFrom.stride();
    const size_t strideTo   = aValueTo.stride();
    std::optional<ArrayT::ReplaceVariantT> replaceData = ArrayT::SCheckAndMake
    (
        aValueFrom,
        aValueTo,
        aIsEqualCache,
        aTotalMemoryUseCache,
        strideFrom,
        strideTo
    );

    if (replaceData.has_value())
    {
        GpReflectDiffProp_VecObject{std::move(replaceData.value())};
    }

    return std::nullopt;
}

}// namespace GPlatform
