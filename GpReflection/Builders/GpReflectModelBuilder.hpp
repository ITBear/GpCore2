#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "GpReflectModelPropBuilder.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectModelBuilder
{
public:
                                        GpReflectModelBuilder   (void) noexcept;
                                        ~GpReflectModelBuilder  (void) noexcept;

    GpReflectModel::CSP                 Build                   (void);

    inline GpReflectModelBuilder&       UID                     (const GpUUID&  aUid);
    inline GpReflectModelBuilder&       BaseUID                 (const GpUUID&  aBaseUid);
    inline GpReflectModelBuilder&       Name                    (std::string_view aName);
    inline GpReflectModelPropBuilder&   Props                   (void);
    inline GpReflectModelBuilder&       SetPropBuilder          (GpReflectModelPropBuilder&& aPropBuilder);
    inline GpReflectModelBuilder&       GroupID                 (const GpUUID&  aGroupId);

private:
    GpUUID                                      iUID;
    GpUUID                                      iBaseUID;
    std::string                                 iName;
    GpUUID                                      iGroupID;
    std::optional<GpReflectModelPropBuilder>    iPropBuilder;
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

GpReflectModelBuilder&  GpReflectModelBuilder::Name (std::string_view aName)
{
    iName = aName;
    return *this;
}

GpReflectModelPropBuilder&  GpReflectModelBuilder::Props (void)
{
    if (iPropBuilder.has_value() == false)
    {
        iPropBuilder = GpReflectModelPropBuilder(*this);
    }

    return iPropBuilder.value();
}

GpReflectModelBuilder&  GpReflectModelBuilder::SetPropBuilder (GpReflectModelPropBuilder&& aPropBuilder)
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

}// namespace GPlatform
