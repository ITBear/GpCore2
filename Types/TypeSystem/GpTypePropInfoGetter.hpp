#pragma once

#include "GpTypePropInfo.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

namespace GPlatform {

class GpTypePropInfoGetter_Val
{
public:
    static const auto&      UInt8       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_UInt8(aStruct);}
    static auto&            UInt8       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_UInt8(aStruct);}
    static const auto&      SInt8       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_SInt8(aStruct);}
    static auto&            SInt8       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_SInt8(aStruct);}
    static const auto&      UInt16      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_UInt16(aStruct);}
    static auto&            UInt16      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_UInt16(aStruct);}
    static const auto&      SInt16      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_SInt16(aStruct);}
    static auto&            SInt16      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_SInt16(aStruct);}
    static const auto&      UInt32      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_UInt32(aStruct);}
    static auto&            UInt32      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_UInt32(aStruct);}
    static const auto&      SInt32      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_SInt32(aStruct);}
    static auto&            SInt32      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_SInt32(aStruct);}
    static const auto&      UInt64      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_UInt64(aStruct);}
    static auto&            UInt64      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_UInt64(aStruct);}
    static const auto&      SInt64      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_SInt64(aStruct);}
    static auto&            SInt64      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_SInt64(aStruct);}
    static const auto&      Double      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Double(aStruct);}
    static auto&            Double      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Double(aStruct);}
    static const auto&      Float       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Float(aStruct);}
    static auto&            Float       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Float(aStruct);}
    static const auto&      Bool        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Bool(aStruct);}
    static auto&            Bool        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Bool(aStruct);}
    static const auto&      UUID        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_UUID(aStruct);}
    static auto&            UUID        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_UUID(aStruct);}
    static const auto&      String      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_String(aStruct);}
    static auto&            String      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_String(aStruct);}
    static const auto&      BLOB        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_BLOB(aStruct);}
    static auto&            BLOB        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_BLOB(aStruct);}
    static const auto&      Struct      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Struct(aStruct);}
    static auto&            Struct      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Struct(aStruct);}
    static const auto&      StructSP    (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_StructSP(aStruct);}
    static auto&            StructSP    (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_StructSP(aStruct);}
    static const auto&      Enum        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Enum(aStruct);}
    static auto&            Enum        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Enum(aStruct);}
    static const auto&      EnumFlags   (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_EnumFlags(aStruct);}
    static auto&            EnumFlags   (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_EnumFlags(aStruct);}
};

class GpTypePropInfoGetter_Vec
{
public:
    static const auto&      UInt8       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_UInt8(aStruct);}
    static auto&            UInt8       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_UInt8(aStruct);}
    static const auto&      SInt8       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_SInt8(aStruct);}
    static auto&            SInt8       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_SInt8(aStruct);}
    static const auto&      UInt16      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_UInt16(aStruct);}
    static auto&            UInt16      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_UInt16(aStruct);}
    static const auto&      SInt16      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_SInt16(aStruct);}
    static auto&            SInt16      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_SInt16(aStruct);}
    static const auto&      UInt32      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_UInt32(aStruct);}
    static auto&            UInt32      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_UInt32(aStruct);}
    static const auto&      SInt32      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_SInt32(aStruct);}
    static auto&            SInt32      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_SInt32(aStruct);}
    static const auto&      UInt64      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_UInt64(aStruct);}
    static auto&            UInt64      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_UInt64(aStruct);}
    static const auto&      SInt64      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_SInt64(aStruct);}
    static auto&            SInt64      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_SInt64(aStruct);}
    static const auto&      Double      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_Double(aStruct);}
    static auto&            Double      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_Double(aStruct);}
    static const auto&      Float       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_Float(aStruct);}
    static auto&            Float       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_Float(aStruct);}
    static const auto&      Bool        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_Bool(aStruct);}
    static auto&            Bool        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_Bool(aStruct);}
    static const auto&      UUID        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_UUID(aStruct);}
    static auto&            UUID        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_UUID(aStruct);}
    static const auto&      String      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_String(aStruct);}
    static auto&            String      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_String(aStruct);}
    static const auto&      BLOB        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_BLOB(aStruct);}
    static auto&            BLOB        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_BLOB(aStruct);}
    //static const auto&    Struct      (const GpTypeStructBase&    aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_Struct(aStruct);}
    //static auto&          Struct      (GpTypeStructBase&          aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_Struct(aStruct);}
    static const auto&      StructSP    (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_StructSP(aStruct);}
    static auto&            StructSP    (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_StructSP(aStruct);}
    //static const auto&    Enum        (const GpTypeStructBase&    aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_Enum(aStruct);}
    //static auto&          Enum        (GpTypeStructBase&          aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Vec_Enum(aStruct);}
};

class GpTypePropInfoGetter_List
{
public:
    static const auto&      UInt8       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_UInt8(aStruct);}
    static auto&            UInt8       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_UInt8(aStruct);}
    static const auto&      SInt8       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_SInt8(aStruct);}
    static auto&            SInt8       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_SInt8(aStruct);}
    static const auto&      UInt16      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_UInt16(aStruct);}
    static auto&            UInt16      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_UInt16(aStruct);}
    static const auto&      SInt16      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_SInt16(aStruct);}
    static auto&            SInt16      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_SInt16(aStruct);}
    static const auto&      UInt32      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_UInt32(aStruct);}
    static auto&            UInt32      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_UInt32(aStruct);}
    static const auto&      SInt32      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_SInt32(aStruct);}
    static auto&            SInt32      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_SInt32(aStruct);}
    static const auto&      UInt64      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_UInt64(aStruct);}
    static auto&            UInt64      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_UInt64(aStruct);}
    static const auto&      SInt64      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_SInt64(aStruct);}
    static auto&            SInt64      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_SInt64(aStruct);}
    static const auto&      Double      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_Double(aStruct);}
    static auto&            Double      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_Double(aStruct);}
    static const auto&      Float       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_Float(aStruct);}
    static auto&            Float       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_Float(aStruct);}
    static const auto&      Bool        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_Bool(aStruct);}
    static auto&            Bool        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_Bool(aStruct);}
    static const auto&      UUID        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_UUID(aStruct);}
    static auto&            UUID        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_UUID(aStruct);}
    static const auto&      String      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_String(aStruct);}
    static auto&            String      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_String(aStruct);}
    static const auto&      BLOB        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_BLOB(aStruct);}
    static auto&            BLOB        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_BLOB(aStruct);}
    //static const auto&    Struct      (const GpTypeStructBase&    aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_Struct(aStruct);}
    //static auto&          Struct      (GpTypeStructBase&          aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_Struct(aStruct);}
    static const auto&      StructSP    (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_StructSP(aStruct);}
    static auto&            StructSP    (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_StructSP(aStruct);}
    //static const auto&    Enum        (const GpTypeStructBase&    aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_Enum(aStruct);}
    //static auto&          Enum        (GpTypeStructBase&          aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_List_Enum(aStruct);}
};

class GpTypePropInfoGetter_Set
{
public:
    static const auto&      UInt8       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_UInt8(aStruct);}
    static auto&            UInt8       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_UInt8(aStruct);}
    static const auto&      SInt8       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_SInt8(aStruct);}
    static auto&            SInt8       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_SInt8(aStruct);}
    static const auto&      UInt16      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_UInt16(aStruct);}
    static auto&            UInt16      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_UInt16(aStruct);}
    static const auto&      SInt16      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_SInt16(aStruct);}
    static auto&            SInt16      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_SInt16(aStruct);}
    static const auto&      UInt32      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_UInt32(aStruct);}
    static auto&            UInt32      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_UInt32(aStruct);}
    static const auto&      SInt32      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_SInt32(aStruct);}
    static auto&            SInt32      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_SInt32(aStruct);}
    static const auto&      UInt64      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_UInt64(aStruct);}
    static auto&            UInt64      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_UInt64(aStruct);}
    static const auto&      SInt64      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_SInt64(aStruct);}
    static auto&            SInt64      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_SInt64(aStruct);}
    static const auto&      Double      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_Double(aStruct);}
    static auto&            Double      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_Double(aStruct);}
    static const auto&      Float       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_Float(aStruct);}
    static auto&            Float       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_Float(aStruct);}
    static const auto&      Bool        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_Bool(aStruct);}
    static auto&            Bool        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_Bool(aStruct);}
    static const auto&      UUID        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_UUID(aStruct);}
    static auto&            UUID        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_UUID(aStruct);}
    static const auto&      String      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_String(aStruct);}
    static auto&            String      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_String(aStruct);}
    static const auto&      BLOB        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_BLOB(aStruct);}
    static auto&            BLOB        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_BLOB(aStruct);}
    //static const auto&    Struct      (const GpTypeStructBase&    aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_Struct(aStruct);}
    //static auto&          Struct      (GpTypeStructBase&          aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_Struct(aStruct);}
    static const auto&      StructSP    (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_StructSP(aStruct);}
    static auto&            StructSP    (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_StructSP(aStruct);}
    //static const auto&    Enum        (const GpTypeStructBase&    aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_Enum(aStruct);}
    //static auto&          Enum        (GpTypeStructBase&          aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Set_Enum(aStruct);}
};

template<typename Key>
class GpTypePropInfoGetter_Map
{
public:
    static const auto&      UInt8       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_UInt8<Key>(aStruct);}
    static auto&            UInt8       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_UInt8<Key>(aStruct);}
    static const auto&      SInt8       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_SInt8<Key>(aStruct);}
    static auto&            SInt8       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_SInt8<Key>(aStruct);}
    static const auto&      UInt16      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_UInt16<Key>(aStruct);}
    static auto&            UInt16      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_UInt16<Key>(aStruct);}
    static const auto&      SInt16      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_SInt16<Key>(aStruct);}
    static auto&            SInt16      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_SInt16<Key>(aStruct);}
    static const auto&      UInt32      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_UInt32<Key>(aStruct);}
    static auto&            UInt32      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_UInt32<Key>(aStruct);}
    static const auto&      SInt32      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_SInt32<Key>(aStruct);}
    static auto&            SInt32      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_SInt32<Key>(aStruct);}
    static const auto&      UInt64      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_UInt64<Key>(aStruct);}
    static auto&            UInt64      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_UInt64<Key>(aStruct);}
    static const auto&      SInt64      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_SInt64<Key>(aStruct);}
    static auto&            SInt64      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_SInt64<Key>(aStruct);}
    static const auto&      Double      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_Double<Key>(aStruct);}
    static auto&            Double      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_Double<Key>(aStruct);}
    static const auto&      Float       (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_Float<Key>(aStruct);}
    static auto&            Float       (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_Float<Key>(aStruct);}
    static const auto&      Bool        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_Bool<Key>(aStruct);}
    static auto&            Bool        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_Bool<Key>(aStruct);}
    static const auto&      UUID        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_UUID<Key>(aStruct);}
    static auto&            UUID        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_UUID<Key>(aStruct);}
    static const auto&      String      (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_String<Key>(aStruct);}
    static auto&            String      (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_String<Key>(aStruct);}
    static const auto&      BLOB        (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_BLOB<Key>(aStruct);}
    static auto&            BLOB        (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_BLOB<Key>(aStruct);}
    //static const auto&    Struct      (const GpTypeStructBase&    aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_Struct<Key>(aStruct);}
    //static auto&          Struct      (GpTypeStructBase&          aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_Struct<Key>(aStruct);}
    static const auto&      StructSP    (const GpTypeStructBase&    aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_StructSP<Key>(aStruct);}
    static auto&            StructSP    (GpTypeStructBase&          aStruct,
                                         const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_StructSP<Key>(aStruct);}
    //static const auto&    Enum        (const GpTypeStructBase&    aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_Enum<Key>(aStruct);}
    //static auto&          Enum        (GpTypeStructBase&          aStruct,
    //                                   const GpTypePropInfo&      aPropInfo) {return aPropInfo.Value_Map_Enum<Key>(aStruct);}
};

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
