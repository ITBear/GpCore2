#include <GpCore2/GpReflection/Diff/GpReflectDiffUtils.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffDelta.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffUtils_CalcDiff.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>

namespace GPlatform {

GpReflectDiffDelta::SP  GpReflectDiffUtils::SCalcDiff
(
    const GpReflectObject&      aObjFrom,
    const GpReflectObject&      aObjTo,
    IsEqualCacheRefOptT         aIsEqualCache,
    TotalMemoryUseCacheRefOptT  aTotalMemoryUseCache
)
{
    return GpReflectDiffUtils_CalcDiff::SDo
    (
        aObjFrom,
        aObjTo,
        aIsEqualCache,
        aTotalMemoryUseCache
    );
}

void    GpReflectDiffUtils::SApplyDiffTo
(
    [[maybe_unused]] GpReflectObject&           aObjToApply,
    [[maybe_unused]] const GpReflectDiffDelta&  aDiffDelta
)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

GpReflectObject::SP GpReflectDiffUtils::SCreateNewAndApplyDiff
(
    [[maybe_unused]] const GpReflectObject&     aObjSrc,
    [[maybe_unused]] const GpReflectDiffDelta&  aDiffDelta
)
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

}// namespace GPlatform
