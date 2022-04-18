#include "GpTypeStructDynamicBuilder.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructDynamic.hpp"

namespace GPlatform {

GpTypeStructDynamicBuilder::GpTypeStructDynamicBuilder (void) noexcept
{
}

GpTypeStructDynamicBuilder::~GpTypeStructDynamicBuilder (void) noexcept
{
}

GpTypeStructInfo::SP    GpTypeStructDynamicBuilder::SBuild (const GpTypeStructDynamicInfoDesc& aDesc)
{
    return SNew()
        .UID(aDesc.uid)
        .BaseUID(aDesc.base_uid)
        .GroupID(aDesc.group_id)
        .Name(aDesc.name)
        .Props()
            .AddProps(aDesc.props)
        .DoneBuildProps()
    .Build();
}

GpTypeStructInfo::SP    GpTypeStructDynamicBuilder::Build (void)
{
    GpTypeStructInfo::SP typeStructInfoSP = MakeSP<GpTypeStructInfo>
    (
        iUID,
        iBaseUID,
        std::move(iName),
        iPropInfoBuilder.MoveProps(),
        iGroupID,
        GpSP<GpTypeStructDynamic::Factory>::SNew(),
        alignof(GpTypeStructDynamic),
        sizeof(GpTypeStructDynamic)
    );

    return typeStructInfoSP;
}

GpTypeStructDynamicBuilder& GpTypeStructDynamicBuilder::UID (const GpUUID& aUid)
{
    iUID = aUid;
    return *this;
}

GpTypeStructDynamicBuilder& GpTypeStructDynamicBuilder::BaseUID (const GpUUID& aBaseUid)
{
    iBaseUID = aBaseUid;
    return *this;
}

GpTypeStructDynamicBuilder& GpTypeStructDynamicBuilder::Name (std::string_view aName)
{
    iName = aName;
    return *this;
}

GpTypePropInfoBuilder&  GpTypeStructDynamicBuilder::Props (void)
{
    iPropInfoBuilder = GpTypePropInfoBuilder::SNew(*this);
    return iPropInfoBuilder;
}

GpTypeStructDynamicBuilder& GpTypeStructDynamicBuilder::GroupID (const GpUUID& aGroupId)
{
    iGroupID = aGroupId;
    return *this;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
