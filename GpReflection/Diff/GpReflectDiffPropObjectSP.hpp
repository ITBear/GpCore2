#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectUtils_IsEqual.hpp>
#include <GpCore2/GpReflection/GpReflectUtils_TotalMemoryUse.hpp>

namespace GPlatform {

class GpReflectDiffDelta;

class GP_REFLECTION_API GpReflectDiffPropObjectSP
{
public:
    CLASS_DD(GpReflectDiffPropObjectSP)

    using IsEqualCacheRefOptT           = GpReflectUtils_IsEqual::CacheRefOptT;
    using TotalMemoryUseCacheRefOptT    = GpReflectUtils_TotalMemoryUse::CacheRefOptT;

public:
                                                    GpReflectDiffPropObjectSP   (void) noexcept = default;
                                                    GpReflectDiffPropObjectSP   (const GpReflectDiffPropObjectSP& aDiffProp);
                                                    GpReflectDiffPropObjectSP   (GpReflectDiffPropObjectSP&& aDiffProp) noexcept;
                                                    GpReflectDiffPropObjectSP   (GpSP<GpReflectDiffDelta> aValue) noexcept;
                                                    ~GpReflectDiffPropObjectSP  (void) noexcept;

    const GpSP<GpReflectDiffDelta>&                 Value                       (void) const noexcept {return iValue;}
    GpSP<GpReflectDiffDelta>&                       Value                       (void) noexcept {return iValue;}

    static GpReflectDiffPropObjectSP::C::Opt::Val   SCheckAndMake               (const GpReflectObject::SP&     aValueFrom,
                                                                                 const GpReflectObject::SP&     aValueTo,
                                                                                 IsEqualCacheRefOptT&           aIsEqualCache,
                                                                                 TotalMemoryUseCacheRefOptT&    aTotalMemoryUseCache);

private:
    GpSP<GpReflectDiffDelta>                        iValue;
};

}// namespace GPlatform
