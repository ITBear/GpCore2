#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "../GpReflection_global.hpp"
#include "GpReflectDiffArray.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffProp_VecUI8
{
public:
    CLASS_DD(GpReflectDiffProp_VecUI8)

    using ContainerT    = std::vector<u_int_8>;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                            GpReflectDiffProp_VecUI8    (void) noexcept = default;
    inline                  GpReflectDiffProp_VecUI8    (const GpReflectDiffProp_VecUI8& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecUI8    (GpReflectDiffProp_VecUI8&& aDiffProp) noexcept;
    inline                  GpReflectDiffProp_VecUI8    (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                            ~GpReflectDiffProp_VecUI8   (void) noexcept;

    static C::Opt::Val      SCheckAndMake               (const ContainerT& aValueFrom,
                                                         const ContainerT& aValueTo);
private:
    ArrayT::ReplaceVariantT iReplaceData;
};

GpReflectDiffProp_VecUI8::GpReflectDiffProp_VecUI8 (const GpReflectDiffProp_VecUI8& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffProp_VecUI8::GpReflectDiffProp_VecUI8 (GpReflectDiffProp_VecUI8&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffProp_VecUI8::GpReflectDiffProp_VecUI8 (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
