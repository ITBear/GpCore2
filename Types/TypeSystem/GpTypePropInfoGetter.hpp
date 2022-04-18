#pragma once

#include "GpTypePropInfo.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

class GpTypePropInfoGetter_Val
{
public:
    static const auto&      UInt8       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_UInt8(aDataPtr);}
    static auto&            UInt8       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_UInt8(aDataPtr);}
    static const auto&      SInt8       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_SInt8(aDataPtr);}
    static auto&            SInt8       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_SInt8(aDataPtr);}
    static const auto&      UInt16      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_UInt16(aDataPtr);}
    static auto&            UInt16      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_UInt16(aDataPtr);}
    static const auto&      SInt16      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_SInt16(aDataPtr);}
    static auto&            SInt16      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_SInt16(aDataPtr);}
    static const auto&      UInt32      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_UInt32(aDataPtr);}
    static auto&            UInt32      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_UInt32(aDataPtr);}
    static const auto&      SInt32      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_SInt32(aDataPtr);}
    static auto&            SInt32      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_SInt32(aDataPtr);}
    static const auto&      UInt64      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_UInt64(aDataPtr);}
    static auto&            UInt64      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_UInt64(aDataPtr);}
    static const auto&      SInt64      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_SInt64(aDataPtr);}
    static auto&            SInt64      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_SInt64(aDataPtr);}
    static const auto&      Double      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_Double(aDataPtr);}
    static auto&            Double      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_Double(aDataPtr);}
    static const auto&      Float       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_Float(aDataPtr);}
    static auto&            Float       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_Float(aDataPtr);}
    static const auto&      Bool        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_Bool(aDataPtr);}
    static auto&            Bool        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_Bool(aDataPtr);}
    static const auto&      UUID        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_UUID(aDataPtr);}
    static auto&            UUID        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_UUID(aDataPtr);}
    static const auto&      String      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_String(aDataPtr);}
    static auto&            String      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_String(aDataPtr);}
    static const auto&      BLOB        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_BLOB(aDataPtr);}
    static auto&            BLOB        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_BLOB(aDataPtr);}
    static const auto&      Struct      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_Struct(aDataPtr);}
    static auto&            Struct      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_Struct(aDataPtr);}
    static const auto&      StructSP    (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_StructSP(aDataPtr);}
    static auto&            StructSP    (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_StructSP(aDataPtr);}
    static const auto&      Enum        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_Enum(aDataPtr);}
    static auto&            Enum        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_Enum(aDataPtr);}
    static const auto&      EnumFlags   (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_EnumFlags(aDataPtr);}
    static auto&            EnumFlags   (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Value_EnumFlags(aDataPtr);}
};

class GpTypePropInfoGetter_Vec
{
public:
    static const auto&      UInt8       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_UInt8(aDataPtr);}
    static auto&            UInt8       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_UInt8(aDataPtr);}
    static const auto&      SInt8       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_SInt8(aDataPtr);}
    static auto&            SInt8       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_SInt8(aDataPtr);}
    static const auto&      UInt16      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_UInt16(aDataPtr);}
    static auto&            UInt16      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_UInt16(aDataPtr);}
    static const auto&      SInt16      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_SInt16(aDataPtr);}
    static auto&            SInt16      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_SInt16(aDataPtr);}
    static const auto&      UInt32      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_UInt32(aDataPtr);}
    static auto&            UInt32      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_UInt32(aDataPtr);}
    static const auto&      SInt32      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_SInt32(aDataPtr);}
    static auto&            SInt32      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_SInt32(aDataPtr);}
    static const auto&      UInt64      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_UInt64(aDataPtr);}
    static auto&            UInt64      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_UInt64(aDataPtr);}
    static const auto&      SInt64      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_SInt64(aDataPtr);}
    static auto&            SInt64      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_SInt64(aDataPtr);}
    static const auto&      Double      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_Double(aDataPtr);}
    static auto&            Double      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_Double(aDataPtr);}
    static const auto&      Float       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_Float(aDataPtr);}
    static auto&            Float       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_Float(aDataPtr);}
    static const auto&      Bool        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_Bool(aDataPtr);}
    static auto&            Bool        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_Bool(aDataPtr);}
    static const auto&      UUID        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_UUID(aDataPtr);}
    static auto&            UUID        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_UUID(aDataPtr);}
    static const auto&      String      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_String(aDataPtr);}
    static auto&            String      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_String(aDataPtr);}
    static const auto&      BLOB        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_BLOB(aDataPtr);}
    static auto&            BLOB        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_BLOB(aDataPtr);}
    //static const auto&    Struct      (const void*            aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_Struct(aDataPtr);}
    //static auto&          Struct      (void*                  aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_Struct(aDataPtr);}
    static const auto&      StructSP    (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_StructSP(aDataPtr);}
    static auto&            StructSP    (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_StructSP(aDataPtr);}
    //static const auto&    Enum        (const void*            aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_Enum(aDataPtr);}
    //static auto&          Enum        (void*                  aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Vec_Enum(aDataPtr);}
};

class GpTypePropInfoGetter_List
{
public:
    static const auto&      UInt8       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_UInt8(aDataPtr);}
    static auto&            UInt8       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_UInt8(aDataPtr);}
    static const auto&      SInt8       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_SInt8(aDataPtr);}
    static auto&            SInt8       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_SInt8(aDataPtr);}
    static const auto&      UInt16      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_UInt16(aDataPtr);}
    static auto&            UInt16      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_UInt16(aDataPtr);}
    static const auto&      SInt16      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_SInt16(aDataPtr);}
    static auto&            SInt16      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_SInt16(aDataPtr);}
    static const auto&      UInt32      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_UInt32(aDataPtr);}
    static auto&            UInt32      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_UInt32(aDataPtr);}
    static const auto&      SInt32      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_SInt32(aDataPtr);}
    static auto&            SInt32      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_SInt32(aDataPtr);}
    static const auto&      UInt64      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_UInt64(aDataPtr);}
    static auto&            UInt64      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_UInt64(aDataPtr);}
    static const auto&      SInt64      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_SInt64(aDataPtr);}
    static auto&            SInt64      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_SInt64(aDataPtr);}
    static const auto&      Double      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_Double(aDataPtr);}
    static auto&            Double      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_Double(aDataPtr);}
    static const auto&      Float       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_Float(aDataPtr);}
    static auto&            Float       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_Float(aDataPtr);}
    static const auto&      Bool        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_Bool(aDataPtr);}
    static auto&            Bool        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_Bool(aDataPtr);}
    static const auto&      UUID        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_UUID(aDataPtr);}
    static auto&            UUID        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_UUID(aDataPtr);}
    static const auto&      String      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_String(aDataPtr);}
    static auto&            String      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_String(aDataPtr);}
    static const auto&      BLOB        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_BLOB(aDataPtr);}
    static auto&            BLOB        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_BLOB(aDataPtr);}
    //static const auto&    Struct      (const void*            aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_Struct(aDataPtr);}
    //static auto&          Struct      (void*                  aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_Struct(aDataPtr);}
    static const auto&      StructSP    (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_StructSP(aDataPtr);}
    static auto&            StructSP    (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_StructSP(aDataPtr);}
    //static const auto&    Enum        (const void*            aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_Enum(aDataPtr);}
    //static auto&          Enum        (void*                  aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.List_Enum(aDataPtr);}
};

class GpTypePropInfoGetter_Set
{
public:
    static const auto&      UInt8       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_UInt8(aDataPtr);}
    static auto&            UInt8       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_UInt8(aDataPtr);}
    static const auto&      SInt8       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_SInt8(aDataPtr);}
    static auto&            SInt8       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_SInt8(aDataPtr);}
    static const auto&      UInt16      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_UInt16(aDataPtr);}
    static auto&            UInt16      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_UInt16(aDataPtr);}
    static const auto&      SInt16      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_SInt16(aDataPtr);}
    static auto&            SInt16      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_SInt16(aDataPtr);}
    static const auto&      UInt32      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_UInt32(aDataPtr);}
    static auto&            UInt32      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_UInt32(aDataPtr);}
    static const auto&      SInt32      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_SInt32(aDataPtr);}
    static auto&            SInt32      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_SInt32(aDataPtr);}
    static const auto&      UInt64      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_UInt64(aDataPtr);}
    static auto&            UInt64      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_UInt64(aDataPtr);}
    static const auto&      SInt64      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_SInt64(aDataPtr);}
    static auto&            SInt64      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_SInt64(aDataPtr);}
    static const auto&      Double      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_Double(aDataPtr);}
    static auto&            Double      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_Double(aDataPtr);}
    static const auto&      Float       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_Float(aDataPtr);}
    static auto&            Float       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_Float(aDataPtr);}
    static const auto&      Bool        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_Bool(aDataPtr);}
    static auto&            Bool        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_Bool(aDataPtr);}
    static const auto&      UUID        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_UUID(aDataPtr);}
    static auto&            UUID        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_UUID(aDataPtr);}
    static const auto&      String      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_String(aDataPtr);}
    static auto&            String      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_String(aDataPtr);}
    static const auto&      BLOB        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_BLOB(aDataPtr);}
    static auto&            BLOB        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_BLOB(aDataPtr);}
    //static const auto&    Struct      (const void*            aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_Struct(aDataPtr);}
    //static auto&          Struct      (void*                  aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_Struct(aDataPtr);}
    static const auto&      StructSP    (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_StructSP(aDataPtr);}
    static auto&            StructSP    (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_StructSP(aDataPtr);}
    //static const auto&    Enum        (const void*            aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_Enum(aDataPtr);}
    //static auto&          Enum        (void*                  aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Set_Enum(aDataPtr);}
};

template<typename Key>
class GpTypePropInfoGetter_Map
{
public:
    static const auto&      UInt8       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_UInt8<Key>(aDataPtr);}
    static auto&            UInt8       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_UInt8<Key>(aDataPtr);}
    static const auto&      SInt8       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_SInt8<Key>(aDataPtr);}
    static auto&            SInt8       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_SInt8<Key>(aDataPtr);}
    static const auto&      UInt16      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_UInt16<Key>(aDataPtr);}
    static auto&            UInt16      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_UInt16<Key>(aDataPtr);}
    static const auto&      SInt16      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_SInt16<Key>(aDataPtr);}
    static auto&            SInt16      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_SInt16<Key>(aDataPtr);}
    static const auto&      UInt32      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_UInt32<Key>(aDataPtr);}
    static auto&            UInt32      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_UInt32<Key>(aDataPtr);}
    static const auto&      SInt32      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_SInt32<Key>(aDataPtr);}
    static auto&            SInt32      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_SInt32<Key>(aDataPtr);}
    static const auto&      UInt64      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_UInt64<Key>(aDataPtr);}
    static auto&            UInt64      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_UInt64<Key>(aDataPtr);}
    static const auto&      SInt64      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_SInt64<Key>(aDataPtr);}
    static auto&            SInt64      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_SInt64<Key>(aDataPtr);}
    static const auto&      Double      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_Double<Key>(aDataPtr);}
    static auto&            Double      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_Double<Key>(aDataPtr);}
    static const auto&      Float       (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_Float<Key>(aDataPtr);}
    static auto&            Float       (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_Float<Key>(aDataPtr);}
    static const auto&      Bool        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_Bool<Key>(aDataPtr);}
    static auto&            Bool        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_Bool<Key>(aDataPtr);}
    static const auto&      UUID        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_UUID<Key>(aDataPtr);}
    static auto&            UUID        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_UUID<Key>(aDataPtr);}
    static const auto&      String      (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_String<Key>(aDataPtr);}
    static auto&            String      (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_String<Key>(aDataPtr);}
    static const auto&      BLOB        (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_BLOB<Key>(aDataPtr);}
    static auto&            BLOB        (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_BLOB<Key>(aDataPtr);}
    //static const auto&    Struct      (const void*            aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_Struct<Key>(aDataPtr);}
    //static auto&          Struct      (void*                  aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_Struct<Key>(aDataPtr);}
    static const auto&      StructSP    (const void*            aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_StructSP<Key>(aDataPtr);}
    static auto&            StructSP    (void*                  aDataPtr,
                                         const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_StructSP<Key>(aDataPtr);}
    //static const auto&    Enum        (const void*            aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_Enum<Key>(aDataPtr);}
    //static auto&          Enum        (void*                  aDataPtr,
    //                                   const GpTypePropInfo&  aPropInfo) {return aPropInfo.Map_Enum<Key>(aDataPtr);}
};

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
