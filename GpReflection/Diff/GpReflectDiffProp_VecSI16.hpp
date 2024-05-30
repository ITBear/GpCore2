#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "../GpReflection_global.hpp"
#include "GpReflectDiffArray.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecSI16
{
public:
    CLASS_DD(GpReflectDiffProp_VecSI16)

    using ContainerT    = std::vector<s_int_16>;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                            GpReflectDiffProp_VecSI16   (void) noexcept = default;
    inline                  GpReflectDiffProp_VecSI16   (const GpReflectDiffProp_VecSI16& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecSI16   (GpReflectDiffProp_VecSI16&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecSI16   (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecSI16  (void) noexcept;

    static C::Opt::Val      SCheckAndMake               (const ContainerT& aValueFrom,
                                                         const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecSI16::GpReflectDiffProp_VecSI16 (const GpReflectDiffProp_VecSI16& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecSI16::GpReflectDiffProp_VecSI16 (GpReflectDiffProp_VecSI16&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecSI16::GpReflectDiffProp_VecSI16 (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
