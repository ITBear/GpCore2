#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "../GpReflectObject.hpp"
#include "../GpReflectUtils_IsEqual.hpp"
#include "../GpReflectUtils_TotalMemoryUse.hpp"

namespace GPlatform {

class GpReflectDiffDelta;

class GP_REFLECTION_API GpReflectDiffPropObject
{
public:
    CLASS_DD(GpReflectDiffPropObject)

    using IsEqualCacheRefOptT           = GpReflectUtils_IsEqual::CacheRefOptT;
    using TotalMemoryUseCacheRefOptT    = GpReflectUtils_TotalMemoryUse::CacheRefOptT;

public:
                                                GpReflectDiffPropObject     (void) noexcept = default;
                                                GpReflectDiffPropObject     (const GpReflectDiffPropObject& aDiffProp);
                                                GpReflectDiffPropObject     (GpReflectDiffPropObject&& aDiffProp) noexcept;
                                                GpReflectDiffPropObject     (GpSP<GpReflectDiffDelta> aValue) noexcept;
                                                ~GpReflectDiffPropObject    (void) noexcept;

    const GpSP<GpReflectDiffDelta>&             Value                       (void) const noexcept {return iValue;}
    GpSP<GpReflectDiffDelta>&                   Value                       (void) noexcept {return iValue;}

    static GpReflectDiffPropObject::C::Opt::Val SCheckAndMake               (const GpReflectObject&         aValueFrom,
                                                                             const GpReflectObject&         aValueTo,
                                                                             IsEqualCacheRefOptT&           aIsEqualCache,
                                                                             TotalMemoryUseCacheRefOptT&    aTotalMemoryUseCache);

private:
    GpSP<GpReflectDiffDelta>                    iValue;
};

}// namespace GPlatform
