#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectProp.hpp>

#include <array>

namespace GPlatform {

class GpReflectPropGetter_Val
{
public:
    static const auto&      UI8         (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UI8(aDataPtr);}
    static auto&            UI8         (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UI8(aDataPtr);}
    static const auto&      SI8         (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SI8(aDataPtr);}
    static auto&            SI8         (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SI8(aDataPtr);}
    static const auto&      UI16        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UI16(aDataPtr);}
    static auto&            UI16        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UI16(aDataPtr);}
    static const auto&      SI16        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SI16(aDataPtr);}
    static auto&            SI16        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SI16(aDataPtr);}
    static const auto&      UI32        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UI32(aDataPtr);}
    static auto&            UI32        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UI32(aDataPtr);}
    static const auto&      SI32        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SI32(aDataPtr);}
    static auto&            SI32        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SI32(aDataPtr);}
    static const auto&      UI64        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UI64(aDataPtr);}
    static auto&            UI64        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UI64(aDataPtr);}
    static const auto&      SI64        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SI64(aDataPtr);}
    static auto&            SI64        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SI64(aDataPtr);}
    static const auto&      Double      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_Double(aDataPtr);}
    static auto&            Double      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_Double(aDataPtr);}
    static const auto&      Float       (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_Float(aDataPtr);}
    static auto&            Float       (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_Float(aDataPtr);}
    static const auto&      Bool        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_Bool(aDataPtr);}
    static auto&            Bool        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_Bool(aDataPtr);}
    static const auto&      UUID        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UUID(aDataPtr);}
    static auto&            UUID        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UUID(aDataPtr);}
    static const auto&      String      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_String(aDataPtr);}
    static auto&            String      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_String(aDataPtr);}
    static const auto&      BLOB        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_BLOB(aDataPtr);}
    static auto&            BLOB        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_BLOB(aDataPtr);}
    static const auto&      Object      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_Object(aDataPtr);}
    static auto&            Object      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_Object(aDataPtr);}
    static const auto&      ObjectSP    (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_ObjectSP(aDataPtr);}
    static auto&            ObjectSP    (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_ObjectSP(aDataPtr);}
    static const auto&      Enum        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_Enum(aDataPtr);}
    static auto&            Enum        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_Enum(aDataPtr);}
    static const auto&      EnumFlags   (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_EnumFlags(aDataPtr);}
    static auto&            EnumFlags   (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_EnumFlags(aDataPtr);}
};

class GpReflectPropGetter_Vec
{
public:
    static const auto&                          UI8         (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_UI8(aDataPtr);}
    static auto&                                UI8         (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_UI8(aDataPtr);}
    static const auto&                          SI8         (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_SI8(aDataPtr);}
    static auto&                                SI8         (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_SI8(aDataPtr);}
    static const auto&                          UI16        (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_UI16(aDataPtr);}
    static auto&                                UI16        (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_UI16(aDataPtr);}
    static const auto&                          SI16        (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_SI16(aDataPtr);}
    static auto&                                SI16        (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_SI16(aDataPtr);}
    static const auto&                          UI32        (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_UI32(aDataPtr);}
    static auto&                                UI32        (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_UI32(aDataPtr);}
    static const auto&                          SI32        (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_SI32(aDataPtr);}
    static auto&                                SI32        (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_SI32(aDataPtr);}
    static const auto&                          UI64        (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_UI64(aDataPtr);}
    static auto&                                UI64        (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_UI64(aDataPtr);}
    static const auto&                          SI64        (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_SI64(aDataPtr);}
    static auto&                                SI64        (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_SI64(aDataPtr);}
    static const auto&                          Double      (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_Double(aDataPtr);}
    static auto&                                Double      (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_Double(aDataPtr);}
    static const auto&                          Float       (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_Float(aDataPtr);}
    static auto&                                Float       (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_Float(aDataPtr);}
    //static const auto&                        Bool        (const void*            aDataPtr,
    //                                                       const GpReflectProp&   aProp) {return aProp.Vec_Bool(aDataPtr);}
    //static auto&                              Bool        (void*                  aDataPtr,
    //                                                       const GpReflectProp&   aProp) {return aProp.Vec_Bool(aDataPtr);}
    static const auto&                          UUID        (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_UUID(aDataPtr);}
    static auto&                                UUID        (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_UUID(aDataPtr);}
    static const auto&                          String      (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_String(aDataPtr);}
    static auto&                                String      (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_String(aDataPtr);}
    static const auto&                          BLOB        (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_BLOB(aDataPtr);}
    static auto&                                BLOB        (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_BLOB(aDataPtr);}
    static const GpVectorReflectObjWrapBase&    Object      (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.VecWrap_Object(aDataPtr);}
    static GpVectorReflectObjWrapBase&          Object      (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.VecWrap_Object(aDataPtr);}
    static const auto&                          ObjectSP    (const void*            aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_ObjectSP(aDataPtr);}
    static auto&                                ObjectSP    (void*                  aDataPtr,
                                                             const GpReflectProp&   aProp) {return aProp.Vec_ObjectSP(aDataPtr);}
    //static const auto&                        Enum        (const void*            aDataPtr,
    //                                                       const GpReflectProp&   aProp) {return aProp.Vec_Enum(aDataPtr);}
    //static auto&                              Enum        (void*                  aDataPtr,
    //                                                       const GpReflectProp&   aProp) {return aProp.Vec_Enum(aDataPtr);}
};

template<typename Key>
class GpReflectPropGetter_Map
{
public:
    static const auto&      UI8         (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UI8<Key>(aDataPtr);}
    static auto&            UI8         (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UI8<Key>(aDataPtr);}
    static const auto&      SI8         (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SI8<Key>(aDataPtr);}
    static auto&            SI8         (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SI8<Key>(aDataPtr);}
    static const auto&      UI16        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UI16<Key>(aDataPtr);}
    static auto&            UI16        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UI16<Key>(aDataPtr);}
    static const auto&      SI16        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SI16<Key>(aDataPtr);}
    static auto&            SI16        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SI16<Key>(aDataPtr);}
    static const auto&      UI32        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UI32<Key>(aDataPtr);}
    static auto&            UI32        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UI32<Key>(aDataPtr);}
    static const auto&      SI32        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SI32<Key>(aDataPtr);}
    static auto&            SI32        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SI32<Key>(aDataPtr);}
    static const auto&      UI64        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UI64<Key>(aDataPtr);}
    static auto&            UI64        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UI64<Key>(aDataPtr);}
    static const auto&      SI64        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SI64<Key>(aDataPtr);}
    static auto&            SI64        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SI64<Key>(aDataPtr);}
    static const auto&      Double      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_Double<Key>(aDataPtr);}
    static auto&            Double      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_Double<Key>(aDataPtr);}
    static const auto&      Float       (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_Float<Key>(aDataPtr);}
    static auto&            Float       (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_Float<Key>(aDataPtr);}
    static const auto&      Bool        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_Bool<Key>(aDataPtr);}
    static auto&            Bool        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_Bool<Key>(aDataPtr);}
    static const auto&      UUID        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UUID<Key>(aDataPtr);}
    static auto&            UUID        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UUID<Key>(aDataPtr);}
    static const auto&      String      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_String<Key>(aDataPtr);}
    static auto&            String      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_String<Key>(aDataPtr);}
    static const auto&      BLOB        (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_BLOB<Key>(aDataPtr);}
    static auto&            BLOB        (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_BLOB<Key>(aDataPtr);}
    //static const auto&    Object      (const void*            aDataPtr,
    //                                   const GpReflectProp&   aProp) {return aProp.Map_Object<Key>(aDataPtr);}
    //static auto&          Object      (void*                  aDataPtr,
    //                                   const GpReflectProp&   aProp) {return aProp.Map_Object<Key>(aDataPtr);}
    static const auto&      ObjectSP    (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_ObjectSP<Key>(aDataPtr);}
    static auto&            ObjectSP    (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_ObjectSP<Key>(aDataPtr);}
    //static const auto&    Enum        (const void*            aDataPtr,
    //                                   const GpReflectProp&   aProp) {return aProp.Map_Enum<Key>(aDataPtr);}
    //static auto&          Enum        (void*                  aDataPtr,
    //                                   const GpReflectProp&   aProp) {return aProp.Map_Enum<Key>(aDataPtr);}
};

}// namespace GPlatform
