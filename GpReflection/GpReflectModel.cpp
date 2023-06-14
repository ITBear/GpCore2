#include "GpReflectModel.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectObject.hpp"

namespace GPlatform {

GpReflectModel::~GpReflectModel (void) noexcept
{
}

const GpReflectProp&    GpReflectModel::Prop (std::u8string_view aName) const
{
    //TODO: implement with flat map

    for (const GpReflectProp& prop: iProps)
    {
        if (prop.Name() == aName)
        {
            return prop;
        }
    }

    THROW_GP(u8"Property was not found by name '"_sv + aName + u8"'"_sv);
}

const GpReflectProp&    GpReflectModel::Prop
(
    std::u8string_view                  aName,
    const GpReflectType::EnumT          aType,
    const GpReflectContainerType::EnumT aContainerType
) const
{
    const GpReflectProp& prop = Prop(aName);

    THROW_COND_GP
    (
        prop.Type() == aType,
        [&](){return u8"Property '"_sv + Name() + u8"'::'"_sv + aName + u8"' have other type"_sv;}
    );

    THROW_COND_GP
    (
        prop.Container() == aContainerType,
        [&](){return u8"Property '"_sv + Name() + u8"'::'"_sv + aName + u8"' have other container type"_sv;}
    );

    return prop;
}

}//GPlatform

#endif//GP_USE_REFLECTION
