#include "GpReflectModelPropBuilder.hpp"

#include "../GpReflectObject.hpp"

namespace GPlatform {

GpReflectModelPropBuilder::~GpReflectModelPropBuilder (void) noexcept
{
}

GpReflectModelBuilder&  GpReflectModelPropBuilder::DoneBuildProps (void)
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

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::UI8 (std::string_view aName)
{
    return _AddProp<u_int_8, GpReflectType::U_INT_8, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::SI8 (std::string_view aName)
{
    return _AddProp<s_int_8, GpReflectType::S_INT_8, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::UI16 (std::string_view aName)
{
    return _AddProp<u_int_16, GpReflectType::U_INT_16, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::SI16 (std::string_view aName)
{
    return _AddProp<s_int_16, GpReflectType::S_INT_16, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::UI32 (std::string_view aName)
{
    return _AddProp<u_int_32, GpReflectType::U_INT_32, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::SI32 (std::string_view aName)
{
    return _AddProp<s_int_32, GpReflectType::S_INT_32, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::UI64 (std::string_view aName)
{
    return _AddProp<u_int_64, GpReflectType::U_INT_64, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::SI64 (std::string_view aName)
{
    return _AddProp<s_int_64, GpReflectType::S_INT_64, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Double (std::string_view aName)
{
    return _AddProp<double, GpReflectType::DOUBLE, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Float (std::string_view aName)
{
    return _AddProp<float, GpReflectType::FLOAT, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Bool (std::string_view aName)
{
    return _AddProp<bool, GpReflectType::BOOLEAN, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::UUID (std::string_view aName)
{
    return _AddProp<GpUUID, GpReflectType::UUID, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::String (std::string_view aName)
{
    return _AddProp<std::string, GpReflectType::STRING, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::BLOB (std::string_view aName)
{
    return _AddProp<GpBytesArray, GpReflectType::BLOB, GpReflectContainerType::NO>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Object
(
    std::string_view        aName,
    const GpReflectModel&   aModel
)
{
    return _AddProp<GpReflectObject, GpReflectType::OBJECT, GpReflectContainerType::NO>(aName, aModel);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::ObjectSP
(
    std::string_view        aName,
    const GpReflectModel&   aModel
)
{
    return _AddProp<GpReflectObject::SP, GpReflectType::OBJECT_SP, GpReflectContainerType::NO>(aName, aModel);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_UI8 (std::string_view aName)
{
    return _AddProp<u_int_8, GpReflectType::U_INT_8, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_SI8 (std::string_view aName)
{
    return _AddProp<s_int_8, GpReflectType::S_INT_8, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_UI16 (std::string_view aName)
{
    return _AddProp<u_int_16, GpReflectType::U_INT_16, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_SI16 (std::string_view aName)
{
    return _AddProp<s_int_16, GpReflectType::S_INT_16, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_UI32 (std::string_view aName)
{
    return _AddProp<u_int_32, GpReflectType::U_INT_32, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_SI32 (std::string_view aName)
{
    return _AddProp<s_int_32, GpReflectType::S_INT_32, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_UI64 (std::string_view aName)
{
    return _AddProp<u_int_64, GpReflectType::U_INT_64, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_SI64 (std::string_view aName)
{
    return _AddProp<s_int_64, GpReflectType::S_INT_64, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_Double (std::string_view aName)
{
    return _AddProp<double, GpReflectType::DOUBLE, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_Float (std::string_view aName)
{
    return _AddProp<float, GpReflectType::FLOAT, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_UUID (std::string_view aName)
{
    return _AddProp<GpUUID, GpReflectType::UUID, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_String (std::string_view aName)
{
    return _AddProp<std::string, GpReflectType::STRING, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_BLOB (std::string_view aName)
{
    return _AddProp<GpBytesArray, GpReflectType::BLOB, GpReflectContainerType::VECTOR>(aName);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_Object
(
    std::string_view        aName,
    const GpReflectModel&   aModel
)
{
    return _AddProp<std::nullopt_t, GpReflectType::OBJECT, GpReflectContainerType::VECTOR_WRAP>(aName, aModel);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Vec_ObjectSP
(
    std::string_view        aName,
    const GpReflectModel&   aModel
)
{
    return _AddProp<GpReflectObject::SP, GpReflectType::OBJECT_SP, GpReflectContainerType::VECTOR>(aName, aModel);
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_UI8
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<u_int_8, GpReflectType::U_INT_8>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_SI8
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<s_int_8, GpReflectType::S_INT_8>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_UI16
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<u_int_16, GpReflectType::U_INT_16>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_SI16
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<s_int_16, GpReflectType::S_INT_16>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_UI32
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<u_int_32, GpReflectType::U_INT_32>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_SI32
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<s_int_32, GpReflectType::S_INT_32>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_UI64
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<u_int_64, GpReflectType::U_INT_64>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_SI64
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<s_int_64, GpReflectType::S_INT_64>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_Double
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<double, GpReflectType::DOUBLE>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_Float
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<float, GpReflectType::FLOAT>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_UUID
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<GpUUID, GpReflectType::UUID>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_String
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<std::string, GpReflectType::STRING>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_BLOB
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType
)
{
    return _AddPropMap<GpBytesArray, GpReflectType::BLOB>(aName, aKeyType, GpUUID::CE_Zero());
}

GpReflectModelPropBuilder&  GpReflectModelPropBuilder::Map_ObjectSP
(
    std::string_view        aName,
    GpReflectType::EnumT    aKeyType,
    const GpReflectModel&   aModel
)
{
    return _AddPropMap<GpReflectObject::SP, GpReflectType::OBJECT_SP>(aName, aKeyType, aModel.Uid());
}

void    GpReflectModelPropBuilder::Check (void) const
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

ptrdiff_t   GpReflectModelPropBuilder::SCalcOffset
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

    return ptrdiff_t(std::numeric_limits<size_t>::max() - space);
}

}// namespace GPlatform
