#include "GpTypePropInfo.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

GpTypePropInfo::GpTypePropInfo (void) noexcept
{
}

GpTypePropInfo::GpTypePropInfo (const TypeTE            aType,
                                const GpUUID            aTypeUID,
                                const ContainerTE       aContainer,
                                const TypeTE            aContainerKeyType,
                                std::string_view        aName,
                                const size_t            aAlign,
                                const size_t            aSize,
                                const std::ptrdiff_t    aOffset,
                                const GpTypePropFlags   aFlags):
iType(aType),
iTypeUID(aTypeUID),
iContainer(aContainer),
iContainerKeyType(aContainerKeyType),
iName(aName),
iAlign(aAlign),
iSize(aSize),
iOffset(aOffset),
iFlags(aFlags)
{
}

GpTypePropInfo::GpTypePropInfo (const GpTypePropInfo& aPropInfo):
iType(aPropInfo.iType),
iTypeUID(aPropInfo.iTypeUID),
iContainer(aPropInfo.iContainer),
iContainerKeyType(aPropInfo.iContainerKeyType),
iName(aPropInfo.iName),
iAlign(aPropInfo.iAlign),
iSize(aPropInfo.iSize),
iOffset(aPropInfo.iOffset),
iFlags(aPropInfo.iFlags)
{
}

GpTypePropInfo::GpTypePropInfo (GpTypePropInfo&& aPropInfo) noexcept:
iType(std::move(aPropInfo.iType)),
iTypeUID(std::move(aPropInfo.iTypeUID)),
iContainer(std::move(aPropInfo.iContainer)),
iContainerKeyType(std::move(aPropInfo.iContainerKeyType)),
iName(std::move(aPropInfo.iName)),
iAlign(std::move(aPropInfo.iAlign)),
iSize(std::move(aPropInfo.iSize)),
iOffset(std::move(aPropInfo.iOffset)),
iFlags(std::move(aPropInfo.iFlags))
{
}

GpTypePropInfo::~GpTypePropInfo (void) noexcept
{   
}

GpTypePropInfo& GpTypePropInfo::operator= (const GpTypePropInfo& aPropInfo)
{
    iType               = aPropInfo.iType;
    iTypeUID            = aPropInfo.iTypeUID;
    iContainer          = aPropInfo.iContainer;
    iContainerKeyType   = aPropInfo.iContainerKeyType;
    iName               = aPropInfo.iName;
    iAlign              = aPropInfo.iAlign;
    iSize               = aPropInfo.iSize;
    iOffset             = aPropInfo.iOffset;
    iFlags              = aPropInfo.iFlags;

    return *this;
}

GpTypePropInfo& GpTypePropInfo::operator= (GpTypePropInfo&& aPropInfo) noexcept
{
    iType               = std::move(aPropInfo.iType);
    iTypeUID            = std::move(aPropInfo.iTypeUID);
    iContainer          = std::move(aPropInfo.iContainer);
    iContainerKeyType   = std::move(aPropInfo.iContainerKeyType);
    iName               = std::move(aPropInfo.iName);
    iAlign              = std::move(aPropInfo.iAlign);
    iSize               = std::move(aPropInfo.iSize);
    iOffset             = std::move(aPropInfo.iOffset);
    iFlags              = std::move(aPropInfo.iFlags);

    return *this;
}

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
