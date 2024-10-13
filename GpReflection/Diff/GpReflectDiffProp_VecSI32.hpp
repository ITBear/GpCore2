#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffArray.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecSI32
{
public:
    CLASS_DD(GpReflectDiffProp_VecSI32)

    using ContainerT    = std::vector<s_int_32>;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                            GpReflectDiffProp_VecSI32   (void) noexcept = default;
    inline                  GpReflectDiffProp_VecSI32   (const GpReflectDiffProp_VecSI32& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecSI32   (GpReflectDiffProp_VecSI32&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecSI32   (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecSI32  (void) noexcept;

    static C::Opt::Val      SCheckAndMake               (const ContainerT& aValueFrom,
                                                         const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecSI32::GpReflectDiffProp_VecSI32 (const GpReflectDiffProp_VecSI32& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecSI32::GpReflectDiffProp_VecSI32 (GpReflectDiffProp_VecSI32&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecSI32::GpReflectDiffProp_VecSI32 (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
