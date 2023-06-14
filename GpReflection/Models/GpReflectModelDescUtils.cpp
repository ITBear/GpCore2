#include "GpReflectModelDescUtils.hpp"
#include "../Builders/GpReflectModelBuilder.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

GpReflectModel  GpReflectModelDescUtils::SDescToModel (const GpReflectModelDesc& aDesc)
{
    return GpReflectModelBuilder()
        .UID(aDesc.uid)
        .BaseUID(aDesc.base_uid)
        .GroupID(aDesc.group_id)
        .Name(aDesc.name)
        .SetPropBuilder(SDescToModelPropBuilder(aDesc.props))
    .Build();
}

GpReflectModelDesc::SP  GpReflectModelDescUtils::SModelToDesc (const GpReflectModel& aModel)
{
    GpReflectModelDesc::SP modelDescSP = MakeSP<GpReflectModelDesc>
    (
        aModel.Uid(),
        aModel.BaseUid(),
        aModel.GroupId(),
        std::u8string(aModel.Name()),
        SModelToDescProps(aModel.Props())
    );

    return modelDescSP;
}

GpReflectPropDesc::C::Vec::SP   GpReflectModelDescUtils::SModelToDescProps (const GpReflectProp::C::Vec::Val& aProps)
{
    GpReflectPropDesc::C::Vec::SP propsDesc;
    propsDesc.reserve(aProps.size());

    for (const GpReflectProp& prop: aProps)
    {
        propsDesc.emplace_back(MakeSP<GpReflectPropDesc>
        (
            prop.Type(),
            prop.ContainerKeyType(),
            prop.Container(),
            prop.ModelUid(),
            std::u8string(prop.Name())
        ));
    }

    return propsDesc;
}

GpReflectPropBuilder    GpReflectModelDescUtils::SDescToModelPropBuilder (const GpReflectPropDesc::C::Vec::SP& aPropsDesc)
{
    GpReflectPropBuilder propBuilder;

    for (const GpReflectPropDesc::SP& propDescSP: aPropsDesc)
    {
        const GpReflectPropDesc& propDesc = propDescSP.V();

        switch (propDesc.container_type.Value())
        {
            case GpReflectContainerType::NO:
            {
                SAddProps_Val(propDesc, propBuilder);
            } break;
            case GpReflectContainerType::VECTOR:
            {
                SAddProps_Vec(propDesc, propBuilder);
            } break;
            case GpReflectContainerType::MAP:
            {
                SAddProps_Map(propDesc, propBuilder);
            } break;
            default:
            {
                THROW_GP(u8"Unsupported container type"_sv);
            }
        }
    }

    propBuilder.Check();

    return propBuilder;
}

void    GpReflectModelDescUtils::SAddProps_Val
(
    const GpReflectPropDesc&    aPropDesc,
    GpReflectPropBuilder&       aPropBuilder
)
{
    std::u8string_view name = aPropDesc.name;

    switch (aPropDesc.val_type.Value())
    {
        case GpReflectType::U_INT_8:
        {
            aPropBuilder.UI8(name);
        } break;
        case GpReflectType::S_INT_8:
        {
            aPropBuilder.SI8(name);
        } break;
        case GpReflectType::U_INT_16:
        {
            aPropBuilder.UI16(name);
        } break;
        case GpReflectType::S_INT_16:
        {
            aPropBuilder.SI16(name);
        } break;
        case GpReflectType::U_INT_32:
        {
            aPropBuilder.UI32(name);
        } break;
        case GpReflectType::S_INT_32:
        {
            aPropBuilder.SI32(name);
        } break;
        case GpReflectType::U_INT_64:
        {
            aPropBuilder.UI64(name);
        } break;
        case GpReflectType::S_INT_64:
        {
            aPropBuilder.SI64(name);
        } break;
        case GpReflectType::DOUBLE:
        {
            aPropBuilder.Double(name);
        } break;
        case GpReflectType::FLOAT:
        {
            aPropBuilder.Float(name);
        } break;
        case GpReflectType::BOOLEAN:
        {
            aPropBuilder.Bool(name);
        } break;
        case GpReflectType::UUID:
        {
            aPropBuilder.Uuid(name);
        } break;
        case GpReflectType::STRING:
        {
            aPropBuilder.String(name);
        } break;
        case GpReflectType::BLOB:
        {
            aPropBuilder.Blob(name);
        } break;
        case GpReflectType::OBJECT:
        {
            const GpReflectModel& model = GpReflectManager::S().Find(aPropDesc.model_uid);
            aPropBuilder.Object(name, model);
        } break;
        case GpReflectType::OBJECT_SP:
        {
            const GpReflectModel& model = GpReflectManager::S().Find(aPropDesc.model_uid);
            aPropBuilder.ObjectSP(name, model);
        } break;
        case GpReflectType::ENUM:
        {
            //TODO: implement enum
            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case GpReflectType::ENUM_FLAGS:
        {
            //TODO: implement enum flags
            THROW_GP_NOT_IMPLEMENTED();
        } break;
        case GpReflectType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GP(u8"Unsupported value type"_sv);
        }
    }
}

void    GpReflectModelDescUtils::SAddProps_Vec
(
    const GpReflectPropDesc&    aPropDesc,
    GpReflectPropBuilder&       aPropBuilder
)
{
    std::u8string_view name = aPropDesc.name;

    switch (aPropDesc.val_type.Value())
    {
        case GpReflectType::U_INT_8:
        {
            aPropBuilder.Vec_UI8(name);
        } break;
        case GpReflectType::S_INT_8:
        {
            aPropBuilder.Vec_SI8(name);
        } break;
        case GpReflectType::U_INT_16:
        {
            aPropBuilder.Vec_UI16(name);
        } break;
        case GpReflectType::S_INT_16:
        {
            aPropBuilder.Vec_SI16(name);
        } break;
        case GpReflectType::U_INT_32:
        {
            aPropBuilder.Vec_UI32(name);
        } break;
        case GpReflectType::S_INT_32:
        {
            aPropBuilder.Vec_SI32(name);
        } break;
        case GpReflectType::U_INT_64:
        {
            aPropBuilder.Vec_UI64(name);
        } break;
        case GpReflectType::S_INT_64:
        {
            aPropBuilder.Vec_SI64(name);
        } break;
        case GpReflectType::DOUBLE:
        {
            aPropBuilder.Vec_Double(name);
        } break;
        case GpReflectType::FLOAT:
        {
            aPropBuilder.Vec_Float(name);
        } break;
        case GpReflectType::BOOLEAN:
        {
            aPropBuilder.Vec_Bool(name);
        } break;
        case GpReflectType::UUID:
        {
            aPropBuilder.Vec_Uuid(name);
        } break;
        case GpReflectType::STRING:
        {
            aPropBuilder.Vec_String(name);
        } break;
        case GpReflectType::BLOB:
        {
            aPropBuilder.Vec_Blob(name);
        } break;
        case GpReflectType::OBJECT_SP:
        {
            const GpReflectModel& model = GpReflectManager::S().Find(aPropDesc.model_uid);
            aPropBuilder.Vec_ObjectSP(name, model);
        } break;
        case GpReflectType::OBJECT:
        case GpReflectType::ENUM:
        case GpReflectType::ENUM_FLAGS:
        case GpReflectType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GP(u8"Unsupported value type"_sv);
        }
    }
}

void    GpReflectModelDescUtils::SAddProps_Map
(
    const GpReflectPropDesc&    aPropDesc,
    GpReflectPropBuilder&       aPropBuilder
)
{
    std::u8string_view name = aPropDesc.name;

    const GpReflectType::EnumT keyType = aPropDesc.key_type.Value();
    const GpReflectType::EnumT valType = aPropDesc.val_type.Value();

    switch (valType)
    {
        case GpReflectType::U_INT_8:
        {
            aPropBuilder.Map_UI8(name, keyType);
        } break;
        case GpReflectType::S_INT_8:
        {
            aPropBuilder.Map_SI8(name, keyType);
        } break;
        case GpReflectType::U_INT_16:
        {
            aPropBuilder.Map_UI16(name, keyType);
        } break;
        case GpReflectType::S_INT_16:
        {
            aPropBuilder.Map_SI16(name, keyType);
        } break;
        case GpReflectType::U_INT_32:
        {
            aPropBuilder.Map_UI32(name, keyType);
        } break;
        case GpReflectType::S_INT_32:
        {
            aPropBuilder.Map_SI32(name, keyType);
        } break;
        case GpReflectType::U_INT_64:
        {
            aPropBuilder.Map_UI64(name, keyType);
        } break;
        case GpReflectType::S_INT_64:
        {
            aPropBuilder.Map_SI64(name, keyType);
        } break;
        case GpReflectType::DOUBLE:
        {
            aPropBuilder.Map_Double(name, keyType);
        } break;
        case GpReflectType::FLOAT:
        {
            aPropBuilder.Map_Float(name, keyType);
        } break;
        case GpReflectType::BOOLEAN:
        {
            aPropBuilder.Map_Bool(name, keyType);
        } break;
        case GpReflectType::UUID:
        {
            aPropBuilder.Map_Uuid(name, keyType);
        } break;
        case GpReflectType::STRING:
        {
            aPropBuilder.Map_String(name, keyType);
        } break;
        case GpReflectType::BLOB:
        {
            aPropBuilder.Map_Blob(name, keyType);
        } break;
        case GpReflectType::OBJECT_SP:
        {
            const GpReflectModel& model = GpReflectManager::S().Find(aPropDesc.model_uid);
            aPropBuilder.Map_ObjectSP(name, keyType, model);
        } break;
        case GpReflectType::OBJECT:
        case GpReflectType::ENUM:
        case GpReflectType::ENUM_FLAGS:
        case GpReflectType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GP(u8"Unsupported value type"_sv);
        }
    }
}

}//namespace GPlatform

#endif//GP_USE_REFLECTION
