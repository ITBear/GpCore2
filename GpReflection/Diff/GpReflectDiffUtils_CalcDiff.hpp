#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>

#include "../GpReflectUtils_IsEqual.hpp"
#include "../GpReflectUtils_TotalMemoryUse.hpp"

namespace GPlatform {

class GpReflectDiffDelta;

class GpReflectDiffUtils_CalcDiff
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectDiffUtils_CalcDiff)

    using IsEqualCacheRefOptT           = GpReflectUtils_IsEqual::CacheRefOptT;
    using TotalMemoryUseCacheRefOptT    = GpReflectUtils_TotalMemoryUse::CacheRefOptT;

public:
    static GpSP<GpReflectDiffDelta> SDo (const GpReflectObject&         aObjFrom,
                                         const GpReflectObject&         aObjTo,
                                         IsEqualCacheRefOptT&           aIsEqualCache,
                                         TotalMemoryUseCacheRefOptT&    aTotalMemoryUseCache);
};

}// namespace GPlatform
