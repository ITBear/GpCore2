#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffArray.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecSI8
{
public:
    CLASS_DD(GpReflectDiffProp_VecSI8)

    using ContainerT    = std::vector<s_int_8>;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                            GpReflectDiffProp_VecSI8    (void) noexcept = default;
    inline                  GpReflectDiffProp_VecSI8    (const GpReflectDiffProp_VecSI8& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecSI8    (GpReflectDiffProp_VecSI8&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecSI8    (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecSI8   (void) noexcept;

    static C::Opt::Val      SCheckAndMake               (const ContainerT& aValueFrom,
                                                         const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecSI8::GpReflectDiffProp_VecSI8 (const GpReflectDiffProp_VecSI8& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecSI8::GpReflectDiffProp_VecSI8 (GpReflectDiffProp_VecSI8&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecSI8::GpReflectDiffProp_VecSI8 (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
