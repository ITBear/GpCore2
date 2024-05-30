#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "GpReflectObject.hpp"
#include "GpReflectPropGetter.hpp"

namespace GPlatform {

template<typename VisitorT>
class GpReflectVisitor
{
public:
    void        Visit       (const GpReflectModel&          aModel,
                             typename VisitorT::VisitCtx&   aCtx);

    void        VisitProp   (const GpReflectProp&           aProp,
                             typename VisitorT::VisitCtx&   aCtx);

private:
    void        VisitValue  (const GpReflectProp&           aProp,
                             typename VisitorT::VisitCtx&   aCtx);

    void        VisitVec    (const GpReflectProp&           aProp,
                             typename VisitorT::VisitCtx&   aCtx);

    void        VisitMap    (const GpReflectProp&           aProp,
                             typename VisitorT::VisitCtx&   aCtx);

    template<typename                       KeyT,
             template<typename...> class    ValGetterT>
    void        ProcessMapK (const GpReflectProp&               aProp,
                             typename VisitorT::VisitCtx&       aCtx,
                             typename VisitorT::VisitMapCtx&    aCtxMap);
};

template<typename VisitorT>
void    GpReflectVisitor<VisitorT>::Visit
(
    const GpReflectModel&           aModel,
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
        VisitProp(prop, aCtx);
    }

    aCtx.OnVisitEnd(aModel);
}

template<typename VisitorT>
void    GpReflectVisitor<VisitorT>::VisitProp
(
    const GpReflectProp&            aProp,
    typename VisitorT::VisitCtx&    aCtx
)
{
    const GpReflectContainerType::EnumT containerType = aProp.Container();

    try
    {
        if (containerType == GpReflectContainerType::NO)
        {
            VisitValue(aProp, aCtx);
        } else if (containerType == GpReflectContainerType::VECTOR)
        {
            VisitVec(aProp, aCtx);
        } else if (containerType == GpReflectContainerType::VECTOR_WRAP)
        {
            VisitVec(aProp, aCtx);
        } else if (containerType == GpReflectContainerType::MAP)
        {
            VisitMap(aProp, aCtx);
        }
    } catch (const std::exception& e)
    {
        THROW_GP
        (
            fmt::format
            (
                "Failed to visit propetry {}\nReason:\n{}",
                aProp.Name(),
                e.what()
            )
        );
    }
}

template<typename VisitorT>
void    GpReflectVisitor<VisitorT>::VisitValue
(
    const GpReflectProp&            aProp,
    typename VisitorT::VisitCtx&    aCtx
)
{
    typename VisitorT::VisitValueCtx visitValueCtx;

    if (visitValueCtx.OnVisitBegin(aProp, aCtx) == false)
    {
        return;
    }

    const GpReflectType::EnumT propType = aProp.Type();

    switch (propType)
    {
        case GpReflectType::U_INT_8:    visitValueCtx.UI8(aProp, aCtx);         break;
        case GpReflectType::S_INT_8:    visitValueCtx.SI8(aProp, aCtx);         break;
        case GpReflectType::U_INT_16:   visitValueCtx.UI16(aProp, aCtx);        break;
        case GpReflectType::S_INT_16:   visitValueCtx.SI16(aProp, aCtx);        break;
        case GpReflectType::U_INT_32:   visitValueCtx.UI32(aProp, aCtx);        break;
        case GpReflectType::S_INT_32:   visitValueCtx.SI32(aProp, aCtx);        break;
        case GpReflectType::U_INT_64:   visitValueCtx.UI64(aProp, aCtx);        break;
        case GpReflectType::S_INT_64:   visitValueCtx.SI64(aProp, aCtx);        break;
        case GpReflectType::DOUBLE:     visitValueCtx.Double(aProp, aCtx);      break;
        case GpReflectType::FLOAT:      visitValueCtx.Float(aProp, aCtx);       break;
        case GpReflectType::BOOLEAN:    visitValueCtx.Bool(aProp, aCtx);        break;
        case GpReflectType::UUID:       visitValueCtx.UUID(aProp, aCtx);        break;
        case GpReflectType::STRING:     visitValueCtx.String(aProp, aCtx);      break;
        case GpReflectType::BLOB:       visitValueCtx.BLOB(aProp, aCtx);        break;
        case GpReflectType::OBJECT:     visitValueCtx.Object(aProp, aCtx);      break;
        case GpReflectType::OBJECT_SP:  visitValueCtx.ObjectSP(aProp, aCtx);    break;
        case GpReflectType::ENUM:       visitValueCtx.Enum(aProp, aCtx);        break;
        case GpReflectType::ENUM_FLAGS: visitValueCtx.EnumFlags(aProp, aCtx);   break;
        case GpReflectType::NOT_SET: [[fallthrough]];
        default:
        {
            THROW_GP
            (
                fmt::format
                (
                    "Unsupported type {}",
                    GpReflectType{aProp.Type()}
                )
            );
        } break;
    }

    visitValueCtx.OnVisitEnd(aProp, aCtx);
}

template<typename VisitorT>
void    GpReflectVisitor<VisitorT>::VisitVec
(
    const GpReflectProp&            aProp,
    typename VisitorT::VisitCtx&    aCtx
)
{
    typename VisitorT::VisitVecCtx visitVecCtx;

    if (visitVecCtx.OnVisitBegin(aProp, aCtx) == false)
    {
        return;
    }

    const GpReflectType::EnumT propType = aProp.Type();

    switch (propType)
    {
        case GpReflectType::U_INT_8:    visitVecCtx.template UI8<GpReflectPropGetter_Vec>(aProp, aCtx);     break;
        case GpReflectType::S_INT_8:    visitVecCtx.template SI8<GpReflectPropGetter_Vec>(aProp, aCtx);     break;
        case GpReflectType::U_INT_16:   visitVecCtx.template UI16<GpReflectPropGetter_Vec>(aProp, aCtx);    break;
        case GpReflectType::S_INT_16:   visitVecCtx.template SI16<GpReflectPropGetter_Vec>(aProp, aCtx);    break;
        case GpReflectType::U_INT_32:   visitVecCtx.template UI32<GpReflectPropGetter_Vec>(aProp, aCtx);    break;
        case GpReflectType::S_INT_32:   visitVecCtx.template SI32<GpReflectPropGetter_Vec>(aProp, aCtx);    break;
        case GpReflectType::U_INT_64:   visitVecCtx.template UI64<GpReflectPropGetter_Vec>(aProp, aCtx);    break;
        case GpReflectType::S_INT_64:   visitVecCtx.template SI64<GpReflectPropGetter_Vec>(aProp, aCtx);    break;
        case GpReflectType::DOUBLE:     visitVecCtx.template Double<GpReflectPropGetter_Vec>(aProp, aCtx);  break;
        case GpReflectType::FLOAT:      visitVecCtx.template Float<GpReflectPropGetter_Vec>(aProp, aCtx);   break;
        case GpReflectType::UUID:       visitVecCtx.template UUID<GpReflectPropGetter_Vec>(aProp, aCtx);    break;
        case GpReflectType::STRING:     visitVecCtx.template String<GpReflectPropGetter_Vec>(aProp, aCtx);  break;
        case GpReflectType::BLOB:       visitVecCtx.template BLOB<GpReflectPropGetter_Vec>(aProp, aCtx);    break;
        case GpReflectType::OBJECT:     visitVecCtx.template Object<GpReflectPropGetter_Vec>(aProp, aCtx);  break;
        case GpReflectType::OBJECT_SP:  visitVecCtx.template ObjectSP<GpReflectPropGetter_Vec>(aProp, aCtx);break;
        case GpReflectType::BOOLEAN:    [[fallthrough]];
        case GpReflectType::ENUM:       [[fallthrough]];
        case GpReflectType::ENUM_FLAGS: [[fallthrough]];
        case GpReflectType::NOT_SET:    [[fallthrough]];
        default:
        {
            THROW_GP
            (
                fmt::format
                (
                    "Unsupported type {}",
                    GpReflectType{aProp.Type()}
                )
            );
        }
    }

    visitVecCtx.OnVisitEnd(aProp, aCtx);
}

template<typename VisitorT>
void    GpReflectVisitor<VisitorT>::VisitMap
(
    const GpReflectProp&            aProp,
    typename VisitorT::VisitCtx&    aCtx
)
{
    typename VisitorT::VisitMapCtx visitMapCtx;

    if (visitMapCtx.OnVisitBegin(aProp, aCtx) == false)
    {
        return;
    }

    const GpReflectType::EnumT keyType = aProp.ContainerKeyType();

    switch (keyType)
    {
        case GpReflectType::U_INT_8:    ProcessMapK<u_int_8, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);    break;
        case GpReflectType::S_INT_8:    ProcessMapK<s_int_8, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);    break;
        case GpReflectType::U_INT_16:   ProcessMapK<u_int_16, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);   break;
        case GpReflectType::S_INT_16:   ProcessMapK<s_int_16, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);   break;
        case GpReflectType::U_INT_32:   ProcessMapK<u_int_32, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);   break;
        case GpReflectType::S_INT_32:   ProcessMapK<s_int_32, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);   break;
        case GpReflectType::U_INT_64:   ProcessMapK<u_int_64, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);   break;
        case GpReflectType::S_INT_64:   ProcessMapK<s_int_64, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);   break;
        case GpReflectType::DOUBLE:     ProcessMapK<double, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);     break;
        case GpReflectType::FLOAT:      ProcessMapK<float, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);      break;
        case GpReflectType::UUID:       ProcessMapK<GpUUID, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);     break;
        case GpReflectType::BLOB:       ProcessMapK<GpBytesArray, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);break;
        case GpReflectType::STRING:     ProcessMapK<std::string, GpReflectPropGetter_Map>(aProp, aCtx, visitMapCtx);break;
        case GpReflectType::BOOLEAN:    [[fallthrough]];        
        case GpReflectType::OBJECT:     [[fallthrough]];
        case GpReflectType::OBJECT_SP:  [[fallthrough]];
        case GpReflectType::ENUM:       [[fallthrough]];
        case GpReflectType::ENUM_FLAGS: [[fallthrough]];
        case GpReflectType::NOT_SET:    [[fallthrough]];
        default:
        {
            THROW_GP
            (
                fmt::format
                (
                    "Type {} is not supported as key",
                    GpReflectType{keyType}
                )
            );
        }
    }

    visitMapCtx.OnVisitEnd(aProp, aCtx);
}

template<typename VisitorT>
template<typename                       KeyT,
         template<typename...> class    ValGetterT>
void    GpReflectVisitor<VisitorT>::ProcessMapK
(
    const GpReflectProp&            aProp,
    typename VisitorT::VisitCtx&    aCtx,
    typename VisitorT::VisitMapCtx& aCtxMap
)
{
    const GpReflectType::EnumT propType = aProp.Type();

    switch (propType)
    {
        case GpReflectType::U_INT_8:    aCtxMap.template K_UI8<KeyT,   u_int_8, ValGetterT>(aProp, aCtx);               break;
        case GpReflectType::S_INT_8:    aCtxMap.template K_SI8<KeyT,   s_int_8, ValGetterT>(aProp, aCtx);               break;
        case GpReflectType::U_INT_16:   aCtxMap.template K_UI16<KeyT, u_int_16, ValGetterT>(aProp, aCtx);               break;
        case GpReflectType::S_INT_16:   aCtxMap.template K_SI16<KeyT, s_int_16, ValGetterT>(aProp, aCtx);               break;
        case GpReflectType::U_INT_32:   aCtxMap.template K_UI32<KeyT, u_int_32, ValGetterT>(aProp, aCtx);               break;
        case GpReflectType::S_INT_32:   aCtxMap.template K_SI32<KeyT, s_int_32, ValGetterT>(aProp, aCtx);               break;
        case GpReflectType::U_INT_64:   aCtxMap.template K_UI64<KeyT, u_int_64, ValGetterT>(aProp, aCtx);               break;
        case GpReflectType::S_INT_64:   aCtxMap.template K_SI64<KeyT, s_int_64, ValGetterT>(aProp, aCtx);               break;
        case GpReflectType::DOUBLE:     aCtxMap.template K_Double<KeyT, double, ValGetterT>(aProp, aCtx);               break;
        case GpReflectType::FLOAT:      aCtxMap.template K_Float<KeyT, float, ValGetterT>(aProp, aCtx);                 break;
        case GpReflectType::UUID:       aCtxMap.template K_UUID<KeyT, GpUUID, ValGetterT>(aProp, aCtx);                 break;
        case GpReflectType::STRING:     aCtxMap.template K_String<KeyT, std::string, ValGetterT>(aProp, aCtx);          break;
        case GpReflectType::BLOB:       aCtxMap.template K_BLOB<KeyT, GpBytesArray, ValGetterT>(aProp, aCtx);           break;
        case GpReflectType::OBJECT_SP:  aCtxMap.template K_ObjectSP<KeyT, GpReflectObject::SP, ValGetterT>(aProp, aCtx);break;
        case GpReflectType::BOOLEAN:    [[fallthrough]];
        case GpReflectType::OBJECT:     [[fallthrough]];
        case GpReflectType::ENUM:       [[fallthrough]];
        case GpReflectType::ENUM_FLAGS: [[fallthrough]];
        case GpReflectType::NOT_SET:    [[fallthrough]];
        default:
        {
            THROW_GP
            (
                fmt::format
                (
                    "Type {} is not supported as value",
                    GpReflectType{propType}
                )
            );
        }
    }
}

}// namespace GPlatform
