#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "../UIDs/GpUUID.hpp"
#include "../Classes/GpClassesDefines.hpp"
#include "../Containers/GpContainersT.hpp"
#include "GpType.hpp"
#include "GpTypeContainer.hpp"

namespace GPlatform {

class GPCORE_API GpTypePropInfo
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypePropInfo)

    using TypeT         = GpType;
    using TypeTE        = TypeT::EnumT;
    using ContainerT    = GpTypeContainer;
    using ContainerTE   = ContainerT::EnumT;

public:
                            GpTypePropInfo      (void) noexcept = default;
    inline                  GpTypePropInfo      (const GpTypePropInfo& aPropInfo);
    inline                  GpTypePropInfo      (GpTypePropInfo&& aPropInfo) noexcept;
                            ~GpTypePropInfo     (void) noexcept = default;

    inline GpTypePropInfo&  operator=           (const GpTypePropInfo& aPropInfo);
    inline GpTypePropInfo&  operator=           (GpTypePropInfo&& aPropInfo) noexcept;

    const GpUUID&           TypeUID             (void) const noexcept {return iTypeUID;}
    ContainerTE             Container           (void) const noexcept {return iContainer;}
    TypeTE                  ContainerKeyType    (void) const noexcept {return iContainerKeyType;}
    std::string_view        Name                (void) const noexcept {return iName;}
    size_t                  Align               (void) const noexcept {return iAlign;}
    size_t                  Size                (void) const noexcept {return iSize;}
    std::ptrdiff_t          Offset              (void) const noexcept {return iOffset;}

private:
    GpUUID                  iTypeUID;
    ContainerTE             iContainer;
    TypeTE                  iContainerKeyType;
    std::string             iName;
    size_t                  iAlign      = 0;
    size_t                  iSize       = 0;
    std::ptrdiff_t          iOffset     = 0;
};

GpTypePropInfo::GpTypePropInfo (const GpTypePropInfo& aPropInfo):
iTypeUID(aPropInfo.iTypeUID),
iContainer(aPropInfo.iContainer),
iContainerKeyType(aPropInfo.iContainerKeyType),
iName(aPropInfo.iName),
iAlign(aPropInfo.iAlign),
iSize(aPropInfo.iSize),
iOffset(aPropInfo.iOffset)
{
}

GpTypePropInfo::GpTypePropInfo (GpTypePropInfo&& aPropInfo) noexcept:
iTypeUID(std::move(aPropInfo.iTypeUID)),
iContainer(std::move(aPropInfo.iContainer)),
iContainerKeyType(std::move(aPropInfo.iContainerKeyType)),
iName(std::move(aPropInfo.iName)),
iAlign(std::move(aPropInfo.iAlign)),
iSize(std::move(aPropInfo.iSize)),
iOffset(std::move(aPropInfo.iOffset))
{
}

GpTypePropInfo& GpTypePropInfo::operator= (const GpTypePropInfo& aPropInfo)
{
    iTypeUID            = aPropInfo.iTypeUID;
    iContainer          = aPropInfo.iContainer;
    iContainerKeyType   = aPropInfo.iContainerKeyType;
    iName               = aPropInfo.iName;
    iAlign              = aPropInfo.iAlign;
    iSize               = aPropInfo.iSize;
    iOffset             = aPropInfo.iOffset;

    return *this;
}

GpTypePropInfo& GpTypePropInfo::operator= (GpTypePropInfo&& aPropInfo) noexcept
{
    iTypeUID            = std::move(aPropInfo.iTypeUID);
    iContainer          = std::move(aPropInfo.iContainer);
    iContainerKeyType   = std::move(aPropInfo.iContainerKeyType);
    iName               = std::move(aPropInfo.iName);
    iAlign              = std::move(aPropInfo.iAlign);
    iSize               = std::move(aPropInfo.iSize);
    iOffset             = std::move(aPropInfo.iOffset);

    return *this;
}

}//GPlatform

#endif//GP_USE_REFLECTION
