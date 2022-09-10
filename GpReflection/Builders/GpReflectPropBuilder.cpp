#include "GpReflectPropBuilder.hpp"

#if defined(GP_USE_REFLECTION)

#include "../GpReflectManager.hpp"
#include "../GpReflectObject.hpp"

namespace GPlatform {

GpReflectPropBuilder::~GpReflectPropBuilder (void) noexcept
{
}

GpReflectModelBuilder&  GpReflectPropBuilder::DoneBuildProps (void)
{
    Check();

    THROW_COND_GP
    (
        iModelBuilder != nullptr,
        "iModelBuilder is null"_sv
    );

    iTotalSize = size_t(iOffset);

    return *iModelBuilder;
}

GpReflectPropBuilder&   GpReflectPropBuilder::UI8 (std::string_view aName)
{
    return AddProp<u_int_8, GpReflectType::U_INT_8, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::SI8 (std::string_view aName)
{
    return AddProp<s_int_8, GpReflectType::S_INT_8, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::UI16 (std::string_view aName)
{
    return AddProp<u_int_16, GpReflectType::U_INT_16, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::SI16 (std::string_view aName)
{
    return AddProp<s_int_16, GpReflectType::S_INT_16, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::UI32 (std::string_view aName)
{
    return AddProp<u_int_32, GpReflectType::U_INT_32, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::SI32 (std::string_view aName)
{
    return AddProp<s_int_32, GpReflectType::S_INT_32, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::UI64 (std::string_view aName)
{
    return AddProp<u_int_64, GpReflectType::U_INT_64, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::SI64 (std::string_view aName)
{
    return AddProp<s_int_64, GpReflectType::S_INT_64, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Double (std::string_view aName)
{
    return AddProp<double, GpReflectType::DOUBLE, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Float (std::string_view aName)
{
    return AddProp<float, GpReflectType::FLOAT, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Bool (std::string_view aName)
{
    return AddProp<bool, GpReflectType::BOOLEAN, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Uuid (std::string_view aName)
{
    return AddProp<GpUUID, GpReflectType::UUID, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::String (std::string_view aName)
{
    return AddProp<std::string, GpReflectType::STRING, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Blob (std::string_view aName)
{
    return AddProp<GpBytesArray, GpReflectType::BLOB, GpReflectContainerType::NO>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Object
(
    std::string_view        aName,
    const GpReflectModel&   aModel
)
{
    return AddProp<GpReflectObject, GpReflectType::OBJECT, GpReflectContainerType::NO>(aName, aModel);
}

GpReflectPropBuilder&   GpReflectPropBuilder::ObjectSP
(
    std::string_view        aName,
    const GpReflectModel&   aModel
)
{
    return AddProp<GpReflectObject::SP, GpReflectType::OBJECT_SP, GpReflectContainerType::NO>(aName, aModel);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_UI8 (std::string_view aName)
{
    return AddProp<u_int_8, GpReflectType::U_INT_8, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_SI8 (std::string_view aName)
{
    return AddProp<s_int_8, GpReflectType::S_INT_8, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_UI16 (std::string_view aName)
{
    return AddProp<u_int_16, GpReflectType::U_INT_16, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_SI16 (std::string_view aName)
{
    return AddProp<s_int_16, GpReflectType::S_INT_16, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_UI32 (std::string_view aName)
{
    return AddProp<u_int_32, GpReflectType::U_INT_32, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_SI32 (std::string_view aName)
{
    return AddProp<s_int_32, GpReflectType::S_INT_32, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_UI64 (std::string_view aName)
{
    return AddProp<u_int_64, GpReflectType::U_INT_64, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_SI64 (std::string_view aName)
{
    return AddProp<s_int_64, GpReflectType::S_INT_64, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_Double (std::string_view aName)
{
    return AddProp<double, GpReflectType::DOUBLE, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_Float (std::string_view aName)
{
    return AddProp<float, GpReflectType::FLOAT, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_Bool (std::string_view aName)
{
    return AddProp<bool, GpReflectType::BOOLEAN, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_Uuid (std::string_view aName)
{
    return AddProp<GpUUID, GpReflectType::UUID, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_String (std::string_view aName)
{
    return AddProp<std::string, GpReflectType::STRING, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_Blob (std::string_view aName)
{
    return AddProp<GpBytesArray, GpReflectType::BLOB, GpReflectContainerType::VECTOR>(aName);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Vec_ObjectSP
(
    std::string_view        aName,
    const GpReflectModel&   aModel
)
{
    return AddProp<GpReflectObject::SP, GpReflectType::OBJECT_SP, GpReflectContainerType::VECTOR>(aName, aModel);
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_UI8
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<u_int_8, GpReflectType::U_INT_8>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_SI8
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<s_int_8, GpReflectType::S_INT_8>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_UI16
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<u_int_16, GpReflectType::U_INT_16>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_SI16
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<s_int_16, GpReflectType::S_INT_16>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_UI32
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<u_int_32, GpReflectType::U_INT_32>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_SI32
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<s_int_32, GpReflectType::S_INT_32>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_UI64
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<u_int_64, GpReflectType::U_INT_64>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_SI64
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<s_int_64, GpReflectType::S_INT_64>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_Double
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<double, GpReflectType::DOUBLE>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_Float
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<float, GpReflectType::FLOAT>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_Bool
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<bool, GpReflectType::BOOLEAN>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_Uuid
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<GpUUID, GpReflectType::UUID>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_String
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<std::string, GpReflectType::STRING>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_Blob
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return AddPropMap<GpBytesArray, GpReflectType::BLOB>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectPropBuilder&   GpReflectPropBuilder::Map_ObjectSP
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType,
    const GpReflectModel&   aModel
)
{
    return AddPropMap<GpReflectObject::SP, GpReflectType::OBJECT_SP>(aName, aKeyType, aModel.Uid());
}

void    GpReflectPropBuilder::Check (void) const
{
    //Check if all property names are unique
    std::set<std::string_view> names;

    for (const GpReflectProp& prop: iProps)
    {
        THROW_COND_GP
        (
            names.emplace(prop.Name()).second == true,
            [&](){return "Property name '"_sv + prop.Name() + "' is not unique"_sv;}
        );
    }
}

ptrdiff_t   GpReflectPropBuilder::SCalcOffset
(
    const ptrdiff_t aCurrentOffset,
    const size_t    aAlign,
    const size_t    aSize
) noexcept
{
    char* chPtr = nullptr;
    chPtr += aCurrentOffset;

    void* ptr = chPtr;
    size_t space = std::numeric_limits<size_t>::max();

    std::align(aAlign, aSize, ptr, space);

    return std::numeric_limits<size_t>::max() - space;
}

}//namespace GPlatform

#endif//#if defined(GP_USE_REFLECTION)
