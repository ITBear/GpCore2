#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "../UIDs/GpUUID.hpp"
#include "../Classes/GpClassesDefines.hpp"
#include "../Containers/GpContainersT.hpp"
#include "GpType.hpp"
#include "GpTypeContainer.hpp"
#include "GpTypePropFlags.hpp"

namespace GPlatform {

class GpTypeStructBase;

class GPCORE_API GpTypePropInfo
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypePropInfo)

    using TypeT         = GpType;
    using TypeTE        = TypeT::EnumT;
    using ContainerT    = GpTypeContainer;
    using ContainerTE   = ContainerT::EnumT;
    using FlagT         = GpTypePropFlag;
    using FlagTE        = FlagT::EnumT;

public:
                                    GpTypePropInfo      (void) noexcept;
                                    GpTypePropInfo      (const TypeTE           aType,
                                                         const GpUUID           aTypeUID,
                                                         const ContainerTE      aContainer,
                                                         const TypeTE           aContainerKeyType,
                                                         std::string_view       aName,
                                                         const size_t           aAlign,
                                                         const size_t           aSize,
                                                         const std::ptrdiff_t   aOffset,
                                                         const GpTypePropFlags& aFlags);
                                    GpTypePropInfo      (const GpTypePropInfo& aPropInfo);
                                    GpTypePropInfo      (GpTypePropInfo&& aPropInfo) noexcept;
                                    ~GpTypePropInfo     (void) noexcept;

    GpTypePropInfo&                 operator=           (const GpTypePropInfo& aPropInfo);
    GpTypePropInfo&                 operator=           (GpTypePropInfo&& aPropInfo) noexcept;

    TypeTE                          Type                (void) const noexcept {return iType;}
    const GpUUID&                   TypeUID             (void) const noexcept {return iTypeUID;}
    ContainerTE                     Container           (void) const noexcept {return iContainer;}
    TypeTE                          ContainerKeyType    (void) const noexcept {return iContainerKeyType;}
    std::string_view                Name                (void) const noexcept {return iName;}
    size_t                          Align               (void) const noexcept {return iAlign;}
    size_t                          Size                (void) const noexcept {return iSize;}
    std::ptrdiff_t                  Offset              (void) const noexcept {return iOffset;}
    const GpTypePropFlags&          Flags               (void) const noexcept {return iFlags;}
    bool                            FlagTest            (const GpTypePropFlag::EnumT aFlag) const noexcept;
    std::optional<std::string_view> FlagArg             (const GpTypePropFlag::EnumT aFlag) const noexcept;
    const GpTypePropInfo&           UnwrapContainerKeyProp (void) const;

    const u_int_8&                  Value_UInt8         (const GpTypeStructBase& aStruct) const {return CastValueAsConst<u_int_8>(aStruct);}
    u_int_8&                        Value_UInt8         (GpTypeStructBase& aStruct) const       {return CastValueAs<u_int_8>(aStruct);}
    const s_int_8&                  Value_SInt8         (const GpTypeStructBase& aStruct) const {return CastValueAsConst<s_int_8>(aStruct);}
    s_int_8&                        Value_SInt8         (GpTypeStructBase& aStruct) const       {return CastValueAs<s_int_8>(aStruct);}
    const u_int_16&                 Value_UInt16        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<u_int_16>(aStruct);}
    u_int_16&                       Value_UInt16        (GpTypeStructBase& aStruct) const       {return CastValueAs<u_int_16>(aStruct);}
    const s_int_16&                 Value_SInt16        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<s_int_16>(aStruct);}
    s_int_16&                       Value_SInt16        (GpTypeStructBase& aStruct) const       {return CastValueAs<s_int_16>(aStruct);}
    const u_int_32&                 Value_UInt32        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<u_int_32>(aStruct);}
    u_int_32&                       Value_UInt32        (GpTypeStructBase& aStruct) const       {return CastValueAs<u_int_32>(aStruct);}
    const s_int_32&                 Value_SInt32        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<s_int_32>(aStruct);}
    s_int_32&                       Value_SInt32        (GpTypeStructBase& aStruct) const       {return CastValueAs<s_int_32>(aStruct);}
    const u_int_64&                 Value_UInt64        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<u_int_64>(aStruct);}
    u_int_64&                       Value_UInt64        (GpTypeStructBase& aStruct) const       {return CastValueAs<u_int_64>(aStruct);}
    const s_int_64&                 Value_SInt64        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<s_int_64>(aStruct);}
    s_int_64&                       Value_SInt64        (GpTypeStructBase& aStruct) const       {return CastValueAs<s_int_64>(aStruct);}
    const double&                   Value_Double        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<double>(aStruct);}
    double&                         Value_Double        (GpTypeStructBase& aStruct) const       {return CastValueAs<double>(aStruct);}
    const float&                    Value_Float         (const GpTypeStructBase& aStruct) const {return CastValueAsConst<float>(aStruct);}
    float&                          Value_Float         (GpTypeStructBase& aStruct) const       {return CastValueAs<float>(aStruct);}
    const bool&                     Value_Bool          (const GpTypeStructBase& aStruct) const {return CastValueAsConst<bool>(aStruct);}
    bool&                           Value_Bool          (GpTypeStructBase& aStruct) const       {return CastValueAs<bool>(aStruct);}
    const GpUUID&                   Value_UUID          (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpUUID>(aStruct);}
    GpUUID&                         Value_UUID          (GpTypeStructBase& aStruct) const       {return CastValueAs<GpUUID>(aStruct);}
    const std::string&              Value_String        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<std::string>(aStruct);}
    std::string&                    Value_String        (GpTypeStructBase& aStruct) const       {return CastValueAs<std::string>(aStruct);}
    const GpBytesArray&             Value_BLOB          (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpBytesArray>(aStruct);}
    GpBytesArray&                   Value_BLOB          (GpTypeStructBase& aStruct) const       {return CastValueAs<GpBytesArray>(aStruct);}
    const GpTypeStructBase&         Value_Struct        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpTypeStructBase>(aStruct);}
    GpTypeStructBase&               Value_Struct        (GpTypeStructBase& aStruct) const       {return CastValueAs<GpTypeStructBase>(aStruct);}
    const GpSP<GpTypeStructBase>&   Value_StructSP      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSP<GpTypeStructBase>>(aStruct);}
    GpSP<GpTypeStructBase>&         Value_StructSP      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSP<GpTypeStructBase>>(aStruct);}
    const GpEnum&                   Value_Enum          (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpEnum>(aStruct);}
    GpEnum&                         Value_Enum          (GpTypeStructBase& aStruct) const       {return CastValueAs<GpEnum>(aStruct);}
    const GpEnumFlags&              Value_EnumFlags     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpEnumFlags>(aStruct);}
    GpEnumFlags&                    Value_EnumFlags     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpEnumFlags>(aStruct);}

    const auto&                     Value_Vec_UInt8     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<u_int_8>>(aStruct);}
    auto&                           Value_Vec_UInt8     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<u_int_8>>(aStruct);}
    const auto&                     Value_Vec_SInt8     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<s_int_8>>(aStruct);}
    auto&                           Value_Vec_SInt8     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<s_int_8>>(aStruct);}
    const auto&                     Value_Vec_UInt16    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<u_int_16>>(aStruct);}
    auto&                           Value_Vec_UInt16    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<u_int_16>>(aStruct);}
    const auto&                     Value_Vec_SInt16    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<s_int_16>>(aStruct);}
    auto&                           Value_Vec_SInt16    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<s_int_16>>(aStruct);}
    const auto&                     Value_Vec_UInt32    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<u_int_32>>(aStruct);}
    auto&                           Value_Vec_UInt32    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<u_int_32>>(aStruct);}
    const auto&                     Value_Vec_SInt32    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<s_int_32>>(aStruct);}
    auto&                           Value_Vec_SInt32    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<s_int_32>>(aStruct);}
    const auto&                     Value_Vec_UInt64    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<u_int_64>>(aStruct);}
    auto&                           Value_Vec_UInt64    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<u_int_64>>(aStruct);}
    const auto&                     Value_Vec_SInt64    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<s_int_64>>(aStruct);}
    auto&                           Value_Vec_SInt64    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<s_int_64>>(aStruct);}
    const auto&                     Value_Vec_Double    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<double>>(aStruct);}
    auto&                           Value_Vec_Double    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<double>>(aStruct);}
    const auto&                     Value_Vec_Float     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<float>>(aStruct);}
    auto&                           Value_Vec_Float     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<float>>(aStruct);}
    const auto&                     Value_Vec_Bool      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<bool>>(aStruct);}
    auto&                           Value_Vec_Bool      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<bool>>(aStruct);}
    const auto&                     Value_Vec_UUID      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<GpUUID>>(aStruct);}
    auto&                           Value_Vec_UUID      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<GpUUID>>(aStruct);}
    const auto&                     Value_Vec_String    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<std::string>>(aStruct);}
    auto&                           Value_Vec_String    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<std::string>>(aStruct);}
    const auto&                     Value_Vec_BLOB      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<GpBytesArray>>(aStruct);}
    auto&                           Value_Vec_BLOB      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<GpBytesArray>>(aStruct);}
    //const auto&                   Value_Vec_Struct    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<GpTypeStructBase>>(aStruct);}
    //auto&                         Value_Vec_Struct    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<GpTypeStructBase>>(aStruct);}
    const auto&                     Value_Vec_StructSP  (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<GpSP<GpTypeStructBase>>>(aStruct);}
    auto&                           Value_Vec_StructSP  (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<GpSP<GpTypeStructBase>>>(aStruct);}
    //const auto&                   Value_Vec_Enum      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<GpEnum>>(aStruct);}
    //auto&                         Value_Vec_Enum      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpVector<GpEnum>>(aStruct);}

    const auto&                     Value_List_UInt8    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<u_int_8>>(aStruct);}
    auto&                           Value_List_UInt8    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<u_int_8>>(aStruct);}
    const auto&                     Value_List_SInt8    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<s_int_8>>(aStruct);}
    auto&                           Value_List_SInt8    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<s_int_8>>(aStruct);}
    const auto&                     Value_List_UInt16   (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<u_int_16>>(aStruct);}
    auto&                           Value_List_UInt16   (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<u_int_16>>(aStruct);}
    const auto&                     Value_List_SInt16   (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<s_int_16>>(aStruct);}
    auto&                           Value_List_SInt16   (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<s_int_16>>(aStruct);}
    const auto&                     Value_List_UInt32   (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<u_int_32>>(aStruct);}
    auto&                           Value_List_UInt32   (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<u_int_32>>(aStruct);}
    const auto&                     Value_List_SInt32   (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<s_int_32>>(aStruct);}
    auto&                           Value_List_SInt32   (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<s_int_32>>(aStruct);}
    const auto&                     Value_List_UInt64   (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<u_int_64>>(aStruct);}
    auto&                           Value_List_UInt64   (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<u_int_64>>(aStruct);}
    const auto&                     Value_List_SInt64   (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<s_int_64>>(aStruct);}
    auto&                           Value_List_SInt64   (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<s_int_64>>(aStruct);}
    const auto&                     Value_List_Double   (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<double>>(aStruct);}
    auto&                           Value_List_Double   (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<double>>(aStruct);}
    const auto&                     Value_List_Float    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<float>>(aStruct);}
    auto&                           Value_List_Float    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<float>>(aStruct);}
    const auto&                     Value_List_Bool     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<bool>>(aStruct);}
    auto&                           Value_List_Bool     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<bool>>(aStruct);}
    const auto&                     Value_List_UUID     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<GpUUID>>(aStruct);}
    auto&                           Value_List_UUID     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<GpUUID>>(aStruct);}
    const auto&                     Value_List_String   (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<std::string>>(aStruct);}
    auto&                           Value_List_String   (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<std::string>>(aStruct);}
    const auto&                     Value_List_BLOB     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<GpBytesArray>>(aStruct);}
    auto&                           Value_List_BLOB     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<GpBytesArray>>(aStruct);}
    //const auto&                   Value_List_Struct   (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<GpTypeStructBase>>(aStruct);}
    //auto&                         Value_List_Struct   (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<GpTypeStructBase>>(aStruct);}
    const auto&                     Value_List_StructSP (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<GpSP<GpTypeStructBase>>>(aStruct);}
    auto&                           Value_List_StructSP (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<GpSP<GpTypeStructBase>>>(aStruct);}
    //const auto&                   Value_List_Enum     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpList<GpEnum>>(aStruct);}
    //auto&                         Value_List_Enum     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpList<GpEnum>>(aStruct);}

    const auto&                     Value_Set_UInt8     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<u_int_8>>(aStruct);}
    auto&                           Value_Set_UInt8     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<u_int_8>>(aStruct);}
    const auto&                     Value_Set_SInt8     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<s_int_8>>(aStruct);}
    auto&                           Value_Set_SInt8     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<s_int_8>>(aStruct);}
    const auto&                     Value_Set_UInt16    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<u_int_16>>(aStruct);}
    auto&                           Value_Set_UInt16    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<u_int_16>>(aStruct);}
    const auto&                     Value_Set_SInt16    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<s_int_16>>(aStruct);}
    auto&                           Value_Set_SInt16    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<s_int_16>>(aStruct);}
    const auto&                     Value_Set_UInt32    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<u_int_32>>(aStruct);}
    auto&                           Value_Set_UInt32    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<u_int_32>>(aStruct);}
    const auto&                     Value_Set_SInt32    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<s_int_32>>(aStruct);}
    auto&                           Value_Set_SInt32    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<s_int_32>>(aStruct);}
    const auto&                     Value_Set_UInt64    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<u_int_64>>(aStruct);}
    auto&                           Value_Set_UInt64    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<u_int_64>>(aStruct);}
    const auto&                     Value_Set_SInt64    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<s_int_64>>(aStruct);}
    auto&                           Value_Set_SInt64    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<s_int_64>>(aStruct);}
    const auto&                     Value_Set_Double    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<double>>(aStruct);}
    auto&                           Value_Set_Double    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<double>>(aStruct);}
    const auto&                     Value_Set_Float     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<float>>(aStruct);}
    auto&                           Value_Set_Float     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<float>>(aStruct);}
    const auto&                     Value_Set_Bool      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<bool>>(aStruct);}
    auto&                           Value_Set_Bool      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<bool>>(aStruct);}
    const auto&                     Value_Set_UUID      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<GpUUID>>(aStruct);}
    auto&                           Value_Set_UUID      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<GpUUID>>(aStruct);}
    const auto&                     Value_Set_String    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<std::string>>(aStruct);}
    auto&                           Value_Set_String    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<std::string>>(aStruct);}
    const auto&                     Value_Set_BLOB      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<GpBytesArray>>(aStruct);}
    auto&                           Value_Set_BLOB      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<GpBytesArray>>(aStruct);}
    //const auto&                   Value_Set_Struct    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<GpTypeStructBase>>(aStruct);}
    //auto&                         Value_Set_Struct    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<GpTypeStructBase>>(aStruct);}
    const auto&                     Value_Set_StructSP  (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<GpSP<GpTypeStructBase>>>(aStruct);}
    auto&                           Value_Set_StructSP  (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<GpSP<GpTypeStructBase>>>(aStruct);}
    //const auto&                   Value_Set_Enum      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSet<GpEnum>>(aStruct);}
    //auto&                         Value_Set_Enum      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpSet<GpEnum>>(aStruct);}

    template<typename Key> const auto&      Value_Map_UInt8     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, u_int_8, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_UInt8     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, u_int_8, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_SInt8     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, s_int_8, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_SInt8     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, s_int_8, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_UInt16    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, u_int_16, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_UInt16    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, u_int_16, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_SInt16    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, s_int_16, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_SInt16    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, s_int_16, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_UInt32    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, u_int_32, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_UInt32    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, u_int_32, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_SInt32    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, s_int_32, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_SInt32    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, s_int_32, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_UInt64    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, u_int_64, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_UInt64    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, u_int_64, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_SInt64    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, s_int_64, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_SInt64    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, s_int_64, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_Double    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, double, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_Double    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, double, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_Float     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, float, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_Float     (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, float, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_Bool      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, bool, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_Bool      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, bool, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_UUID      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, GpUUID, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_UUID      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, GpUUID, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_String    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, std::string, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_String    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, std::string, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_BLOB      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, GpBytesArray, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_BLOB      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, GpBytesArray, std::less<>>>(aStruct);}
    //template<typename Key> const auto&    Value_Map_Struct    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, GpTypeStructBase, std::less<>>>(aStruct);}
    //template<typename Key> auto&          Value_Map_Struct    (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, GpTypeStructBase, std::less<>>>(aStruct);}
    template<typename Key> const auto&      Value_Map_StructSP  (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, GpSP<GpTypeStructBase>, std::less<>>>(aStruct);}
    template<typename Key> auto&            Value_Map_StructSP  (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, GpSP<GpTypeStructBase>, std::less<>>>(aStruct);}
    //template<typename Key> const auto&    Value_Map_Enum      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpMap<Key, GpEnum>, std::less<>>(aStruct);}
    //template<typename Key> auto&          Value_Map_Enum      (GpTypeStructBase& aStruct) const       {return CastValueAs<GpMap<Key, GpEnum>, std::less<>>(aStruct);}

private:
    template<typename T>
    const T&                        CastValueAsConst    (const GpTypeStructBase& aStruct) const;

    template<typename T>
    T&                              CastValueAs         (GpTypeStructBase& aStruct) const;

private:
    TypeTE                  iType;
    GpUUID                  iTypeUID;   //Only for structs
    ContainerTE             iContainer;
    TypeTE                  iContainerKeyType;
    std::string             iName;
    size_t                  iAlign      = 0;
    size_t                  iSize       = 0;
    std::ptrdiff_t          iOffset     = 0;
    GpTypePropFlags         iFlags;
};

template<typename T>
const T&    GpTypePropInfo::CastValueAsConst (const GpTypeStructBase& aStruct) const
{
    return *reinterpret_cast<const T*>(reinterpret_cast<const char*>(&aStruct) + iOffset);
}

template<typename T>
T&  GpTypePropInfo::CastValueAs (GpTypeStructBase& aStruct) const
{
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(&aStruct) + iOffset);
}

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
