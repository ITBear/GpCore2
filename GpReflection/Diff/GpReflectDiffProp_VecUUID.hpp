#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/UIDs/GpUUID.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>
#include <GpCore2/GpReflection/Diff/GpReflectDiffArray.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecUUID
{
public:
    CLASS_DD(GpReflectDiffProp_VecUUID)

    using ContainerT    = std::vector<GpUUID>;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                            GpReflectDiffProp_VecUUID   (void) noexcept = default;
    inline                  GpReflectDiffProp_VecUUID   (const GpReflectDiffProp_VecUUID& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecUUID   (GpReflectDiffProp_VecUUID&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecUUID   (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecUUID  (void) noexcept;

    static C::Opt::Val      SCheckAndMake               (const ContainerT& aValueFrom,
                                                         const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecUUID::GpReflectDiffProp_VecUUID (const GpReflectDiffProp_VecUUID& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecUUID::GpReflectDiffProp_VecUUID (GpReflectDiffProp_VecUUID&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecUUID::GpReflectDiffProp_VecUUID (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
