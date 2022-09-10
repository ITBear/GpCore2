#include "GpReflectProp.hpp"
#include "GpReflectManager.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

GpReflectProp::GpReflectProp (void) noexcept
{
}

GpReflectProp::GpReflectProp
(
    const TypeTE            aType,
    const GpUUID            aModelUid,
    const ContainerTE       aContainer,
    const TypeTE            aContainerKeyType,
    std::string&&           aName,
    const size_t            aAlign,
    const size_t            aSize,
    const std::ptrdiff_t    aOffset,
    const FlagsT            aFlags,
    FlagArgsT&&             aFlagArgs,
    GenFnOptT               aGenFn,
    ProcessCustomFnOptT     aConstructCustomFn,
    ProcessCustomFnOptT     aDestructCustomFn
) noexcept:
iType(aType),
iModelUid(aModelUid),
iContainer(aContainer),
iContainerKeyType(aContainerKeyType),
iName(std::move(aName)),
iAlign(aAlign),
iSize(aSize),
iOffset(aOffset),
iFlags(std::move(aFlags)),
iFlagArgs(aFlagArgs),
iGenFn(aGenFn),
iConstructCustomFn(aConstructCustomFn),
iDestructCustomFn(aDestructCustomFn)
{
}

GpReflectProp::GpReflectProp (const GpReflectProp& aProp):
iType(aProp.iType),
iModelUid(aProp.iModelUid),
iContainer(aProp.iContainer),
iContainerKeyType(aProp.iContainerKeyType),
iName(aProp.iName),
iAlign(aProp.iAlign),
iSize(aProp.iSize),
iOffset(aProp.iOffset),
iFlags(aProp.iFlags),
iFlagArgs(aProp.iFlagArgs),
iGenFn(aProp.iGenFn),
iConstructCustomFn(aProp.iConstructCustomFn),
iDestructCustomFn(aProp.iDestructCustomFn)
{
}

GpReflectProp::GpReflectProp (GpReflectProp&& aProp) noexcept:
iType(std::move(aProp.iType)),
iModelUid(std::move(aProp.iModelUid)),
iContainer(std::move(aProp.iContainer)),
iContainerKeyType(std::move(aProp.iContainerKeyType)),
iName(std::move(aProp.iName)),
iAlign(std::move(aProp.iAlign)),
iSize(std::move(aProp.iSize)),
iOffset(std::move(aProp.iOffset)),
iFlags(std::move(aProp.iFlags)),
iFlagArgs(std::move(aProp.iFlagArgs)),
iGenFn(std::move(aProp.iGenFn)),
iConstructCustomFn(std::move(aProp.iConstructCustomFn)),
iDestructCustomFn(std::move(aProp.iDestructCustomFn))
{
}

GpReflectProp::~GpReflectProp (void) noexcept
{
}

GpReflectProp&  GpReflectProp::operator= (const GpReflectProp& aProp)
{
    iType               = aProp.iType;
    iModelUid           = aProp.iModelUid;
    iContainer          = aProp.iContainer;
    iContainerKeyType   = aProp.iContainerKeyType;
    iName               = aProp.iName;
    iAlign              = aProp.iAlign;
    iSize               = aProp.iSize;
    iOffset             = aProp.iOffset;
    iFlags              = aProp.iFlags;
    iFlagArgs           = aProp.iFlagArgs;
    iGenFn              = aProp.iGenFn;
    iConstructCustomFn  = aProp.iConstructCustomFn;
    iDestructCustomFn   = aProp.iDestructCustomFn;

    return *this;
}

GpReflectProp&  GpReflectProp::operator= (GpReflectProp&& aProp) noexcept
{
    iType               = std::move(aProp.iType);
    iModelUid           = std::move(aProp.iModelUid);
    iContainer          = std::move(aProp.iContainer);
    iContainerKeyType   = std::move(aProp.iContainerKeyType);
    iName               = std::move(aProp.iName);
    iAlign              = std::move(aProp.iAlign);
    iSize               = std::move(aProp.iSize);
    iOffset             = std::move(aProp.iOffset);
    iFlags              = std::move(aProp.iFlags);
    iFlagArgs           = std::move(aProp.iFlagArgs);
    iGenFn              = std::move(aProp.iGenFn);
    iConstructCustomFn  = std::move(aProp.iConstructCustomFn);
    iDestructCustomFn   = std::move(aProp.iDestructCustomFn);

    return *this;
}

std::optional<std::string_view> GpReflectProp::FlagArg (const GpReflectPropFlag::EnumT aFlag) const noexcept
{
    auto iter = iFlagArgs.find(aFlag);

    if (iter != iFlagArgs.end())
    {
        return iter->second;
    } else
    {
        return std::nullopt;
    }
}

const GpReflectProp&    GpReflectProp::UnwrapContainerKeyProp (void) const
{
    //
    THROW_COND_GP
    (
           (Container() != ContainerT::NO)
        || (FlagTest(GpReflectPropFlag::UNWRAP_CONTAINER))
        || (Type() == TypeT::OBJECT_SP),
        [&](){return "Failed to get container key. Model UID "_sv + ModelUid();}
    );

    //
    const GpReflectModel& model = GpReflectManager::S().Find(ModelUid());

    //Find prop with flag GpReflectPropFlag::UNWRAP_CONTAINER_KEY
    for (const auto& prop: model.Props())
    {
        if (prop.FlagTest(GpReflectPropFlag::UNWRAP_CONTAINER_KEY))
        {
            return prop;
        }
    }

    THROW_GP("No property with UNWRAP_CONTAINER_KEY flag was found for model UID "_sv + ModelUid());
}

bool    GpReflectProp::GenFn (void* aDataPtr) const
{
    if (iGenFn.has_value())
    {
        iGenFn.value()(*this, aDataPtr);
        return true;
    }

    return false;
}

void    GpReflectProp::ConstructCustom (void* aDataPtr) const
{
    if (iConstructCustomFn.has_value())
    {
        iConstructCustomFn.value()(PropPtr(aDataPtr));
    } else
    {
        THROW_GP("There are no custom construct function"_sv);
    }
}

void    GpReflectProp::DestructCustom (void* aDataPtr) const
{
    if (iDestructCustomFn.has_value())
    {
        iDestructCustomFn.value()(PropPtr(aDataPtr));
    } else
    {
        THROW_GP("There are no custom destruct function"_sv);
    }
}

}//GPlatform

#endif//GP_USE_REFLECTION
