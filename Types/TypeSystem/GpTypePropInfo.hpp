#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

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
                            GpTypePropInfo      (void) noexcept;
                            GpTypePropInfo      (const TypeTE           aType,
                                                 const GpUUID           aStructTypeUID,
                                                 const ContainerTE      aContainer,
                                                 const TypeTE           aContainerKeyType,
                                                 std::string&&          aName,
                                                 const size_t           aAlign,
                                                 const size_t           aSize,
                                                 const std::ptrdiff_t   aOffset) noexcept;
                            GpTypePropInfo      (const GpTypePropInfo& aPropInfo);
                            GpTypePropInfo      (GpTypePropInfo&& aPropInfo) noexcept;
                            ~GpTypePropInfo     (void) noexcept;

    GpTypePropInfo&         operator=           (const GpTypePropInfo& aPropInfo);
    GpTypePropInfo&         operator=           (GpTypePropInfo&& aPropInfo) noexcept;

    TypeTE                  Type                (void) const noexcept {return iType;}
    const GpUUID&           StructTypeUID       (void) const noexcept {return iStructTypeUID;}
    ContainerTE             Container           (void) const noexcept {return iContainer;}
    TypeTE                  ContainerKeyType    (void) const noexcept {return iContainerKeyType;}
    std::string_view        Name                (void) const noexcept {return iName;}
    size_t                  Align               (void) const noexcept {return iAlign;}
    size_t                  Size                (void) const noexcept {return iSize;}
    std::ptrdiff_t          Offset              (void) const noexcept {return iOffset;}

private:
    TypeTE                  iType;
    GpUUID                  iStructTypeUID;
    ContainerTE             iContainer;
    TypeTE                  iContainerKeyType;
    std::string             iName;
    size_t                  iAlign      = 0;
    size_t                  iSize       = 0;
    std::ptrdiff_t          iOffset     = 0;
};

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
