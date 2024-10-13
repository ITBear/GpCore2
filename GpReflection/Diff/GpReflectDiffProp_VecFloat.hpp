#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffArray.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecFloat
{
public:
    CLASS_DD(GpReflectDiffProp_VecFloat)

    using ContainerT    = std::vector<float>;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                            GpReflectDiffProp_VecFloat  (void) noexcept = default;
    inline                  GpReflectDiffProp_VecFloat  (const GpReflectDiffProp_VecFloat& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecFloat  (GpReflectDiffProp_VecFloat&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecFloat  (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecFloat (void) noexcept;

    static C::Opt::Val      SCheckAndMake               (const ContainerT& aValueFrom,
                                                         const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecFloat::GpReflectDiffProp_VecFloat (const GpReflectDiffProp_VecFloat& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecFloat::GpReflectDiffProp_VecFloat (GpReflectDiffProp_VecFloat&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecFloat::GpReflectDiffProp_VecFloat (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
