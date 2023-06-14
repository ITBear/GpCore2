#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectObject.hpp"
#include "GpReflectPropGetter.hpp"

namespace GPlatform {

template<typename T,
         typename VisitorT>
class GpReflectVisitor
{
public:
    static_assert(   std::is_same_v<T, const GpReflectObject>
                  || std::is_same_v<T, GpReflectObject>);

    using DataPtrT = std::conditional_t<std::is_const_v<T>, const void*, void*>;

public:
    void                Visit               (const GpReflectModel&          aModel,
                                             DataPtrT                       aDataPtr,
                                             typename VisitorT::VisitCtx&   aCtx);

private:
    void                VisitValue          (DataPtrT                       aDataPtr,
                                             const GpReflectProp&           aProp,
                                             typename VisitorT::VisitCtx&   aCtx);

    void                VisitValueVec       (DataPtrT                       aDataPtr,
                                             const GpReflectProp&           aProp,
                                             typename VisitorT::VisitCtx&   aCtx);

    template<typename ValGetterT>
    void                ProcessContainer    (DataPtrT                       aDataPtr,
                                             const GpReflectProp&           aProp,
                                             typename VisitorT::VisitCtx&   aCtx);

    void                VisitValueMap       (DataPtrT                       aDataPtr,
                                             const GpReflectProp&           aProp,
                                             typename VisitorT::VisitCtx&   aCtx);

    template<typename                       KeyT,
             template<typename...> class    ValGetterT>
    void                ProcessMapK         (DataPtrT                           aDataPtr,
                                             const GpReflectProp&               aProp,
                                             typename VisitorT::VisitCtx&       aCtx,
                                             typename VisitorT::VisitMapCtx&    aCtxMap);
};

template<typename T,
         typename VisitorT>
void    GpReflectVisitor<T, VisitorT>::Visit
(
    const GpReflectModel&           aModel,
    DataPtrT                        aDataPtr,
    typename VisitorT::VisitCtx&    aCtx
)
{
    if (aCtx.OnVisitBegin(aModel) == false)
    {
        return;
    }

    const auto& props = aModel.Props();

    for (const GpReflectProp& prop: props)
    {
        const GpReflectContainerType::EnumT containerType = prop.Container();
        try
        {
            if (containerType == GpReflectContainerType::NO)
            {
                VisitValue(aDataPtr, prop, aCtx);
            } else if (containerType == GpReflectContainerType::VECTOR)
            {
                VisitValueVec(aDataPtr, prop, aCtx);
            } else if (containerType == GpReflectContainerType::MAP)
            {
                VisitValueMap(aDataPtr, prop, aCtx);
            }
        } catch (const std::exception& e)
        {
            THROW_GP(u8"Failed to visit propetry "_sv + aModel.Name() + u8"."_sv + prop.Name() + u8"\nReason:\n"_sv + e.what());
        }
    }

    aCtx.OnVisitEnd(aModel);
}

template<typename T,
         typename VisitorT>
void    GpReflectVisitor<T, VisitorT>::VisitValue
(
    DataPtrT                        aDataPtr,
    const GpReflectProp&            aProp,
    typename VisitorT::VisitCtx&    aCtx
)
{
    typename VisitorT::VisitValueCtx visitValueCtx;

    if (visitValueCtx.OnVisitBegin(aDataPtr, aProp, aCtx) == false)
    {
        return;
    }

    const GpReflectType::EnumT propType = aProp.Type();

    switch (propType)
    {
        case GpReflectType::U_INT_8:    visitValueCtx.Value_UInt8(aDataPtr, aProp, aCtx);               break;
        case GpReflectType::S_INT_8:    visitValueCtx.Value_SInt8(aDataPtr, aProp, aCtx);               break;
        case GpReflectType::U_INT_16:   visitValueCtx.Value_UInt16(aDataPtr, aProp, aCtx);              break;
        case GpReflectType::S_INT_16:   visitValueCtx.Value_SInt16(aDataPtr, aProp, aCtx);              break;
        case GpReflectType::U_INT_32:   visitValueCtx.Value_UInt32(aDataPtr, aProp, aCtx);              break;
        case GpReflectType::S_INT_32:   visitValueCtx.Value_SInt32(aDataPtr, aProp, aCtx);              break;
        case GpReflectType::U_INT_64:   visitValueCtx.Value_UInt64(aDataPtr, aProp, aCtx);              break;
        case GpReflectType::S_INT_64:   visitValueCtx.Value_SInt64(aDataPtr, aProp, aCtx);              break;
        case GpReflectType::DOUBLE:     visitValueCtx.Value_Double(aDataPtr, aProp, aCtx);              break;
        case GpReflectType::FLOAT:      visitValueCtx.Value_Float(aDataPtr, aProp, aCtx);               break;
        case GpReflectType::BOOLEAN:    visitValueCtx.Value_Bool(aDataPtr, aProp, aCtx);                break;
        case GpReflectType::UUID:       visitValueCtx.Value_UUID(aDataPtr, aProp, aCtx);                break;
        case GpReflectType::STRING:     visitValueCtx.Value_String(aDataPtr, aProp, aCtx);              break;
        case GpReflectType::BLOB:       visitValueCtx.Value_BLOB(aDataPtr, aProp, aCtx);                break;
        case GpReflectType::OBJECT:     visitValueCtx.Value_Object(aDataPtr, aProp, aCtx);              break;
        case GpReflectType::OBJECT_SP:  visitValueCtx.Value_ObjectSP(aDataPtr, aProp, aCtx);            break;
        case GpReflectType::ENUM:       visitValueCtx.Value_Enum(aDataPtr, aProp, aCtx);                break;
        case GpReflectType::ENUM_FLAGS: visitValueCtx.Value_EnumFlags(aDataPtr, aProp, aCtx);           break;
        case GpReflectType::NOT_SET:    THROW_GP(u8"Type "_sv + GpReflectType::SToString(aProp.Type()));    break;
        default:                        THROW_GP(u8"Unsupported type "_sv + GpReflectType::SToString(aProp.Type()));
    }

    visitValueCtx.OnVisitEnd(aDataPtr, aProp, aCtx);
}

template<typename T,
         typename VisitorT>
void    GpReflectVisitor<T, VisitorT>::VisitValueVec
(
    DataPtrT                        aDataPtr,
    const GpReflectProp&            aProp,
    typename VisitorT::VisitCtx&    aCtx
)
{
    ProcessContainer<GpReflectPropGetter_Vec>(aDataPtr, aProp, aCtx);
}

template<typename T,
         typename VisitorT>
template<typename ValGetterT>
void    GpReflectVisitor<T, VisitorT>::ProcessContainer
(
    DataPtrT                        aDataPtr,
    const GpReflectProp&            aProp,
    typename VisitorT::VisitCtx&    aCtx
)
{
    typename VisitorT::VisitContainerCtx visitContainerCtx;

    if (visitContainerCtx.OnVisitBegin(aDataPtr, aProp, aCtx) == false)
    {
        return;
    }

    const GpReflectType::EnumT propType = aProp.Type();

    switch (propType)
    {
        case GpReflectType::U_INT_8:    visitContainerCtx.template Value_UInt8<ValGetterT>(aDataPtr, aProp, aCtx);      break;
        case GpReflectType::S_INT_8:    visitContainerCtx.template Value_SInt8<ValGetterT>(aDataPtr, aProp, aCtx);      break;
        case GpReflectType::U_INT_16:   visitContainerCtx.template Value_UInt16<ValGetterT>(aDataPtr, aProp, aCtx);     break;
        case GpReflectType::S_INT_16:   visitContainerCtx.template Value_SInt16<ValGetterT>(aDataPtr, aProp, aCtx);     break;
        case GpReflectType::U_INT_32:   visitContainerCtx.template Value_UInt32<ValGetterT>(aDataPtr, aProp, aCtx);     break;
        case GpReflectType::S_INT_32:   visitContainerCtx.template Value_SInt32<ValGetterT>(aDataPtr, aProp, aCtx);     break;
        case GpReflectType::U_INT_64:   visitContainerCtx.template Value_UInt64<ValGetterT>(aDataPtr, aProp, aCtx);     break;
        case GpReflectType::S_INT_64:   visitContainerCtx.template Value_SInt64<ValGetterT>(aDataPtr, aProp, aCtx);     break;
        case GpReflectType::DOUBLE:     visitContainerCtx.template Value_Double<ValGetterT>(aDataPtr, aProp, aCtx);     break;
        case GpReflectType::FLOAT:      visitContainerCtx.template Value_Float<ValGetterT>(aDataPtr, aProp, aCtx);      break;
        case GpReflectType::BOOLEAN:    visitContainerCtx.template Value_Bool<ValGetterT>(aDataPtr, aProp, aCtx);       break;
        case GpReflectType::UUID:       visitContainerCtx.template Value_UUID<ValGetterT>(aDataPtr, aProp, aCtx);       break;
        case GpReflectType::STRING:     visitContainerCtx.template Value_String<ValGetterT>(aDataPtr, aProp, aCtx);     break;
        case GpReflectType::BLOB:       visitContainerCtx.template Value_BLOB<ValGetterT>(aDataPtr, aProp, aCtx);       break;
        case GpReflectType::OBJECT:     visitContainerCtx.template Value_Object<ValGetterT>(aDataPtr, aProp, aCtx);     break;
        case GpReflectType::OBJECT_SP:  visitContainerCtx.template Value_ObjectSP<ValGetterT>(aDataPtr, aProp, aCtx);   break;
        case GpReflectType::ENUM:       visitContainerCtx.template Value_Enum<ValGetterT>(aDataPtr, aProp, aCtx);       break;
        case GpReflectType::ENUM_FLAGS: visitContainerCtx.template Value_EnumFlags<ValGetterT>(aDataPtr, aProp, aCtx);  break;
        case GpReflectType::NOT_SET:    THROW_GP(u8"Type "_sv + GpReflectType::SToString(aProp.Type()));                    break;
        default:                        THROW_GP(u8"Unsupported type "_sv + GpReflectType::SToString(aProp.Type()));
    }

    visitContainerCtx.OnVisitEnd(aDataPtr, aProp, aCtx);
}

template<typename T,
         typename VisitorT>
void    GpReflectVisitor<T, VisitorT>::VisitValueMap
(
    DataPtrT                        aDataPtr,
    const GpReflectProp&            aProp,
    typename VisitorT::VisitCtx&    aCtx
)
{
    typename VisitorT::VisitMapCtx visitMapCtx;

    if (visitMapCtx.OnVisitBegin(aDataPtr, aProp, aCtx) == false)
    {
        return;
    }

    const GpReflectType::EnumT keyType = aProp.ContainerKeyType();

    switch (keyType)
    {
        case GpReflectType::U_INT_8:    ProcessMapK<u_int_8, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);      break;
        case GpReflectType::S_INT_8:    ProcessMapK<s_int_8, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);      break;
        case GpReflectType::U_INT_16:   ProcessMapK<u_int_16, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);     break;
        case GpReflectType::S_INT_16:   ProcessMapK<s_int_16, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);     break;
        case GpReflectType::U_INT_32:   ProcessMapK<u_int_32, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);     break;
        case GpReflectType::S_INT_32:   ProcessMapK<s_int_32, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);     break;
        case GpReflectType::U_INT_64:   ProcessMapK<u_int_64, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);     break;
        case GpReflectType::S_INT_64:   ProcessMapK<s_int_64, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);     break;
        case GpReflectType::DOUBLE:     ProcessMapK<double, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);       break;
        case GpReflectType::FLOAT:      ProcessMapK<float, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);        break;
        case GpReflectType::BOOLEAN:    THROW_GP(u8"bool is not supported as map key"_sv);                                      break;
        case GpReflectType::UUID:       ProcessMapK<GpUUID, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);       break;
        case GpReflectType::STRING:     ProcessMapK<std::u8string, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx);break;
        case GpReflectType::BLOB:       ProcessMapK<GpBytesArray, GpReflectPropGetter_Map>(aDataPtr, aProp, aCtx, visitMapCtx); break;
        case GpReflectType::OBJECT:     THROW_GP(u8"Object is not supported as map key"_sv);                                    break;
        case GpReflectType::OBJECT_SP:  THROW_GP(u8"Object::SP is not supported as map key"_sv);                                break;
        case GpReflectType::ENUM:       THROW_GP(u8"enums is not supported as map key"_sv);                                     break;
        case GpReflectType::ENUM_FLAGS: THROW_GP(u8"enum flags is not supported as map key"_sv);                                break;
        case GpReflectType::NOT_SET:    THROW_GP(u8"Type "_sv + GpReflectType::SToString(aProp.Type()));                        break;
        default:                        THROW_GP(u8"Unsupported type "_sv + GpReflectType::SToString(aProp.Type()));
    }

    visitMapCtx.OnVisitEnd(aDataPtr, aProp, aCtx);
}

template<typename T,
         typename VisitorT>
template<typename                       KeyT,
         template<typename...> class    ValGetterT>
void    GpReflectVisitor<T, VisitorT>::ProcessMapK
(
    DataPtrT                    aDataPtr,
    const GpReflectProp&            aProp,
    typename VisitorT::VisitCtx&    aCtx,
    typename VisitorT::VisitMapCtx& aCtxMap
)
{
    const GpReflectType::EnumT propType = aProp.Type();

    switch (propType)
    {
        case GpReflectType::U_INT_8:    aCtxMap.template K_UInt8<KeyT,   u_int_8, ValGetterT>(aDataPtr, aProp, aCtx);               break;
        case GpReflectType::S_INT_8:    aCtxMap.template K_SInt8<KeyT,   s_int_8, ValGetterT>(aDataPtr, aProp, aCtx);               break;
        case GpReflectType::U_INT_16:   aCtxMap.template K_UInt16<KeyT, u_int_16, ValGetterT>(aDataPtr, aProp, aCtx);               break;
        case GpReflectType::S_INT_16:   aCtxMap.template K_SInt16<KeyT, s_int_16, ValGetterT>(aDataPtr, aProp, aCtx);               break;
        case GpReflectType::U_INT_32:   aCtxMap.template K_UInt32<KeyT, u_int_32, ValGetterT>(aDataPtr, aProp, aCtx);               break;
        case GpReflectType::S_INT_32:   aCtxMap.template K_SInt32<KeyT, s_int_32, ValGetterT>(aDataPtr, aProp, aCtx);               break;
        case GpReflectType::U_INT_64:   aCtxMap.template K_UInt64<KeyT, u_int_64, ValGetterT>(aDataPtr, aProp, aCtx);               break;
        case GpReflectType::S_INT_64:   aCtxMap.template K_SInt64<KeyT, s_int_64, ValGetterT>(aDataPtr, aProp, aCtx);               break;
        case GpReflectType::DOUBLE:     aCtxMap.template K_Double<KeyT, double, ValGetterT>(aDataPtr, aProp, aCtx);                 break;
        case GpReflectType::FLOAT:      aCtxMap.template K_Float<KeyT, float, ValGetterT>(aDataPtr, aProp, aCtx);                   break;
        case GpReflectType::BOOLEAN:    aCtxMap.template K_Bool<KeyT, bool, ValGetterT>(aDataPtr, aProp, aCtx);                     break;
        case GpReflectType::UUID:       aCtxMap.template K_UUID<KeyT, GpUUID, ValGetterT>(aDataPtr, aProp, aCtx);                   break;
        case GpReflectType::STRING:     aCtxMap.template K_String<KeyT, std::u8string, ValGetterT>(aDataPtr, aProp, aCtx);          break;
        case GpReflectType::BLOB:       aCtxMap.template K_BLOB<KeyT, GpBytesArray, ValGetterT>(aDataPtr, aProp, aCtx);             break;
        case GpReflectType::OBJECT:     THROW_GP(u8"Object is not supported as map key"_sv);                                        break;
        case GpReflectType::OBJECT_SP:  aCtxMap.template K_ObjectSP<KeyT, GpReflectObject::SP, ValGetterT>(aDataPtr, aProp, aCtx);  break;
        case GpReflectType::ENUM:       THROW_GP(u8"enums is not supported as map key"_sv);                                         break;
        case GpReflectType::ENUM_FLAGS: THROW_GP(u8"enum flags is not supported as map key"_sv);                                    break;
        case GpReflectType::NOT_SET:    THROW_GP(u8"Type "_sv + GpReflectType::SToString(aProp.Type()));                            break;
        default:                        THROW_GP(u8"Unsupported type "_sv + GpReflectType::SToString(aProp.Type()));
    }
}

}//namespace GPlatform

#endif//GP_USE_REFLECTION
