#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "../GpReflection_global.hpp"
#include "GpReflectDiffArray.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecSI64
{
public:
    CLASS_DD(GpReflectDiffProp_VecSI64)

    using ContainerT    = std::vector<s_int_64>;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                            GpReflectDiffProp_VecSI64   (void) noexcept = default;
    inline                  GpReflectDiffProp_VecSI64   (const GpReflectDiffProp_VecSI64& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecSI64   (GpReflectDiffProp_VecSI64&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecSI64   (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecSI64  (void) noexcept;

    static C::Opt::Val      SCheckAndMake               (const ContainerT& aValueFrom,
                                                         const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecSI64::GpReflectDiffProp_VecSI64 (const GpReflectDiffProp_VecSI64& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecSI64::GpReflectDiffProp_VecSI64 (GpReflectDiffProp_VecSI64&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecSI64::GpReflectDiffProp_VecSI64 (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
