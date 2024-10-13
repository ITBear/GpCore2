#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffPropEnum
{
public:
    CLASS_DD(GpReflectDiffPropEnum)

public:
                                                GpReflectDiffPropEnum   (void) noexcept = default;
    inline                                      GpReflectDiffPropEnum   (const GpReflectDiffPropEnum& aDiffProp) noexcept;
    inline                                      GpReflectDiffPropEnum   (GpReflectDiffPropEnum&& aDiffProp) noexcept;
    inline                                      GpReflectDiffPropEnum   (GpEnum::value_type aValue) noexcept;
                                                ~GpReflectDiffPropEnum  (void) noexcept;

    static GpReflectDiffPropEnum::C::Opt::Val   SCheckAndMake           (const GpEnum&  aValueFrom,
                                                                         const GpEnum&  aValueTo);

    GpEnum::value_type                          Value                   (void) const noexcept {return iValue;}

private:
    GpEnum::value_type                          iValue  = {};
};

GpReflectDiffPropEnum::GpReflectDiffPropEnum (const GpReflectDiffPropEnum& aDiffProp) noexcept:
iValue(aDiffProp.iValue)
{
}

GpReflectDiffPropEnum::GpReflectDiffPropEnum (GpReflectDiffPropEnum&& aDiffProp) noexcept:
iValue(std::move(aDiffProp.iValue))
{
}

GpReflectDiffPropEnum::GpReflectDiffPropEnum (GpEnum::value_type aValue) noexcept:
iValue(aValue)
{
}

}// namespace GPlatform
