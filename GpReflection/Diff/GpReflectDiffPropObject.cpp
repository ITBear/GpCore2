#include <GpCore2/GpReflection/Diff/GpReflectDiffPropObject.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffDelta.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffUtils_CalcDiff.hpp>

namespace GPlatform {

GpReflectDiffPropObject::GpReflectDiffPropObject (const GpReflectDiffPropObject& aDiffProp):
iValue{aDiffProp.iValue}
{
}

GpReflectDiffPropObject::GpReflectDiffPropObject (GpReflectDiffPropObject&& aDiffProp) noexcept:
iValue{std::move(aDiffProp.iValue)}
{
}

GpReflectDiffPropObject::GpReflectDiffPropObject (GpSP<GpReflectDiffDelta> aValue) noexcept:
iValue{std::move(aValue)}
{
}

GpReflectDiffPropObject::~GpReflectDiffPropObject (void) noexcept
{
}

GpReflectDiffPropObject::C::Opt::Val    GpReflectDiffPropObject::SCheckAndMake
(
    const GpReflectObject&          aValueFrom,
    const GpReflectObject&          aValueTo,
    IsEqualCacheRefOptT&            aIsEqualCache,
    TotalMemoryUseCacheRefOptT&     aTotalMemoryUseCache
)
{
    GpReflectDiffDelta::SP  diffDeltaSP = GpReflectDiffUtils_CalcDiff::SDo(aValueFrom, aValueTo, aIsEqualCache, aTotalMemoryUseCache);
    GpReflectDiffDelta&     diffDelta   = diffDeltaSP.V();

    if (diffDelta.Empty())
    {
        return std::nullopt;
    }

    return GpReflectDiffPropObject{std::move(diffDeltaSP)};
}

}// namespace GPlatform
