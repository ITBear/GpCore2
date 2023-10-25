#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include <array>
#include "GpReflectProp.hpp"

namespace GPlatform {

class GpReflectPropGetter_Val
{
public:
    static const auto&      UInt8       (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UInt8(aDataPtr);}
    static auto&            UInt8       (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UInt8(aDataPtr);}
    static const auto&      SInt8       (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SInt8(aDataPtr);}
    static auto&            SInt8       (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SInt8(aDataPtr);}
    static const auto&      UInt16      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UInt16(aDataPtr);}
    static auto&            UInt16      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UInt16(aDataPtr);}
    static const auto&      SInt16      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SInt16(aDataPtr);}
    static auto&            SInt16      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SInt16(aDataPtr);}
    static const auto&      UInt32      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UInt32(aDataPtr);}
    static auto&            UInt32      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UInt32(aDataPtr);}
    static const auto&      SInt32      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SInt32(aDataPtr);}
    static auto&            SInt32      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SInt32(aDataPtr);}
    static const auto&      UInt64      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UInt64(aDataPtr);}
    static auto&            UInt64      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_UInt64(aDataPtr);}
    static const auto&      SInt64      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SInt64(aDataPtr);}
    static auto&            SInt64      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Value_SInt64(aDataPtr);}
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
    static const auto&                                  UInt8       (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_UInt8(aDataPtr);}
    static auto&                                        UInt8       (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_UInt8(aDataPtr);}
    static const auto&                                  SInt8       (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_SInt8(aDataPtr);}
    static auto&                                        SInt8       (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_SInt8(aDataPtr);}
    static const auto&                                  UInt16      (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_UInt16(aDataPtr);}
    static auto&                                        UInt16      (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_UInt16(aDataPtr);}
    static const auto&                                  SInt16      (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_SInt16(aDataPtr);}
    static auto&                                        SInt16      (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_SInt16(aDataPtr);}
    static const auto&                                  UInt32      (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_UInt32(aDataPtr);}
    static auto&                                        UInt32      (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_UInt32(aDataPtr);}
    static const auto&                                  SInt32      (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_SInt32(aDataPtr);}
    static auto&                                        SInt32      (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_SInt32(aDataPtr);}
    static const auto&                                  UInt64      (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_UInt64(aDataPtr);}
    static auto&                                        UInt64      (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_UInt64(aDataPtr);}
    static const auto&                                  SInt64      (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_SInt64(aDataPtr);}
    static auto&                                        SInt64      (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_SInt64(aDataPtr);}
    static const auto&                                  Double      (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_Double(aDataPtr);}
    static auto&                                        Double      (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_Double(aDataPtr);}
    static const auto&                                  Float       (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_Float(aDataPtr);}
    static auto&                                        Float       (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_Float(aDataPtr);}
    static const auto&                                  Bool        (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_Bool(aDataPtr);}
    static auto&                                        Bool        (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_Bool(aDataPtr);}
    static const auto&                                  UUID        (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_UUID(aDataPtr);}
    static auto&                                        UUID        (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_UUID(aDataPtr);}
    static const auto&                                  String      (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_String(aDataPtr);}
    static auto&                                        String      (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_String(aDataPtr);}
    static const auto&                                  BLOB        (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_BLOB(aDataPtr);}
    static auto&                                        BLOB        (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_BLOB(aDataPtr);}
    static const std::array<GpReflectObject, 0>&        Object      (const void*            /*aDataPtr*/,
                                                                     const GpReflectProp&   /*aProp*/) {static std::array<GpReflectObject, 0> _v; return _v;}
    static std::array<GpReflectObject, 0>&              Object      (void*                  /*aDataPtr*/,
                                                                     const GpReflectProp&   /*aProp*/) {static std::array<GpReflectObject, 0> _v; return _v;}
    static const auto&                                  ObjectSP    (const void*            aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_ObjectSP(aDataPtr);}
    static auto&                                        ObjectSP    (void*                  aDataPtr,
                                                                     const GpReflectProp&   aProp) {return aProp.Vec_ObjectSP(aDataPtr);}
    //static const auto&                                Enum        (const void*            aDataPtr,
    //                                                               const GpReflectProp&   aProp) {return aProp.Vec_Enum(aDataPtr);}
    //static auto&                                      Enum        (void*                  aDataPtr,
    //                                                               const GpReflectProp&   aProp) {return aProp.Vec_Enum(aDataPtr);}
};

class GpReflectPropGetter_VecWrap
{
public:
    static const std::array<u_int_8, 0>&            UInt8       (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<u_int_8, 0> _v; return _v;}
    static std::array<u_int_8, 0>&                  UInt8       (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<u_int_8, 0> _v; return _v;}
    static const std::array<s_int_8, 0>&            SInt8       (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<s_int_8, 0> _v; return _v;}
    static std::array<s_int_8, 0>&                  SInt8       (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<s_int_8, 0> _v; return _v;}
    static const std::array<u_int_16, 0>&           UInt16      (const void*            /*aDataPtr*/,
                                                                const GpReflectProp&    /*aProp*/) {static std::array<u_int_16, 0> _v; return _v;}
    static std::array<u_int_16, 0>&                 UInt16      (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<u_int_16, 0> _v; return _v;}
    static const std::array<s_int_16, 0>&           SInt16      (const void*            /*aDataPtr*/,
                                                                const GpReflectProp&    /*aProp*/) {static std::array<s_int_16, 0> _v; return _v;}
    static std::array<s_int_16, 0>&                 SInt16      (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<s_int_16, 0> _v; return _v;}
    static const std::array<u_int_32, 0>&           UInt32      (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<u_int_32, 0> _v; return _v;}
    static std::array<u_int_32, 0>&                 UInt32      (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<u_int_32, 0> _v; return _v;}
    static const std::array<s_int_32, 0>&           SInt32      (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<s_int_32, 0> _v; return _v;}
    static std::array<s_int_32, 0>&                 SInt32      (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<s_int_32, 0> _v; return _v;}
    static const std::array<u_int_64, 0>&           UInt64      (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<u_int_64, 0> _v; return _v;}
    static std::array<u_int_64, 0>&                 UInt64      (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<u_int_64, 0> _v; return _v;}
    static const std::array<s_int_64, 0>&           SInt64      (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<s_int_64, 0> _v; return _v;}
    static std::array<s_int_64, 0>&                 SInt64      (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<s_int_64, 0> _v; return _v;}
    static const std::array<double, 0>&             Double      (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<double, 0> _v; return _v;}
    static std::array<double, 0>&                   Double      (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<double, 0> _v; return _v;}
    static const std::array<float, 0>&              Float       (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<float, 0> _v; return _v;}
    static std::array<float, 0>&                    Float       (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<float, 0> _v; return _v;}
    static const std::array<bool, 0>&               Bool        (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<bool, 0> _v; return _v;}
    static std::array<bool, 0>&                     Bool        (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<bool, 0> _v; return _v;}
    static const std::array<GpUUID, 0>&             UUID        (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<GpUUID, 0> _v; return _v;}
    static std::array<GpUUID, 0>&                   UUID        (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<GpUUID, 0> _v; return _v;}
    static const std::array<std::u8string, 0>&      String      (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<std::u8string, 0> _v; return _v;}
    static std::array<std::u8string, 0>&            String      (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<std::u8string, 0> _v; return _v;}
    static const std::array<GpBytesArray, 0>&       BLOB        (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<GpBytesArray, 0> _v; return _v;}
    static std::array<GpBytesArray, 0>&             BLOB        (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<GpBytesArray, 0> _v; return _v;}
    static const GpVectorReflectObjWrapBase&        Object      (const void*            aDataPtr,
                                                                 const GpReflectProp&   aProp) {return aProp.VecWrap_Object(aDataPtr);}
    static GpVectorReflectObjWrapBase&              Object      (void*                  aDataPtr,
                                                                 const GpReflectProp&   aProp) {return aProp.VecWrap_Object(aDataPtr);}
    static const std::array<GpSP<GpReflectObject>, 0>&ObjectSP  (const void*            /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<GpSP<GpReflectObject>, 0> _v; return _v;}
    static std::array<GpSP<GpReflectObject>, 0>&    ObjectSP    (void*                  /*aDataPtr*/,
                                                                 const GpReflectProp&   /*aProp*/) {static std::array<GpSP<GpReflectObject>, 0> _v; return _v;}
};

template<typename Key>
class GpReflectPropGetter_Map
{
public:
    static const auto&      UInt8       (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UInt8<Key>(aDataPtr);}
    static auto&            UInt8       (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UInt8<Key>(aDataPtr);}
    static const auto&      SInt8       (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SInt8<Key>(aDataPtr);}
    static auto&            SInt8       (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SInt8<Key>(aDataPtr);}
    static const auto&      UInt16      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UInt16<Key>(aDataPtr);}
    static auto&            UInt16      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UInt16<Key>(aDataPtr);}
    static const auto&      SInt16      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SInt16<Key>(aDataPtr);}
    static auto&            SInt16      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SInt16<Key>(aDataPtr);}
    static const auto&      UInt32      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UInt32<Key>(aDataPtr);}
    static auto&            UInt32      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UInt32<Key>(aDataPtr);}
    static const auto&      SInt32      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SInt32<Key>(aDataPtr);}
    static auto&            SInt32      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SInt32<Key>(aDataPtr);}
    static const auto&      UInt64      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UInt64<Key>(aDataPtr);}
    static auto&            UInt64      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_UInt64<Key>(aDataPtr);}
    static const auto&      SInt64      (const void*            aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SInt64<Key>(aDataPtr);}
    static auto&            SInt64      (void*                  aDataPtr,
                                         const GpReflectProp&   aProp) {return aProp.Map_SInt64<Key>(aDataPtr);}
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

}//GPlatform

#endif//GP_USE_REFLECTION
