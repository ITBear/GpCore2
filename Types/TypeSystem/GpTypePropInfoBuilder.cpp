#include "GpTypePropInfoBuilder.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeManager.hpp"

namespace GPlatform {

GpTypePropInfoBuilder::GpTypePropInfoBuilder (void) noexcept
{
}

GpTypePropInfoBuilder::GpTypePropInfoBuilder (GpTypePropInfoBuilder&& aBuilder) noexcept:
iStructDynamicBuilder(aBuilder.iStructDynamicBuilder),
iProps(std::move(aBuilder.iProps)),
iOffset(std::move(aBuilder.iOffset))
{
}

GpTypePropInfoBuilder::GpTypePropInfoBuilder (GpTypeStructDynamicBuilder& aStructDynamicBuilder) noexcept:
iStructDynamicBuilder(&aStructDynamicBuilder)
{
}

GpTypePropInfoBuilder::~GpTypePropInfoBuilder (void) noexcept
{
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::operator= (GpTypePropInfoBuilder&& aBuilder) noexcept
{
    iStructDynamicBuilder   = aBuilder.iStructDynamicBuilder;
    iProps                  = std::move(aBuilder.iProps);
    iOffset                 = std::move(aBuilder.iOffset);

    return *this;
}

GpTypeStructDynamicBuilder& GpTypePropInfoBuilder::DoneBuildProps (void)
{
    CheckForProps();

    THROW_GPE_COND
    (
        iStructDynamicBuilder != nullptr,
        "iStructDynamicBuilder is null"_sv
    );

    return *iStructDynamicBuilder;
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::AddProps  (const GpTypeStructDynamicPropDesc::C::Vec::SP& aProps)
{
    for (const GpTypeStructDynamicPropDesc::SP& propDescSP: aProps)
    {
        const GpTypeStructDynamicPropDesc& propDesc = propDescSP.VC();

        switch (propDesc.container_type.Value())
        {
            case GpTypeContainer::NO:
            {
                AddProps_Val(propDesc);
            } break;
            case GpTypeContainer::VECTOR:
            {
                AddProps_Vec(propDesc);
            } break;
            case GpTypeContainer::LIST:
            {
                AddProps_List(propDesc);
            } break;
            case GpTypeContainer::SET:
            {
                AddProps_Set(propDesc);
            } break;
            case GpTypeContainer::MAP:
            {
                AddProps_Map(propDesc);
            } break;
            default:
            {
                THROW_GPE("Unsupported container type"_sv);
            }
        }
    }

    return *this;
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::UI8 (std::string_view aName)
{
    return AddProp<u_int_8, GpType::U_INT_8, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::SI8 (std::string_view aName)
{
    return AddProp<s_int_8, GpType::S_INT_8, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::UI16 (std::string_view aName)
{
    return AddProp<u_int_16, GpType::U_INT_16, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::SI16 (std::string_view aName)
{
    return AddProp<s_int_16, GpType::S_INT_16, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::UI32 (std::string_view aName)
{
    return AddProp<u_int_32, GpType::U_INT_32, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::SI32 (std::string_view aName)
{
    return AddProp<s_int_32, GpType::S_INT_32, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::UI64 (std::string_view aName)
{
    return AddProp<u_int_64, GpType::U_INT_64, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::SI64 (std::string_view aName)
{
    return AddProp<s_int_64, GpType::S_INT_64, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::UnixTsS (std::string_view aName)
{
    return AddProp<s_int_64, GpType::UNIX_TS_S, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::UnixTsMS (std::string_view aName)
{
    return AddProp<s_int_64, GpType::UNIX_TS_MS, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Double (std::string_view aName)
{
    return AddProp<double, GpType::DOUBLE, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Float (std::string_view aName)
{
    return AddProp<float, GpType::FLOAT, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Bool (std::string_view aName)
{
    return AddProp<bool, GpType::BOOLEAN, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Uuid (std::string_view aName)
{
    return AddProp<GpUUID, GpType::UUID, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::String (std::string_view aName)
{
    return AddProp<std::string, GpType::STRING, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Blob (std::string_view aName)
{
    return AddProp<GpBytesArray, GpType::BLOB, GpTypeContainer::NO>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Struct
(
    std::string_view        aName,
    const GpTypeStructInfo& aTypeStructInfo
)
{
    return AddProp<GpTypeStructBase, GpType::STRUCT, GpTypeContainer::NO>(aName, aTypeStructInfo);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::StructSP
(
    std::string_view        aName,
    const GpTypeStructInfo& aTypeStructInfo
)
{
    return AddProp<GpTypeStructBase::SP, GpType::STRUCT_SP, GpTypeContainer::NO>(aName, aTypeStructInfo);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_UI8 (std::string_view aName)
{
    return AddProp<u_int_8, GpType::U_INT_8, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_SI8 (std::string_view aName)
{
    return AddProp<s_int_8, GpType::S_INT_8, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_UI16 (std::string_view aName)
{
    return AddProp<u_int_16, GpType::U_INT_16, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_SI16 (std::string_view aName)
{
    return AddProp<s_int_16, GpType::S_INT_16, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_UI32 (std::string_view aName)
{
    return AddProp<u_int_32, GpType::U_INT_32, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_SI32 (std::string_view aName)
{
    return AddProp<s_int_32, GpType::S_INT_32, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_UI64 (std::string_view aName)
{
    return AddProp<u_int_64, GpType::U_INT_64, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_SI64 (std::string_view aName)
{
    return AddProp<s_int_64, GpType::S_INT_64, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_UnixTsS (std::string_view aName)
{
    return AddProp<s_int_64, GpType::UNIX_TS_S, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_UnixTsMS (std::string_view aName)
{
    return AddProp<s_int_64, GpType::UNIX_TS_MS, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_Double (std::string_view aName)
{
    return AddProp<double, GpType::DOUBLE, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_Float (std::string_view aName)
{
    return AddProp<float, GpType::FLOAT, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_Bool (std::string_view aName)
{
    return AddProp<bool, GpType::BOOLEAN, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_Uuid (std::string_view aName)
{
    return AddProp<GpUUID, GpType::UUID, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_String (std::string_view aName)
{
    return AddProp<std::string, GpType::STRING, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_Blob (std::string_view aName)
{
    return AddProp<GpBytesArray, GpType::BLOB, GpTypeContainer::VECTOR>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Vec_StructSP
(
    std::string_view        aName,
    const GpTypeStructInfo& aTypeStructInfo
)
{
    return AddProp<GpTypeStructBase::SP, GpType::STRUCT_SP, GpTypeContainer::VECTOR>(aName, aTypeStructInfo);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_UI8 (std::string_view aName)
{
    return AddProp<u_int_8, GpType::U_INT_8, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_SI8 (std::string_view aName)
{
    return AddProp<s_int_8, GpType::S_INT_8, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_UI16 (std::string_view aName)
{
    return AddProp<u_int_16, GpType::U_INT_16, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_SI16 (std::string_view aName)
{
    return AddProp<s_int_16, GpType::S_INT_16, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_UI32 (std::string_view aName)
{
    return AddProp<u_int_32, GpType::U_INT_32, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_SI32 (std::string_view aName)
{
    return AddProp<s_int_32, GpType::S_INT_32, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_UI64 (std::string_view aName)
{
    return AddProp<u_int_64, GpType::U_INT_64, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_SI64 (std::string_view aName)
{
    return AddProp<s_int_64, GpType::S_INT_64, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_UnixTsS (std::string_view aName)
{
    return AddProp<s_int_64, GpType::UNIX_TS_S, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_UnixTsMS (std::string_view aName)
{
    return AddProp<s_int_64, GpType::UNIX_TS_MS, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_Double (std::string_view aName)
{
    return AddProp<double, GpType::DOUBLE, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_Float (std::string_view aName)
{
    return AddProp<float, GpType::FLOAT, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_Bool (std::string_view aName)
{
    return AddProp<bool, GpType::BOOLEAN, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_Uuid (std::string_view aName)
{
    return AddProp<GpUUID, GpType::UUID, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_String (std::string_view aName)
{
    return AddProp<std::string, GpType::STRING, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_Blob (std::string_view aName)
{
    return AddProp<GpBytesArray, GpType::BLOB, GpTypeContainer::LIST>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::List_StructSP
(
    std::string_view        aName,
    const GpTypeStructInfo& aTypeStructInfo
)
{
    return AddProp<GpTypeStructBase::SP, GpType::STRUCT_SP, GpTypeContainer::LIST>(aName, aTypeStructInfo);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_UI8 (std::string_view aName)
{
    return AddProp<u_int_8, GpType::U_INT_8, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_SI8 (std::string_view aName)
{
    return AddProp<s_int_8, GpType::S_INT_8, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_UI16 (std::string_view aName)
{
    return AddProp<u_int_16, GpType::U_INT_16, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_SI16 (std::string_view aName)
{
    return AddProp<s_int_16, GpType::S_INT_16, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_UI32 (std::string_view aName)
{
    return AddProp<u_int_32, GpType::U_INT_32, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_SI32 (std::string_view aName)
{
    return AddProp<s_int_32, GpType::S_INT_32, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_UI64 (std::string_view aName)
{
    return AddProp<u_int_64, GpType::U_INT_64, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_SI64 (std::string_view aName)
{
    return AddProp<s_int_64, GpType::S_INT_64, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_UnixTsS (std::string_view aName)
{
    return AddProp<s_int_64, GpType::UNIX_TS_S, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_UnixTsMS (std::string_view aName)
{
    return AddProp<s_int_64, GpType::UNIX_TS_MS, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_Double (std::string_view aName)
{
    return AddProp<double, GpType::DOUBLE, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_Float (std::string_view aName)
{
    return AddProp<float, GpType::FLOAT, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_Bool (std::string_view aName)
{
    return AddProp<bool, GpType::BOOLEAN, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_Uuid (std::string_view aName)
{
    return AddProp<GpUUID, GpType::UUID, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_String (std::string_view aName)
{
    return AddProp<std::string, GpType::STRING, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_Blob (std::string_view aName)
{
    return AddProp<GpBytesArray, GpType::BLOB, GpTypeContainer::SET>(aName);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Set_StructSP
(
    std::string_view        aName,
    const GpTypeStructInfo& aTypeStructInfo
)
{
    return AddProp<GpTypeStructBase::SP, GpType::STRUCT_SP, GpTypeContainer::SET>(aName, aTypeStructInfo);
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_UI8
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<u_int_8, GpType::U_INT_8>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_SI8
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<s_int_8, GpType::S_INT_8>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_UI16
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<u_int_16, GpType::U_INT_16>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_SI16
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<s_int_16, GpType::S_INT_16>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_UI32
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<u_int_32, GpType::U_INT_32>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_SI32
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<s_int_32, GpType::S_INT_32>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_UI64
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<u_int_64, GpType::U_INT_64>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_SI64
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<s_int_64, GpType::S_INT_64>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_UnixTsS
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<s_int_64, GpType::UNIX_TS_S>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_UnixTsMS
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<s_int_64, GpType::UNIX_TS_MS>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_Double
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<double, GpType::DOUBLE>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_Float
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<float, GpType::FLOAT>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_Bool
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<bool, GpType::BOOLEAN>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_Uuid
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<GpUUID, GpType::UUID>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_String
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<std::string, GpType::STRING>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_Blob
(
    std::string_view    aName,
    GpType::EnumT       aKeyType
)
{
    return AddPropMap<GpBytesArray, GpType::BLOB>(aName, aKeyType, GpUUID::CE_Zero());
}

GpTypePropInfoBuilder&  GpTypePropInfoBuilder::Map_StructSP
(
    std::string_view        aName,
    GpType::EnumT           aKeyType,
    const GpTypeStructInfo& aTypeStructInfo
)
{
    return AddPropMap<GpTypeStructBase::SP, GpType::STRUCT_SP>(aName, aKeyType, aTypeStructInfo.UID());
}

void    GpTypePropInfoBuilder::UpdateOffsetToAlign
(
    const size_t aAlign,
    const size_t aSize
) noexcept
{
    char* chPtr = nullptr;
    chPtr += iOffset;

    void* ptr = chPtr;
    size_t space = std::numeric_limits<size_t>::max();

    std::align(aAlign, aSize, ptr, space);

    iOffset += std::numeric_limits<size_t>::max() - space;
}

void    GpTypePropInfoBuilder::CheckForProps (void)
{
    //
    //Check if all property names are unique
    std::set<std::string_view> names;

    for (const GpTypePropInfo& prop: iProps)
    {
        THROW_GPE_COND
        (
            names.count(prop.Name()) == 0,
            [&](){return "Property name '"_sv + prop.Name() + "' is not unique"_sv;}
        );

        names.emplace(prop.Name());
    }
}

void    GpTypePropInfoBuilder::AddProps_Val (const GpTypeStructDynamicPropDesc& aPropDesc)
{
    std::string_view name = aPropDesc.name;

    switch (aPropDesc.val_type.Value())
    {
        case GpType::U_INT_8:
        {
            UI8(name);
        } break;
        case GpType::S_INT_8:
        {
            SI8(name);
        } break;
        case GpType::U_INT_16:
        {
            UI16(name);
        } break;
        case GpType::S_INT_16:
        {
            SI16(name);
        } break;
        case GpType::U_INT_32:
        {
            UI32(name);
        } break;
        case GpType::S_INT_32:
        {
            SI32(name);
        } break;
        case GpType::U_INT_64:
        {
            UI64(name);
        } break;
        case GpType::S_INT_64:
        {
            SI64(name);
        } break;
        case GpType::UNIX_TS_S:
        {
            UnixTsS(name);
        } break;
        case GpType::UNIX_TS_MS:
        {
            UnixTsMS(name);
        } break;
        case GpType::DOUBLE:
        {
            Double(name);
        } break;
        case GpType::FLOAT:
        {
            Float(name);
        } break;
        case GpType::BOOLEAN:
        {
            Bool(name);
        } break;
        case GpType::UUID:
        {
            Uuid(name);
        } break;
        case GpType::STRING:
        {
            String(name);
        } break;
        case GpType::BLOB:
        {
            Blob(name);
        } break;
        case GpType::STRUCT:
        {
            GpTypeStructInfo::C::Opt::CRef typeInfoOpt = GpTypeManager::S().Find(aPropDesc.TypeUID());

            THROW_GPE_COND
            (
                typeInfoOpt.has_value(),
                [&](){return "Type struct info not found by uid '"_sv + aPropDesc.TypeUID().ToString() + "'"_sv;}
            );

            Struct(name, typeInfoOpt.value());
        } break;
        case GpType::STRUCT_SP:
        {
            GpTypeStructInfo::C::Opt::CRef typeInfoOpt = GpTypeManager::S().Find(aPropDesc.TypeUID());

            THROW_GPE_COND
            (
                typeInfoOpt.has_value(),
                [&](){return "Type struct info not found by uid '"_sv + aPropDesc.TypeUID().ToString() + "'"_sv;}
            );

            StructSP(name, typeInfoOpt.value());
        } break;
        case GpType::ENUM:
        {
            //TODO: implement enum
            THROW_GPE_NOT_IMPLEMENTED();
        } break;
        case GpType::ENUM_FLAGS:
        {
            //TODO: implement enum flags
            THROW_GPE_NOT_IMPLEMENTED();
        } break;
        case GpType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GPE("Unsupported value type"_sv);
        }
    }
}

void    GpTypePropInfoBuilder::AddProps_Vec (const GpTypeStructDynamicPropDesc& aPropDesc)
{
    std::string_view name = aPropDesc.name;

    switch (aPropDesc.val_type.Value())
    {
        case GpType::U_INT_8:
        {
            Vec_UI8(name);
        } break;
        case GpType::S_INT_8:
        {
            Vec_SI8(name);
        } break;
        case GpType::U_INT_16:
        {
            Vec_UI16(name);
        } break;
        case GpType::S_INT_16:
        {
            Vec_SI16(name);
        } break;
        case GpType::U_INT_32:
        {
            Vec_UI32(name);
        } break;
        case GpType::S_INT_32:
        {
            Vec_SI32(name);
        } break;
        case GpType::U_INT_64:
        {
            Vec_UI64(name);
        } break;
        case GpType::S_INT_64:
        {
            Vec_SI64(name);
        } break;
        case GpType::UNIX_TS_S:
        {
            Vec_UnixTsS(name);
        } break;
        case GpType::UNIX_TS_MS:
        {
            Vec_UnixTsMS(name);
        } break;
        case GpType::DOUBLE:
        {
            Vec_Double(name);
        } break;
        case GpType::FLOAT:
        {
            Vec_Float(name);
        } break;
        case GpType::BOOLEAN:
        {
            Vec_Bool(name);
        } break;
        case GpType::UUID:
        {
            Vec_Uuid(name);
        } break;
        case GpType::STRING:
        {
            Vec_String(name);
        } break;
        case GpType::BLOB:
        {
            Vec_Blob(name);
        } break;
        case GpType::STRUCT_SP:
        {
            GpTypeStructInfo::C::Opt::CRef typeInfoOpt = GpTypeManager::S().Find(aPropDesc.TypeUID());

            THROW_GPE_COND
            (
                typeInfoOpt.has_value(),
                [&](){return "Type struct info not found by uid '"_sv + aPropDesc.TypeUID().ToString() + "'"_sv;}
            );

            Vec_StructSP(name, typeInfoOpt.value());
        } break;
        case GpType::STRUCT:
        case GpType::ENUM:
        case GpType::ENUM_FLAGS:
        case GpType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GPE("Unsupported value type"_sv);
        }
    }
}

void    GpTypePropInfoBuilder::AddProps_List (const GpTypeStructDynamicPropDesc& aPropDesc)
{
    std::string_view name = aPropDesc.name;

    switch (aPropDesc.val_type.Value())
    {
        case GpType::U_INT_8:
        {
            List_UI8(name);
        } break;
        case GpType::S_INT_8:
        {
            List_SI8(name);
        } break;
        case GpType::U_INT_16:
        {
            List_UI16(name);
        } break;
        case GpType::S_INT_16:
        {
            List_SI16(name);
        } break;
        case GpType::U_INT_32:
        {
            List_UI32(name);
        } break;
        case GpType::S_INT_32:
        {
            List_SI32(name);
        } break;
        case GpType::U_INT_64:
        {
            List_UI64(name);
        } break;
        case GpType::S_INT_64:
        {
            List_SI64(name);
        } break;
        case GpType::UNIX_TS_S:
        {
            List_UnixTsS(name);
        } break;
        case GpType::UNIX_TS_MS:
        {
            List_UnixTsMS(name);
        } break;
        case GpType::DOUBLE:
        {
            List_Double(name);
        } break;
        case GpType::FLOAT:
        {
            List_Float(name);
        } break;
        case GpType::BOOLEAN:
        {
            List_Bool(name);
        } break;
        case GpType::UUID:
        {
            List_Uuid(name);
        } break;
        case GpType::STRING:
        {
            List_String(name);
        } break;
        case GpType::BLOB:
        {
            List_Blob(name);
        } break;
        case GpType::STRUCT_SP:
        {
            GpTypeStructInfo::C::Opt::CRef typeInfoOpt = GpTypeManager::S().Find(aPropDesc.TypeUID());

            THROW_GPE_COND
            (
                typeInfoOpt.has_value(),
                [&](){return "Type struct info not found by uid '"_sv + aPropDesc.TypeUID().ToString() + "'"_sv;}
            );

            List_StructSP(name, typeInfoOpt.value());
        } break;
        case GpType::STRUCT:
        case GpType::ENUM:
        case GpType::ENUM_FLAGS:
        case GpType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GPE("Unsupported value type"_sv);
        }
    }
}

void    GpTypePropInfoBuilder::AddProps_Set (const GpTypeStructDynamicPropDesc& aPropDesc)
{
    std::string_view name = aPropDesc.name;

    switch (aPropDesc.val_type.Value())
    {
        case GpType::U_INT_8:
        {
            Set_UI8(name);
        } break;
        case GpType::S_INT_8:
        {
            Set_SI8(name);
        } break;
        case GpType::U_INT_16:
        {
            Set_UI16(name);
        } break;
        case GpType::S_INT_16:
        {
            Set_SI16(name);
        } break;
        case GpType::U_INT_32:
        {
            Set_UI32(name);
        } break;
        case GpType::S_INT_32:
        {
            Set_SI32(name);
        } break;
        case GpType::U_INT_64:
        {
            Set_UI64(name);
        } break;
        case GpType::S_INT_64:
        {
            Set_SI64(name);
        } break;
        case GpType::UNIX_TS_S:
        {
            Set_UnixTsS(name);
        } break;
        case GpType::UNIX_TS_MS:
        {
            Set_UnixTsMS(name);
        } break;
        case GpType::DOUBLE:
        {
            Set_Double(name);
        } break;
        case GpType::FLOAT:
        {
            Set_Float(name);
        } break;
        case GpType::BOOLEAN:
        {
            Set_Bool(name);
        } break;
        case GpType::UUID:
        {
            Set_Uuid(name);
        } break;
        case GpType::STRING:
        {
            Set_String(name);
        } break;
        case GpType::BLOB:
        {
            Set_Blob(name);
        } break;
        case GpType::STRUCT_SP:
        {
            GpTypeStructInfo::C::Opt::CRef typeInfoOpt = GpTypeManager::S().Find(aPropDesc.TypeUID());

            THROW_GPE_COND
            (
                typeInfoOpt.has_value(),
                [&](){return "Type struct info not found by uid '"_sv + aPropDesc.TypeUID().ToString() + "'"_sv;}
            );

            Set_StructSP(name, typeInfoOpt.value());
        } break;
        case GpType::STRUCT:
        case GpType::ENUM:
        case GpType::ENUM_FLAGS:
        case GpType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GPE("Unsupported value type"_sv);
        }
    }
}

void    GpTypePropInfoBuilder::AddProps_Map (const GpTypeStructDynamicPropDesc& aPropDesc)
{
    std::string_view name = aPropDesc.name;

    const GpType::EnumT keyType = aPropDesc.key_type.Value();
    const GpType::EnumT valType = aPropDesc.val_type.Value();

    switch (valType)
    {
        case GpType::U_INT_8:
        {
            Map_UI8(name, keyType);
        } break;
        case GpType::S_INT_8:
        {
            Map_SI8(name, keyType);
        } break;
        case GpType::U_INT_16:
        {
            Map_UI16(name, keyType);
        } break;
        case GpType::S_INT_16:
        {
            Map_SI16(name, keyType);
        } break;
        case GpType::U_INT_32:
        {
            Map_UI32(name, keyType);
        } break;
        case GpType::S_INT_32:
        {
            Map_SI32(name, keyType);
        } break;
        case GpType::U_INT_64:
        {
            Map_UI64(name, keyType);
        } break;
        case GpType::S_INT_64:
        {
            Map_SI64(name, keyType);
        } break;
        case GpType::UNIX_TS_S:
        {
            Map_UnixTsS(name, keyType);
        } break;
        case GpType::UNIX_TS_MS:
        {
            Map_UnixTsMS(name, keyType);
        } break;
        case GpType::DOUBLE:
        {
            Map_Double(name, keyType);
        } break;
        case GpType::FLOAT:
        {
            Map_Float(name, keyType);
        } break;
        case GpType::BOOLEAN:
        {
            Map_Bool(name, keyType);
        } break;
        case GpType::UUID:
        {
            Map_Uuid(name, keyType);
        } break;
        case GpType::STRING:
        {
            Map_String(name, keyType);
        } break;
        case GpType::BLOB:
        {
            Map_Blob(name, keyType);
        } break;
        case GpType::STRUCT_SP:
        {
            GpTypeStructInfo::C::Opt::CRef typeInfoOpt = GpTypeManager::S().Find(aPropDesc.TypeUID());

            THROW_GPE_COND
            (
                typeInfoOpt.has_value(),
                [&](){return "Type struct info not found by uid '"_sv + aPropDesc.TypeUID().ToString() + "'"_sv;}
            );

            Map_StructSP(name, keyType, typeInfoOpt.value());
        } break;
        case GpType::STRUCT:
        case GpType::ENUM:
        case GpType::ENUM_FLAGS:
        case GpType::NOT_SET:[[fallthrough]];
        default:
        {
            THROW_GPE("Unsupported value type"_sv);
        }
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_TYPE_SYSTEM)
