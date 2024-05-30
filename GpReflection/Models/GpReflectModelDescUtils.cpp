#include "GpReflectModelDescUtils.hpp"
#include "../Builders/GpReflectModelBuilder.hpp"

namespace GPlatform {

GpReflectModel::CSP GpReflectModelDescUtils::SDescToModel (const GpReflectModelDesc& aDesc)
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
        std::string(aModel.Name()),
        SModelToDescProps(aModel.Props())
    );

    return modelDescSP;
}

GpReflectPropDesc::C::Vec::SP   GpReflectModelDescUtils::SModelToDescProps (const GpReflectProp::SmallVecVal& aProps)
{
    GpReflectPropDesc::C::Vec::SP propsDesc;
    propsDesc.reserve(std::size(aProps));

    for (const GpReflectProp& prop: aProps)
    {
        propsDesc.emplace_back(MakeSP<GpReflectPropDesc>
        (
            prop.Type(),
            prop.ContainerKeyType(),
            prop.Container(),
            prop.ModelUid(),
            std::string(prop.Name())
        ));
    }

    return propsDesc;
}

GpReflectModelPropBuilder   GpReflectModelDescUtils::SDescToModelPropBuilder (const GpReflectPropDesc::C::Vec::SP& aPropsDesc)
{
    GpReflectModelPropBuilder propBuilder;

    for (const GpReflectPropDesc::SP& propDescSP: aPropsDesc)
    {
        const GpReflectPropDesc& propDesc = propDescSP.V();

        switch (propDesc.container_type)
        {
            case GpReflectContainerType::NO:
            {
                SAddProps_Val(propDesc, propBuilder);
            } break;
            case GpReflectContainerType::VECTOR: [[fallthrough]];
            case GpReflectContainerType::VECTOR_WRAP:
            {
                SAddProps_Vec(propDesc, propBuilder);
            } break;
            case GpReflectContainerType::MAP:
            {
                SAddProps_Map(propDesc, propBuilder);
            } break;
            default:
            {
                THROW_GP("Unsupported container type"_sv);
            }
        }
    }

    propBuilder.Check();

    return propBuilder;
}

void    GpReflectModelDescUtils::SAddProps_Val
(
    const GpReflectPropDesc&    aPropDesc,
    GpReflectModelPropBuilder&  aPropBuilder
)
{
    std::string_view name = aPropDesc.name;

    switch (aPropDesc.val_type)
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
            aPropBuilder.UUID(name);
        } break;
        case GpReflectType::STRING:
        {
            aPropBuilder.String(name);
        } break;
        case GpReflectType::BLOB:
        {
            aPropBuilder.BLOB(name);
        } break;
        case GpReflectType::OBJECT:
        {
            GpReflectModel::CSP modelCSP = GpReflectManager::S().Find(aPropDesc.model_uid);
            aPropBuilder.Object(name, modelCSP.Vn());
        } break;
        case GpReflectType::OBJECT_SP:
        {
            GpReflectModel::CSP modelCSP = GpReflectManager::S().Find(aPropDesc.model_uid);
            aPropBuilder.ObjectSP(name, modelCSP.Vn());
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
            THROW_GP("Unsupported value type"_sv);
        }
    }
}

void    GpReflectModelDescUtils::SAddProps_Vec
(
    const GpReflectPropDesc&    aPropDesc,
    GpReflectModelPropBuilder&  aPropBuilder
)
{
    std::string_view name = aPropDesc.name;

    switch (aPropDesc.val_type)
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
        case GpReflectType::UUID:
        {
            aPropBuilder.Vec_UUID(name);
        } break;
        case GpReflectType::STRING:
        {
            aPropBuilder.Vec_String(name);
        } break;
        case GpReflectType::BLOB:
        {
            aPropBuilder.Vec_BLOB(name);
        } break;
        case GpReflectType::OBJECT:
        {
            GpReflectModel::CSP modelCSP = GpReflectManager::S().Find(aPropDesc.model_uid);
            aPropBuilder.Vec_Object(name, modelCSP.Vn());
        } break;
        case GpReflectType::OBJECT_SP:
        {
            GpReflectModel::CSP modelCSP = GpReflectManager::S().Find(aPropDesc.model_uid);
            aPropBuilder.Vec_ObjectSP(name, modelCSP.Vn());
        } break;
        case GpReflectType::ENUM:       [[fallthrough]];
        case GpReflectType::ENUM_FLAGS: [[fallthrough]];
        case GpReflectType::BOOLEAN:    [[fallthrough]];
        case GpReflectType::NOT_SET:    [[fallthrough]];
        default:
        {
            THROW_GP("Unsupported value type"_sv);
        }
    }
}

void    GpReflectModelDescUtils::SAddProps_Map
(
    const GpReflectPropDesc&    aPropDesc,
    GpReflectModelPropBuilder&  aPropBuilder
)
{
    std::string_view name = aPropDesc.name;

    const GpReflectType::EnumT keyType = aPropDesc.key_type;
    const GpReflectType::EnumT valType = aPropDesc.val_type;

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
        case GpReflectType::UUID:
        {
            aPropBuilder.Map_UUID(name, keyType);
        } break;
        case GpReflectType::STRING:
        {
            aPropBuilder.Map_String(name, keyType);
        } break;
        case GpReflectType::BLOB:
        {
            aPropBuilder.Map_BLOB(name, keyType);
        } break;
        case GpReflectType::OBJECT_SP:
        {
            GpReflectModel::CSP modelCSP = GpReflectManager::S().Find(aPropDesc.model_uid);
            aPropBuilder.Map_ObjectSP(name, keyType, modelCSP.Vn());
        } break;
        case GpReflectType::OBJECT:     [[fallthrough]];
        case GpReflectType::ENUM:       [[fallthrough]];
        case GpReflectType::ENUM_FLAGS: [[fallthrough]];
        case GpReflectType::BOOLEAN:    [[fallthrough]];
        case GpReflectType::NOT_SET:    [[fallthrough]];
        default:
        {
            THROW_GP("Unsupported value type"_sv);
        }
    }
}

}// namespace GPlatform
