#pragma once

#include "../GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectPropBuilder.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectModelBuilder
{
private:
                                        GpReflectModelBuilder   (void) noexcept;

public:
                                        ~GpReflectModelBuilder  (void) noexcept;

    static GpReflectModelBuilder        SNew                    (void) noexcept {return GpReflectModelBuilder();}

    GpReflectModel                      Build                   (void);

    inline GpReflectModelBuilder&       UID                     (const GpUUID&  aUid);
    inline GpReflectModelBuilder&       BaseUID                 (const GpUUID&  aBaseUid);
    inline GpReflectModelBuilder&       Name                    (std::string_view aName);
    inline GpReflectPropBuilder&        Props                   (void);
    inline GpReflectModelBuilder&       GroupID                 (const GpUUID&  aGroupId);

private:
    GpUUID                              iUID;
    GpUUID                              iBaseUID;
    std::string                         iName;
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

GpReflectModelBuilder&  GpReflectModelBuilder::Name (std::string_view aName)
{
    iName = aName;
    return *this;
}

GpReflectPropBuilder&   GpReflectModelBuilder::Props (void)
{
    if (iPropBuilder.has_value() == false)
    {
        iPropBuilder = GpReflectPropBuilder::SNew(*this);
    }

    return iPropBuilder.value();
}

GpReflectModelBuilder&  GpReflectModelBuilder::GroupID (const GpUUID& aGroupId)
{
    iGroupID = aGroupId;
    return *this;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_REFLECTION)
