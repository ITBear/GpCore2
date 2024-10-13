#include <GpCore2/GpReflection/Diff/GpReflectDiffPropObjectSP.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffDelta.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffUtils_CalcDiff.hpp>

namespace GPlatform {

GpReflectDiffPropObjectSP::GpReflectDiffPropObjectSP (const GpReflectDiffPropObjectSP& aDiffProp):
iValue{aDiffProp.iValue}
{
}

GpReflectDiffPropObjectSP::GpReflectDiffPropObjectSP (GpReflectDiffPropObjectSP&& aDiffProp) noexcept:
iValue{std::move(aDiffProp.iValue)}
{
}

GpReflectDiffPropObjectSP::GpReflectDiffPropObjectSP (GpSP<GpReflectDiffDelta> aValue) noexcept:
iValue{std::move(aValue)}
{
}

GpReflectDiffPropObjectSP::~GpReflectDiffPropObjectSP (void) noexcept
{
}

GpReflectDiffPropObjectSP::C::Opt::Val  GpReflectDiffPropObjectSP::SCheckAndMake
(
    const GpReflectObject::SP&  aValueFrom,
    const GpReflectObject::SP&  aValueTo,
    IsEqualCacheRefOptT&        aIsEqualCache,
    TotalMemoryUseCacheRefOptT& aTotalMemoryUseCache
)
{
    if (aValueTo.IsNULL())
    {
        return GpReflectDiffPropObjectSP{GpReflectDiffDelta::SP::SNull()};
    }

    GpReflectObject::SP valueFrom = aValueFrom;

    if (aValueFrom.IsNULL())
    {
        valueFrom = aValueTo.Vn().ReflectNewInstance();
    }

    GpReflectDiffDelta::SP  diffDeltaSP = GpReflectDiffUtils_CalcDiff::SDo(valueFrom.Vn(), aValueTo.Vn(), aIsEqualCache, aTotalMemoryUseCache);
    GpReflectDiffDelta&     diffDelta   = diffDeltaSP.V();

    if (diffDelta.Empty())
    {
        return std::nullopt;
    }

    return GpReflectDiffPropObjectSP{std::move(diffDeltaSP)};
}

}// namespace GPlatform
