#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffArray.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecDouble
{
public:
    CLASS_DD(GpReflectDiffProp_VecDouble)

    using ContainerT    = std::vector<double>;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                            GpReflectDiffProp_VecDouble     (void) noexcept = default;
    inline                  GpReflectDiffProp_VecDouble     (const GpReflectDiffProp_VecDouble& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecDouble     (GpReflectDiffProp_VecDouble&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecDouble     (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecDouble    (void) noexcept;

    static C::Opt::Val      SCheckAndMake                   (const ContainerT& aValueFrom,
                                                             const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecDouble::GpReflectDiffProp_VecDouble (const GpReflectDiffProp_VecDouble& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecDouble::GpReflectDiffProp_VecDouble (GpReflectDiffProp_VecDouble&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecDouble::GpReflectDiffProp_VecDouble (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
