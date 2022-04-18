#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructBase.hpp"
#include "GpTypePropInfoGetter.hpp"

namespace GPlatform {

template<typename TypeStructBaseT,
         typename VisitorT>
class GpTypeStructVisitor
{
public:
    static_assert(   std::is_same_v<TypeStructBaseT, const GpTypeStructBase>
                  || std::is_same_v<TypeStructBaseT, GpTypeStructBase>);

    using StructDataPtrT = std::conditional_t<std::is_const_v<TypeStructBaseT>, const void*, void*>;

public:
    void                Visit               (const GpTypeStructInfo&        aTypeInfo,
                                             StructDataPtrT                 aStructDataPtr,
                                             typename VisitorT::VisitCtx&   aCtx);

private:
    void                VisitValue          (StructDataPtrT                 aStructDataPtr,
                                             const GpTypePropInfo&          aPropInfo,
                                             typename VisitorT::VisitCtx&   aCtx);

    void                VisitValueVec       (StructDataPtrT                 aStructDataPtr,
                                             const GpTypePropInfo&          aPropInfo,
                                             typename VisitorT::VisitCtx&   aCtx);

    void                VisitValueList      (StructDataPtrT                 aStructDataPtr,
                                             const GpTypePropInfo&          aPropInfo,
                                             typename VisitorT::VisitCtx&   aCtx);

    void                VisitValueSet       (StructDataPtrT                 aStructDataPtr,
                                             const GpTypePropInfo&          aPropInfo,
                                             typename VisitorT::VisitCtx&   aCtx);

    template<typename ValGetterT>
    void                ProcessContainer    (StructDataPtrT                 aStructDataPtr,
                                             const GpTypePropInfo&          aPropInfo,
                                             typename VisitorT::VisitCtx&   aCtx);

    void                VisitValueMap       (StructDataPtrT                 aStructDataPtr,
                                             const GpTypePropInfo&          aPropInfo,
                                             typename VisitorT::VisitCtx&   aCtx);

    template<typename                       KeyT,
             template<typename...> class    ValGetterT>
    void                ProcessMapK         (StructDataPtrT                     aStructDataPtr,
                                             const GpTypePropInfo&              aPropInfo,
                                             typename VisitorT::VisitCtx&       aCtx,
                                             typename VisitorT::VisitMapCtx&    aCtxMap);
};

template<typename TypeStructBaseT,
         typename VisitorT>
void    GpTypeStructVisitor<TypeStructBaseT, VisitorT>::Visit
(
    const GpTypeStructInfo&         aTypeInfo,
    StructDataPtrT                  aStructDataPtr,
    typename VisitorT::VisitCtx&    aCtx
)
{
    if (aCtx.OnVisitBegin(aTypeInfo) == false)
    {
        return;
    }

    const auto& props = aTypeInfo.Props();

    for (const GpTypePropInfo& propInfo: props)
    {
        const GpTypeContainer::EnumT containerType = propInfo.Container();
        try
        {
            if (containerType == GpTypeContainer::NO)
            {
                VisitValue(aStructDataPtr, propInfo, aCtx);
            } else if (containerType == GpTypeContainer::VECTOR)
            {
                VisitValueVec(aStructDataPtr, propInfo, aCtx);
            } else if (containerType == GpTypeContainer::LIST)
            {
                VisitValueList(aStructDataPtr, propInfo, aCtx);
            } else if (containerType == GpTypeContainer::SET)
            {
                VisitValueSet(aStructDataPtr, propInfo, aCtx);
            } else if (containerType == GpTypeContainer::MAP)
            {
                VisitValueMap(aStructDataPtr, propInfo, aCtx);
            }
        } catch (const std::exception& e)
        {
            THROW_GPE("Failed to visit type. Struct "_sv + aTypeInfo.Name() + "."_sv + propInfo.Name() + "\nReason:\n"_sv + e.what());
        } catch (...)
        {
            THROW_GPE("Failed to visit type. Struct "_sv + aTypeInfo.Name() + "."_sv + propInfo.Name() + "\nReason:\nUnknown exception"_sv);
        }
    }

    aCtx.OnVisitEnd(aTypeInfo);
}

template<typename TypeStructBaseT,
         typename VisitorT>
void    GpTypeStructVisitor<TypeStructBaseT, VisitorT>::VisitValue
(
    StructDataPtrT                  aStructDataPtr,
    const GpTypePropInfo&           aPropInfo,
    typename VisitorT::VisitCtx&    aCtx
)
{
    typename VisitorT::VisitValueCtx visitValueCtx;

    if (visitValueCtx.OnVisitBegin(aStructDataPtr, aPropInfo, aCtx) == false)
    {
        return;
    }

    const GpType::EnumT propType = aPropInfo.Type();

    switch (propType)
    {
        case GpType::U_INT_8:       visitValueCtx.Value_UInt8(aStructDataPtr, aPropInfo, aCtx);     break;
        case GpType::S_INT_8:       visitValueCtx.Value_SInt8(aStructDataPtr, aPropInfo, aCtx);     break;
        case GpType::U_INT_16:      visitValueCtx.Value_UInt16(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::S_INT_16:      visitValueCtx.Value_SInt16(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::U_INT_32:      visitValueCtx.Value_UInt32(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::S_INT_32:      visitValueCtx.Value_SInt32(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::U_INT_64:      visitValueCtx.Value_UInt64(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::S_INT_64:   [[fallthrough]];
        case GpType::UNIX_TS_S:  [[fallthrough]];
        case GpType::UNIX_TS_MS:    visitValueCtx.Value_SInt64(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::DOUBLE:        visitValueCtx.Value_Double(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::FLOAT:         visitValueCtx.Value_Float(aStructDataPtr, aPropInfo, aCtx);     break;
        case GpType::BOOLEAN:       visitValueCtx.Value_Bool(aStructDataPtr, aPropInfo, aCtx);      break;
        case GpType::UUID:          visitValueCtx.Value_UUID(aStructDataPtr, aPropInfo, aCtx);      break;
        case GpType::STRING:        visitValueCtx.Value_String(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::BLOB:          visitValueCtx.Value_BLOB(aStructDataPtr, aPropInfo, aCtx);      break;
        case GpType::STRUCT:        visitValueCtx.Value_Struct(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::STRUCT_SP:     visitValueCtx.Value_StructSP(aStructDataPtr, aPropInfo, aCtx);  break;
        case GpType::ENUM:          visitValueCtx.Value_Enum(aStructDataPtr, aPropInfo, aCtx);      break;
        case GpType::ENUM_FLAGS:    visitValueCtx.Value_EnumFlags(aStructDataPtr, aPropInfo, aCtx); break;
        case GpType::NOT_SET:       THROW_GPE("Type "_sv + GpType::SToString(aPropInfo.Type()));    break;
        default:                    THROW_GPE("Unsupported type "_sv + GpType::SToString(aPropInfo.Type()));
    }

    visitValueCtx.OnVisitEnd(aStructDataPtr, aPropInfo, aCtx);
}

template<typename TypeStructBaseT,
         typename VisitorT>
void    GpTypeStructVisitor<TypeStructBaseT, VisitorT>::VisitValueVec
(
    StructDataPtrT                  aStructDataPtr,
    const GpTypePropInfo&           aPropInfo,
    typename VisitorT::VisitCtx&    aCtx
)
{
    ProcessContainer<GpTypePropInfoGetter_Vec>(aStructDataPtr, aPropInfo, aCtx);
}

template<typename TypeStructBaseT,
         typename VisitorT>
void    GpTypeStructVisitor<TypeStructBaseT, VisitorT>::VisitValueList
(
    StructDataPtrT                  aStructDataPtr,
    const GpTypePropInfo&           aPropInfo,
    typename VisitorT::VisitCtx&    aCtx
)
{
    ProcessContainer<GpTypePropInfoGetter_List>(aStructDataPtr, aPropInfo, aCtx);
}

template<typename TypeStructBaseT,
         typename VisitorT>
void    GpTypeStructVisitor<TypeStructBaseT, VisitorT>::VisitValueSet
(
    StructDataPtrT                  aStructDataPtr,
    const GpTypePropInfo&           aPropInfo,
    typename VisitorT::VisitCtx&    aCtx
)
{
    ProcessContainer<GpTypePropInfoGetter_Set>(aStructDataPtr, aPropInfo, aCtx);
}

template<typename TypeStructBaseT,
         typename VisitorT>
template<typename ValGetterT>
void    GpTypeStructVisitor<TypeStructBaseT, VisitorT>::ProcessContainer
(
    StructDataPtrT                  aStructDataPtr,
    const GpTypePropInfo&           aPropInfo,
    typename VisitorT::VisitCtx&    aCtx
)
{
    typename VisitorT::VisitContainerCtx visitContainerCtx;

    if (visitContainerCtx.OnVisitBegin(aStructDataPtr, aPropInfo, aCtx) == false)
    {
        return;
    }

    const GpType::EnumT propType = aPropInfo.Type();

    switch (propType)
    {
        case GpType::U_INT_8:       visitContainerCtx.template Value_UInt8<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::S_INT_8:       visitContainerCtx.template Value_SInt8<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::U_INT_16:      visitContainerCtx.template Value_UInt16<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);   break;
        case GpType::S_INT_16:      visitContainerCtx.template Value_SInt16<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);   break;
        case GpType::U_INT_32:      visitContainerCtx.template Value_UInt32<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);   break;
        case GpType::S_INT_32:      visitContainerCtx.template Value_SInt32<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);   break;
        case GpType::U_INT_64:      visitContainerCtx.template Value_UInt64<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);   break;
        case GpType::S_INT_64:   [[fallthrough]];
        case GpType::UNIX_TS_S:  [[fallthrough]];
        case GpType::UNIX_TS_MS:    visitContainerCtx.template Value_SInt64<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);   break;
        case GpType::DOUBLE:        visitContainerCtx.template Value_Double<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);   break;
        case GpType::FLOAT:         visitContainerCtx.template Value_Float<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);    break;
        case GpType::BOOLEAN:       visitContainerCtx.template Value_Bool<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);     break;
        case GpType::UUID:          visitContainerCtx.template Value_UUID<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);     break;
        case GpType::STRING:        visitContainerCtx.template Value_String<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);   break;
        case GpType::BLOB:          visitContainerCtx.template Value_BLOB<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);     break;
        case GpType::STRUCT:        visitContainerCtx.template Value_Struct<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);   break;
        case GpType::STRUCT_SP:     visitContainerCtx.template Value_StructSP<ValGetterT>(aStructDataPtr, aPropInfo, aCtx); break;
        case GpType::ENUM:          visitContainerCtx.template Value_Enum<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);     break;
        case GpType::ENUM_FLAGS:    visitContainerCtx.template Value_EnumFlags<ValGetterT>(aStructDataPtr, aPropInfo, aCtx);break;
        case GpType::NOT_SET:       THROW_GPE("Type "_sv + GpType::SToString(aPropInfo.Type()));                            break;
        default:                    THROW_GPE("Unsupported type "_sv + GpType::SToString(aPropInfo.Type()));
    }

    visitContainerCtx.OnVisitEnd(aStructDataPtr, aPropInfo, aCtx);
}

template<typename TypeStructBaseT,
         typename VisitorT>
void    GpTypeStructVisitor<TypeStructBaseT, VisitorT>::VisitValueMap
(
    StructDataPtrT                  aStructDataPtr,
    const GpTypePropInfo&           aPropInfo,
    typename VisitorT::VisitCtx&    aCtx
)
{
    typename VisitorT::VisitMapCtx visitMapCtx;

    if (visitMapCtx.OnVisitBegin(aStructDataPtr, aPropInfo, aCtx) == false)
    {
        return;
    }

    const GpType::EnumT keyType = aPropInfo.ContainerKeyType();

    switch (keyType)
    {
        case GpType::U_INT_8:       ProcessMapK<u_int_8, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);       break;
        case GpType::S_INT_8:       ProcessMapK<s_int_8, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);       break;
        case GpType::U_INT_16:      ProcessMapK<u_int_16, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);      break;
        case GpType::S_INT_16:      ProcessMapK<s_int_16, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);      break;
        case GpType::U_INT_32:      ProcessMapK<u_int_32, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);      break;
        case GpType::S_INT_32:      ProcessMapK<s_int_32, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);      break;
        case GpType::U_INT_64:      ProcessMapK<u_int_64, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);      break;
        case GpType::S_INT_64:   [[fallthrough]];
        case GpType::UNIX_TS_S:  [[fallthrough]];
        case GpType::UNIX_TS_MS:    ProcessMapK<s_int_64, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);      break;
        case GpType::DOUBLE:        ProcessMapK<double, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);            break;
        case GpType::FLOAT:         ProcessMapK<float, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);         break;
        case GpType::BOOLEAN:       THROW_GPE("bool is not supported as map key"_sv);                                                   break;
        case GpType::UUID:          ProcessMapK<GpUUID, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);            break;
        case GpType::STRING:        ProcessMapK<std::string, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);   break;
        case GpType::BLOB:          ProcessMapK<GpBytesArray, GpTypePropInfoGetter_Map>(aStructDataPtr, aPropInfo, aCtx, visitMapCtx);  break;
        case GpType::STRUCT:        THROW_GPE("struct is not supported as map key"_sv);                                                 break;
        case GpType::STRUCT_SP:     THROW_GPE("struct::sp is not supported as map key"_sv);                                             break;
        case GpType::ENUM:          THROW_GPE("enums is not supported as map key"_sv);                                                  break;
        case GpType::ENUM_FLAGS:    THROW_GPE("enum flags is not supported as map key"_sv);                                             break;
        case GpType::NOT_SET:       THROW_GPE("Type "_sv + GpType::SToString(aPropInfo.Type()));                                        break;
        default:                    THROW_GPE("Unsupported type "_sv + GpType::SToString(aPropInfo.Type()));
    }

    visitMapCtx.OnVisitEnd(aStructDataPtr, aPropInfo, aCtx);
}

template<typename TypeStructBaseT,
         typename VisitorT>
template<typename                       KeyT,
         template<typename...> class    ValGetterT>
void    GpTypeStructVisitor<TypeStructBaseT, VisitorT>::ProcessMapK
(
    StructDataPtrT                  aStructDataPtr,
    const GpTypePropInfo&           aPropInfo,
    typename VisitorT::VisitCtx&    aCtx,
    typename VisitorT::VisitMapCtx& aCtxMap
)
{
    const GpType::EnumT propType = aPropInfo.Type();

    switch (propType)
    {
        case GpType::U_INT_8:       aCtxMap.template K_UInt8<KeyT,   u_int_8, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                 break;
        case GpType::S_INT_8:       aCtxMap.template K_SInt8<KeyT,   s_int_8, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                 break;
        case GpType::U_INT_16:      aCtxMap.template K_UInt16<KeyT, u_int_16, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                 break;
        case GpType::S_INT_16:      aCtxMap.template K_SInt16<KeyT, s_int_16, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                 break;
        case GpType::U_INT_32:      aCtxMap.template K_UInt32<KeyT, u_int_32, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                 break;
        case GpType::S_INT_32:      aCtxMap.template K_SInt32<KeyT, s_int_32, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                 break;
        case GpType::U_INT_64:      aCtxMap.template K_UInt64<KeyT, u_int_64, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                 break;
        case GpType::S_INT_64:   [[fallthrough]];
        case GpType::UNIX_TS_S:  [[fallthrough]];
        case GpType::UNIX_TS_MS:    aCtxMap.template K_SInt64<KeyT, s_int_64, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                 break;
        case GpType::DOUBLE:        aCtxMap.template K_Double<KeyT, double, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                   break;
        case GpType::FLOAT:         aCtxMap.template K_Float<KeyT, float, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                     break;
        case GpType::BOOLEAN:       aCtxMap.template K_Bool<KeyT, bool, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                       break;
        case GpType::UUID:          aCtxMap.template K_UUID<KeyT, GpUUID, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);                     break;
        case GpType::STRING:        aCtxMap.template K_String<KeyT, std::string, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);              break;
        case GpType::BLOB:          aCtxMap.template K_BLOB<KeyT, GpBytesArray, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);               break;
        case GpType::STRUCT:        THROW_GPE("struct is not supported as map key"_sv);                                                     break;
        case GpType::STRUCT_SP:     aCtxMap.template K_StructSP<KeyT, GpTypeStructBase::SP, ValGetterT>(aStructDataPtr, aPropInfo, aCtx);   break;
        case GpType::ENUM:          THROW_GPE("enums is not supported as map key"_sv);                                                      break;
        case GpType::ENUM_FLAGS:    THROW_GPE("enum flags is not supported as map key"_sv);                                                 break;
        case GpType::NOT_SET:       THROW_GPE("Type "_sv + GpType::SToString(aPropInfo.Type()));                                            break;
        default:                    THROW_GPE("Unsupported type "_sv + GpType::SToString(aPropInfo.Type()));
    }
}

}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM
