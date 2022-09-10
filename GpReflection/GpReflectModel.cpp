#include "GpReflectModel.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectObject.hpp"

namespace GPlatform {

GpReflectModel::GpReflectModel
(
    const GpUUID&               aUid,
    const GpUUID&               aBaseUid,
    std::string&&               aName,
    PropsT&&                    aProps,
    const GpUUID&               aGroupId,
    GpReflectObjectFactory::SP  aFactory,
    const size_t                aAlign,
    const size_t                aSize
) noexcept:
iUid(aUid),
iBaseUid(aBaseUid),
iName(std::move(aName)),
iProps(std::move(aProps)),
iGroupId(aGroupId),
iFactory(std::move(aFactory)),
iAlign(aAlign),
iSize(aSize)
{
}

GpReflectModel::GpReflectModel
(
    const GpUUID&               aUid,
    const GpUUID&               aBaseUid,
    std::string&&               aName,
    const GpUUID&               aGroupId,
    GpReflectObjectFactory::SP  aFactory
) noexcept:
iUid(aUid),
iBaseUid(aBaseUid),
iName(std::move(aName)),
iGroupId(aGroupId),
iFactory(std::move(aFactory)),
iAlign(1),
iSize(0)
{
}

GpReflectModel::GpReflectModel (const GpReflectModel& aModel):
iUid(aModel.iUid),
iBaseUid(aModel.iBaseUid),
iName(aModel.iName),
iProps(aModel.iProps),
iGroupId(aModel.iGroupId),
iFactory(aModel.iFactory),
iAlign(aModel.iAlign),
iSize(aModel.iSize)
{
}

GpReflectModel::GpReflectModel (GpReflectModel&& aModel) noexcept:
iUid(std::move(aModel.iUid)),
iBaseUid(std::move(aModel.iBaseUid)),
iName(std::move(aModel.iName)),
iProps(std::move(aModel.iProps)),
iGroupId(std::move(aModel.iGroupId)),
iFactory(std::move(aModel.iFactory)),
iAlign(std::move(aModel.iAlign)),
iSize(std::move(aModel.iSize))
{
}

GpReflectModel::~GpReflectModel (void) noexcept
{
}

const GpReflectProp&    GpReflectModel::Prop (std::string_view aName) const
{
    //TODO: implement with flat map

    for (const GpReflectProp& prop: iProps)
    {
        if (prop.Name() == aName)
        {
            return prop;
        }
    }

    THROW_GP("Property was not found by name '"_sv + aName + "'"_sv);
}

const GpReflectProp&    GpReflectModel::Prop
(
    std::string_view                    aName,
    const GpReflectType::EnumT          aType,
    const GpReflectContainerType::EnumT aContainerType
) const
{
    const GpReflectProp& prop = Prop(aName);

    THROW_COND_GP
    (
        prop.Type() == aType,
        [&](){return "Property '"_sv + Name() + "'::'"_sv + aName + "' have other type"_sv;}
    );

    THROW_COND_GP
    (
        prop.Container() == aContainerType,
        [&](){return "Property '"_sv + Name() + "'::'"_sv + aName + "' have other container type"_sv;}
    );

    return prop;
}

}//GPlatform

#endif//GP_USE_REFLECTION
