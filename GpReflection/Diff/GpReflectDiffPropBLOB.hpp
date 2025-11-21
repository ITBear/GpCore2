#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpByteArray.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffArray.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffPropBLOB
{
public:
    CLASS_DD(GpReflectDiffPropBLOB)

    using ContainerT    = GpByteArray;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                                                GpReflectDiffPropBLOB   (void) noexcept = default;
    inline                                      GpReflectDiffPropBLOB   (const GpReflectDiffPropBLOB& aDiffProp) noexcept;
    inline                                      GpReflectDiffPropBLOB   (GpReflectDiffPropBLOB&& aDiffProp) noexcept;
    inline                                      GpReflectDiffPropBLOB   (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                                                ~GpReflectDiffPropBLOB  (void) noexcept;

    static GpReflectDiffPropBLOB::C::Opts::Val  SCheckAndMake           (const GpByteArray& aValueFrom,
                                                                         const GpByteArray& aValueTo);
private:
    ArrayT::ReplaceVariantT                     iReplaceData;
};

GpReflectDiffPropBLOB::GpReflectDiffPropBLOB (const GpReflectDiffPropBLOB& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffPropBLOB::GpReflectDiffPropBLOB (GpReflectDiffPropBLOB&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffPropBLOB::GpReflectDiffPropBLOB (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
