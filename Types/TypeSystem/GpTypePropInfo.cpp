#include "GpTypePropInfo.hpp"
#include "GpTypeManager.hpp"

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
                                const GpTypePropFlags&  aFlags):
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

bool    GpTypePropInfo::FlagTest (const GpTypePropFlag::EnumT aFlag) const noexcept
{
    return iFlags.count(aFlag) > 0;
}

std::optional<std::string_view> GpTypePropInfo::FlagArg (const GpTypePropFlag::EnumT aFlag) const noexcept
{
    auto iter = iFlags.find(aFlag);

    if (iter != iFlags.end())
    {
        return iter->second;
    } else
    {
        return std::nullopt;
    }
}

const GpTypePropInfo&   GpTypePropInfo::UnwrapContainerKeyProp (void) const
{
    //
    THROW_GPE_COND
    (
           (Container() != ContainerT::NO)
        || (FlagTest(GpTypePropFlag::UNWRAP_CONTAINER))
        || (Type() == TypeT::STRUCT_SP),
        [&](){return "Failed to get unwrap container key. Struct UID "_sv + TypeUID().ToString();}
    );

    //
    auto typeStructInfoOpt = GpTypeManager::S().Find(TypeUID());

    THROW_GPE_COND
    (
        typeStructInfoOpt.has_value(),
        [&](){return "Struct info not found by UID "_sv + TypeUID().ToString();}
    );

    const auto& typeStructInfo = typeStructInfoOpt.value().get();

    //Find prop with flag GpTypePropFlag::UNWRAP_CONTAINER_KEY
    for (const auto& propInfo: typeStructInfo.Props())
    {
        if (propInfo.FlagTest(GpTypePropFlag::UNWRAP_CONTAINER_KEY))
        {
            return propInfo;
        }
    }

    THROW_GPE("No prop info with flag UNWRAP_CONTAINER_KEY was found for type UID"_sv + TypeUID().ToString());
}

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
