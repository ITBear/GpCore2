#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectPropBuilder.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectModelBuilder
{
public:
                                        GpReflectModelBuilder   (void) noexcept;
                                        ~GpReflectModelBuilder  (void) noexcept;

    GpReflectModel                      Build                   (void);

    inline GpReflectModelBuilder&       UID                     (const GpUUID&  aUid);
    inline GpReflectModelBuilder&       BaseUID                 (const GpUUID&  aBaseUid);
    inline GpReflectModelBuilder&       Name                    (std::u8string_view aName);
    inline GpReflectPropBuilder&        Props                   (void);
    inline GpReflectModelBuilder&       SetPropBuilder          (GpReflectPropBuilder&& aPropBuilder);
    inline GpReflectModelBuilder&       GroupID                 (const GpUUID&  aGroupId);

private:
    GpUUID                              iUID;
    GpUUID                              iBaseUID;
    std::u8string                       iName;
    GpUUID                              iGroupID;
    std::optional<GpReflectPropBuilder> iPropBuilder;
};

GpReflectModelBuilder&  GpReflectModelBuilder::UID (const GpUUID& aUid)
{
    iUID = aUid;
    return *this;
}

GpReflectModelBuilder&  GpReflectModelBuilder::BaseUID (const GpUUID& aBaseUid)
{
    iBaseUID = aBaseUid;
    return *this;
}

GpReflectModelBuilder&  GpReflectModelBuilder::Name (std::u8string_view aName)
{
    iName = aName;
    return *this;
}

GpReflectPropBuilder&   GpReflectModelBuilder::Props (void)
{
    if (iPropBuilder.has_value() == false)
    {
        iPropBuilder = GpReflectPropBuilder(*this);
    }

    return iPropBuilder.value();
}

GpReflectModelBuilder&  GpReflectModelBuilder::SetPropBuilder (GpReflectPropBuilder&& aPropBuilder)
{
    iPropBuilder = std::move(aPropBuilder);
    iPropBuilder->SetModelBuilder(this);
    iPropBuilder->DoneBuildProps();

    return *this;
}

GpReflectModelBuilder&  GpReflectModelBuilder::GroupID (const GpUUID& aGroupId)
{
    iGroupID = aGroupId;
    return *this;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_REFLECTION)
