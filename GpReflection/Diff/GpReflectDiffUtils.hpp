#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "../GpReflection_global.hpp"
#include "../GpReflectUtils_IsEqual.hpp"
#include "../GpReflectUtils_TotalMemoryUse.hpp"

namespace GPlatform {

class GpReflectDiffDelta;

class GP_REFLECTION_API GpReflectDiffUtils
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectDiffUtils)

    using IsEqualCacheRefOptT           = typename GpReflectUtils_IsEqual::CacheRefOptT;
    using TotalMemoryUseCacheRefOptT    = typename GpReflectUtils_TotalMemoryUse::CacheRefOptT;

public:
    static GpSP<GpReflectDiffDelta> SCalcDiff               (const GpReflectObject&     aObjFrom,
                                                             const GpReflectObject&     aObjTo,
                                                             IsEqualCacheRefOptT        aIsEqualCache,
                                                             TotalMemoryUseCacheRefOptT aTotalMemoryUseCache);

    static void                     SApplyDiffTo            (GpReflectObject&           aObjToApply,
                                                             const GpReflectDiffDelta&  aDiffDelta);

    static GpReflectObject::SP      SCreateNewAndApplyDiff  (const GpReflectObject&     aObjSrc,
                                                             const GpReflectDiffDelta&  aDiffDelta);
};

}// namespace GPlatform
