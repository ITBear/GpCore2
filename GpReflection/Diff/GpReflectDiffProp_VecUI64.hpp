#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "../GpReflection_global.hpp"
#include "GpReflectDiffArray.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecUI64
{
public:
    CLASS_DD(GpReflectDiffProp_VecUI64)

    using ContainerT    = std::vector<u_int_64>;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                            GpReflectDiffProp_VecUI64   (void) noexcept = default;
    inline                  GpReflectDiffProp_VecUI64   (const GpReflectDiffProp_VecUI64& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecUI64   (GpReflectDiffProp_VecUI64&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecUI64   (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecUI64  (void) noexcept;

    static C::Opt::Val      SCheckAndMake               (const ContainerT& aValueFrom,
                                                         const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecUI64::GpReflectDiffProp_VecUI64 (const GpReflectDiffProp_VecUI64& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecUI64::GpReflectDiffProp_VecUI64 (GpReflectDiffProp_VecUI64&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecUI64::GpReflectDiffProp_VecUI64 (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
