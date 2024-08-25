#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "../GpReflection_global.hpp"
#include "GpReflectDiffArray.hpp"

#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectDiffPropString
{
public:
    CLASS_DD(GpReflectDiffPropString)

    using ContainerT    = std::string;
    using ArrayT        = GpReflectDiffArray<ContainerT, typename ContainerT::value_type, void>;

public:
                                                GpReflectDiffPropString     (void) noexcept = default;
    inline                                      GpReflectDiffPropString     (const GpReflectDiffPropString& aDiffProp) noexcept;
    inline                                      GpReflectDiffPropString     (GpReflectDiffPropString&& aDiffProp) noexcept;
    inline                                      GpReflectDiffPropString     (ArrayT::ReplaceVariantT&& aReplaceData) noexcept;
                                                ~GpReflectDiffPropString    (void) noexcept;

    const ArrayT::ReplaceVariantT&              Value                       (void) const noexcept {return iReplaceData;}
    ArrayT::ReplaceVariantT&                    Value                       (void) noexcept {return iReplaceData;}

    static GpReflectDiffPropString::C::Opt::Val SCheckAndMake               (const std::string& aValueFrom,
                                                                             const std::string& aValueTo);
private:
    ArrayT::ReplaceVariantT                     iReplaceData;
};

GpReflectDiffPropString::GpReflectDiffPropString (const GpReflectDiffPropString& aDiffProp) noexcept:
iReplaceData(aDiffProp.iReplaceData)
{
}

GpReflectDiffPropString::GpReflectDiffPropString (GpReflectDiffPropString&& aDiffProp) noexcept:
iReplaceData(std::move(aDiffProp.iReplaceData))
{
}

GpReflectDiffPropString::GpReflectDiffPropString (ArrayT::ReplaceVariantT&& aReplaceData) noexcept:
iReplaceData(std::move(aReplaceData))
{
}

}// namespace GPlatform
