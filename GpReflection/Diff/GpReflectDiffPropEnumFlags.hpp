#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnumFlags.hpp>
#include <GpCore2/GpReflection/GpReflection_global.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffPropEnumFlags
{
public:
    CLASS_DD(GpReflectDiffPropEnumFlags)

public:
                                                    GpReflectDiffPropEnumFlags  (void) noexcept = default;
    inline                                          GpReflectDiffPropEnumFlags  (const GpReflectDiffPropEnumFlags& aDiffProp) noexcept;
    inline                                          GpReflectDiffPropEnumFlags  (GpReflectDiffPropEnumFlags&& aDiffProp) noexcept;
    inline                                          GpReflectDiffPropEnumFlags  (GpEnumFlags::value_type aValue) noexcept;
                                                    ~GpReflectDiffPropEnumFlags (void) noexcept;

    static GpReflectDiffPropEnumFlags::C::Opt::Val  SCheckAndMake               (const GpEnumFlags& aValueFrom,
                                                                                 const GpEnumFlags& aValueTo);

    GpEnumFlags::value_type                         Value                       (void) const noexcept {return iValue;}

private:
    GpEnumFlags::value_type                         iValue  = {};
};

GpReflectDiffPropEnumFlags::GpReflectDiffPropEnumFlags (const GpReflectDiffPropEnumFlags& aDiffProp) noexcept:
iValue(aDiffProp.iValue)
{
}

GpReflectDiffPropEnumFlags::GpReflectDiffPropEnumFlags (GpReflectDiffPropEnumFlags&& aDiffProp) noexcept:
iValue(std::move(aDiffProp.iValue))
{
}

GpReflectDiffPropEnumFlags::GpReflectDiffPropEnumFlags (GpEnumFlags::value_type aValue) noexcept:
iValue(aValue)
{
}

}// namespace GPlatform
