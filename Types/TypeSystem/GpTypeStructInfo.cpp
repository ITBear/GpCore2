#include "GpTypeStructInfo.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructBase.hpp"

namespace GPlatform {

GpTypeStructInfo::GpTypeStructInfo
(
    const GpUUID&           aUID,
    const GpUUID&           aBaseUID,
    std::string&&           aName,
    PropsT&&                aProps,
    const GpUUID&           aGroupID,
    GpTypeStructFactory::SP aFactory,
    const size_t            aAlign,
    const size_t            aSize
) noexcept:
iUID(aUID),
iBaseUID(aBaseUID),
iName(std::move(aName)),
iProps(std::move(aProps)),
iGroupID(aGroupID),
iFactory(std::move(aFactory)),
iAlign(aAlign),
iSize(aSize)
{
}

GpTypeStructInfo::GpTypeStructInfo (const GpTypeStructInfo& aTypeInfo):
iUID(aTypeInfo.iUID),
iBaseUID(aTypeInfo.iBaseUID),
iName(aTypeInfo.iName),
iProps(aTypeInfo.iProps),
iGroupID(aTypeInfo.iGroupID),
iFactory(aTypeInfo.iFactory),
iAlign(aTypeInfo.iAlign),
iSize(aTypeInfo.iSize),
iSizeOfProps(aTypeInfo.iSizeOfProps)
{
}

GpTypeStructInfo::GpTypeStructInfo (GpTypeStructInfo&& aTypeInfo) noexcept:
iUID(std::move(aTypeInfo.iUID)),
iBaseUID(std::move(aTypeInfo.iBaseUID)),
iName(std::move(aTypeInfo.iName)),
iProps(std::move(aTypeInfo.iProps)),
iGroupID(std::move(aTypeInfo.iGroupID)),
iFactory(std::move(aTypeInfo.iFactory)),
iAlign(std::move(aTypeInfo.iAlign)),
iSize(std::move(aTypeInfo.iSize)),
iSizeOfProps(std::move(aTypeInfo.iSizeOfProps))
{
}

GpTypeStructInfo::~GpTypeStructInfo (void) noexcept
{
}

GpTypeStructInfo&   GpTypeStructInfo::operator= (const GpTypeStructInfo& aTypeInfo)
{
    iUID        = aTypeInfo.iUID;
    iBaseUID    = aTypeInfo.iBaseUID;
    iName       = aTypeInfo.iName;
    iProps      = aTypeInfo.iProps;
    iFactory    = aTypeInfo.iFactory;
    iGroupID    = aTypeInfo.iGroupID;
    iAlign      = aTypeInfo.iAlign;
    iSize       = aTypeInfo.iSize;
    iSizeOfProps= aTypeInfo.iSizeOfProps;

    return *this;
}

GpTypeStructInfo&   GpTypeStructInfo::operator= (GpTypeStructInfo&& aTypeInfo) noexcept
{
    iUID        = std::move(aTypeInfo.iUID);
    iBaseUID    = std::move(aTypeInfo.iBaseUID);
    iName       = std::move(aTypeInfo.iName);
    iProps      = std::move(aTypeInfo.iProps);
    iFactory    = std::move(aTypeInfo.iFactory);
    iGroupID    = std::move(aTypeInfo.iGroupID);
    iAlign      = std::move(aTypeInfo.iAlign);
    iSize       = std::move(aTypeInfo.iSize);
    iSizeOfProps= std::move(aTypeInfo.iSizeOfProps);

    return *this;
}

const GpTypePropInfo&   GpTypeStructInfo::Prop (std::string_view aName) const
{
    //TODO: implement with map

    for (const GpTypePropInfo& propInfo: iProps)
    {
        if (propInfo.Name() == aName)
        {
            return propInfo;
        }
    }

    THROW_GPE("Property was not found by name '"_sv + aName + "'"_sv);
}

size_t  GpTypeStructInfo::SizeOfProps (void) const
{
    if (iSizeOfProps > 0)
    {
        return iSizeOfProps;
    }

    size_t maxOffset            = 0;
    size_t maxOffsetPropSize    = 0;

    for (const GpTypePropInfo& p: Props())
    {
        const size_t offset = p.Offset();

        if (offset > maxOffset)
        {
            maxOffset           = offset;
            maxOffsetPropSize   = p.Size();
        }
    }

    iSizeOfProps = maxOffset + maxOffsetPropSize;

    return iSizeOfProps;
}

/*std::string   GpTypeStructInfo::SEcho (const GpTypeStructBase& aStruct)
{
    std::string res;
    res.reserve(1024);

    _SEcho(aStruct, res, 0);

    return res;
}

void    GpTypeStructInfo::_SEcho
(
    const GpTypeStructBase& aStruct,
    std::string&            aStrOut,
    const size_t            aLevel
)
{
    const GpTypeStructInfo& typeInfo = aStruct.TypeInfo();

    const size_t indentSize = 4;
    const std::string indent1(aLevel*indentSize, ' ');
    const std::string indent2(aLevel*indentSize + indentSize, ' ');
    aStrOut.append(indent1).append("Struct: '"_sv).append(typeInfo.Name()).append("'"_sv);

    const auto& props = typeInfo.Props();

    if (props.size() == 0)
    {
        aStrOut.append(", no props."_sv);
        return;
    }

    aStrOut.append(", props:\n"_sv);

    for (const GpTypePropInfo& propInfo: props)
    {
        aStrOut.append(indent2).append("'"_sv).append(propInfo.Name()).append("'"_sv);

        const GpTypeContainer::EnumT containerType = propInfo.Container();

        if (containerType == GpTypeContainer::NO)
        {
            aStrOut.append("["_sv).append(GpType::SToString(propInfo.Type())).append("] = "_sv);
            _SEchoValue(aStruct.DataPtr(), aStrOut, propInfo, aLevel);
        } else if (containerType == GpTypeContainer::VECTOR)
        {
            aStrOut.append("[Vector<"_sv).append(GpType::SToString(propInfo.Type())).append(">] = "_sv);
        } else if (containerType == GpTypeContainer::LIST)
        {
            aStrOut.append("[List<"_sv).append(GpType::SToString(propInfo.Type())).append(">] = "_sv);
        } else if (containerType == GpTypeContainer::SET)
        {
            aStrOut.append("[Set<"_sv).append(GpType::SToString(propInfo.Type())).append(">] = "_sv);
        } else if (containerType == GpTypeContainer::MAP)
        {
            aStrOut.append("[Map<"_sv).append(GpType::SToString(propInfo.ContainerKeyType()))
                    .append(", "_sv).append(GpType::SToString(propInfo.Type())).append(">] = "_sv);
        }

        aStrOut.append("\n"_sv);
    }
}

void    GpTypeStructInfo::_SEchoValue
(
    const void*             aStructDataPtr,
    std::string&            aStrOut,
    const GpTypePropInfo&   aPropInfo,
    const size_t            aLevel
)
{
    switch (aPropInfo.Type())
    {
        case GpType::U_INT_8:
        {
            aStrOut.append(StrOps::SFromUI64(aPropInfo.Value_UInt8(aStructDataPtr)));
        } break;
        case GpType::S_INT_8:
        {
            aStrOut.append(StrOps::SFromSI64(aPropInfo.Value_SInt8(aStructDataPtr)));
        } break;
        case GpType::U_INT_16:
        {
            aStrOut.append(StrOps::SFromUI64(aPropInfo.Value_UInt16(aStructDataPtr)));
        } break;
        case GpType::S_INT_16:
        {
            aStrOut.append(StrOps::SFromSI64(aPropInfo.Value_SInt16(aStructDataPtr)));
        } break;
        case GpType::U_INT_32:
        {
            aStrOut.append(StrOps::SFromUI64(aPropInfo.Value_UInt32(aStructDataPtr)));
        } break;
        case GpType::S_INT_32:
        {
            aStrOut.append(StrOps::SFromSI64(aPropInfo.Value_SInt32(aStructDataPtr)));
        } break;
        case GpType::U_INT_64:
        {
            aStrOut.append(StrOps::SFromUI64(aPropInfo.Value_UInt64(aStructDataPtr)));
        } break;
        case GpType::S_INT_64:   [[fallthrough]];
        case GpType::UNIX_TS_S:  [[fallthrough]];
        case GpType::UNIX_TS_MS:
        {
            aStrOut.append(StrOps::SFromSI64(aPropInfo.Value_SInt64(aStructDataPtr)));
        } break;
        case GpType::DOUBLE:
        {
            aStrOut.append(StrOps::SFromDouble(aPropInfo.Value_Double(aStructDataPtr)));
        } break;
        case GpType::FLOAT:
        {
            aStrOut.append(StrOps::SFromDouble(double(aPropInfo.Value_Float(aStructDataPtr))));
        } break;
        case GpType::BOOLEAN:
        {
            aStrOut.append(aPropInfo.Value_Bool(aStructDataPtr) ? "true"_sv : "false"_sv);
        } break;
        case GpType::UUID:
        {
            aStrOut.append(aPropInfo.Value_UUID(aStructDataPtr).ToString());
        } break;
        case GpType::STRING:
        {
            aStrOut.append(aPropInfo.Value_String(aStructDataPtr));
        } break;
        case GpType::BLOB:
        {
            aStrOut.append(StrOps::SFromBytesHex(aPropInfo.Value_BLOB(aStructDataPtr)));
        } break;
        case GpType::STRUCT:
        {
            _SEcho(aPropInfo.Value_Struct(aStructDataPtr), aStrOut, aLevel + 1);
        } break;
        case GpType::STRUCT_SP:
        {
            const GpTypeStructBase::SP& structSP = aPropInfo.Value_StructSP(aStructDataPtr);
            if (structSP.IsNotNULL())
            {
                _SEcho(structSP.VCn(), aStrOut, aLevel + 1);
            } else
            {
                aStrOut.append("NULL"_sv);
            }
        } break;
        case GpType::ENUM:
        {
            aStrOut.append(aPropInfo.Value_Enum(aStructDataPtr).ToString());
        } break;
        case GpType::ENUM_FLAGS:
        {
            aStrOut.append(aPropInfo.Value_EnumFlags(aStructDataPtr).Echo());
        } break;
        case GpType::NOT_SET:
        {
            THROW_GPE("Type "_sv + GpType::SToString(aPropInfo.Type()));
        } break;
        default:
        {
            THROW_GPE("Unsupported type "_sv + GpType::SToString(aPropInfo.Type()));
        }
    }
}*/

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
