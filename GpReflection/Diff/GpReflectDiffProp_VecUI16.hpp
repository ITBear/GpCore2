#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffArray.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecUI16
{
public:
    CLASS_DD(GpReflectDiffProp_VecUI16)

    using ContainerT    = std::vector<u_int_16>;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                            GpReflectDiffProp_VecUI16   (void) noexcept = default;
    inline                  GpReflectDiffProp_VecUI16   (const GpReflectDiffProp_VecUI16& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecUI16   (GpReflectDiffProp_VecUI16&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecUI16   (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecUI16  (void) noexcept;

    static C::Opt::Val      SCheckAndMake               (const ContainerT& aValueFrom,
                                                         const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecUI16::GpReflectDiffProp_VecUI16 (const GpReflectDiffProp_VecUI16& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecUI16::GpReflectDiffProp_VecUI16 (GpReflectDiffProp_VecUI16&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecUI16::GpReflectDiffProp_VecUI16 (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
