#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "../UIDs/GpUUID.hpp"
#include "../Classes/GpClassesDefines.hpp"
#include "../Containers/GpContainersT.hpp"
#include "GpType.hpp"
#include "GpTypeContainer.hpp"

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

public:
                                    GpTypePropInfo      (void) noexcept;
                                    GpTypePropInfo      (const TypeTE           aType,
                                                         const GpUUID           aStructTypeUID,
                                                         const ContainerTE      aContainer,
                                                         const TypeTE           aContainerKeyType,
                                                         std::string&&          aName,
                                                         const size_t           aAlign,
                                                         const size_t           aSize,
                                                         const std::ptrdiff_t   aOffset) noexcept;
                                    GpTypePropInfo      (const GpTypePropInfo& aPropInfo);
                                    GpTypePropInfo      (GpTypePropInfo&& aPropInfo) noexcept;
                                    ~GpTypePropInfo     (void) noexcept;

    GpTypePropInfo&                 operator=           (const GpTypePropInfo& aPropInfo);
    GpTypePropInfo&                 operator=           (GpTypePropInfo&& aPropInfo) noexcept;

    TypeTE                          Type                (void) const noexcept {return iType;}
    const GpUUID&                   StructTypeUID       (void) const noexcept {return iStructTypeUID;}
    ContainerTE                     Container           (void) const noexcept {return iContainer;}
    TypeTE                          ContainerKeyType    (void) const noexcept {return iContainerKeyType;}
    std::string_view                Name                (void) const noexcept {return iName;}
    size_t                          Align               (void) const noexcept {return iAlign;}
    size_t                          Size                (void) const noexcept {return iSize;}
    std::ptrdiff_t                  Offset              (void) const noexcept {return iOffset;}

    const u_int_8&                  Value_UInt8         (const GpTypeStructBase& aStruct) const {return CastValueAsConst<u_int_8>(aStruct);}
    u_int_8&                        Value_UInt8         (GpTypeStructBase& aStruct) const {return CastValueAs<u_int_8>(aStruct);}
    const s_int_8&                  Value_SInt8         (const GpTypeStructBase& aStruct) const {return CastValueAsConst<s_int_8>(aStruct);}
    s_int_8&                        Value_SInt8         (GpTypeStructBase& aStruct) const {return CastValueAs<s_int_8>(aStruct);}
    const u_int_16&                 Value_UInt16        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<u_int_16>(aStruct);}
    u_int_16&                       Value_UInt16        (GpTypeStructBase& aStruct) const {return CastValueAs<u_int_16>(aStruct);}
    const s_int_16&                 Value_SInt16        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<s_int_16>(aStruct);}
    s_int_16&                       Value_SInt16        (GpTypeStructBase& aStruct) const {return CastValueAs<s_int_16>(aStruct);}
    const u_int_32&                 Value_UInt32        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<u_int_32>(aStruct);}
    u_int_32&                       Value_UInt32        (GpTypeStructBase& aStruct) const {return CastValueAs<u_int_32>(aStruct);}
    const s_int_32&                 Value_SInt32        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<s_int_32>(aStruct);}
    s_int_32&                       Value_SInt32        (GpTypeStructBase& aStruct) const {return CastValueAs<s_int_32>(aStruct);}
    const u_int_64&                 Value_UInt64        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<u_int_64>(aStruct);}
    u_int_64&                       Value_UInt64        (GpTypeStructBase& aStruct) const {return CastValueAs<u_int_64>(aStruct);}
    const s_int_64&                 Value_SInt64        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<s_int_64>(aStruct);}
    s_int_64&                       Value_SInt64        (GpTypeStructBase& aStruct) const {return CastValueAs<s_int_64>(aStruct);}
    const double&                   Value_Double        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<double>(aStruct);}
    double&                         Value_Double        (GpTypeStructBase& aStruct) const {return CastValueAs<double>(aStruct);}
    const float&                    Value_Float         (const GpTypeStructBase& aStruct) const {return CastValueAsConst<float>(aStruct);}
    float&                          Value_Float         (GpTypeStructBase& aStruct) const {return CastValueAs<float>(aStruct);}
    const bool&                     Value_Bool          (const GpTypeStructBase& aStruct) const {return CastValueAsConst<bool>(aStruct);}
    bool&                           Value_Bool          (GpTypeStructBase& aStruct) const {return CastValueAs<bool>(aStruct);}
    const GpUUID&                   Value_UUID          (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpUUID>(aStruct);}
    GpUUID&                         Value_UUID          (GpTypeStructBase& aStruct) const {return CastValueAs<GpUUID>(aStruct);}
    const std::string&              Value_String        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<std::string>(aStruct);}
    std::string&                    Value_String        (GpTypeStructBase& aStruct) const {return CastValueAs<std::string>(aStruct);}
    const GpBytesArray&             Value_BLOB          (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpBytesArray>(aStruct);}
    GpBytesArray&                   Value_BLOB          (GpTypeStructBase& aStruct) const {return CastValueAs<GpBytesArray>(aStruct);}
    const GpTypeStructBase&         Value_Struct        (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpTypeStructBase>(aStruct);}
    GpTypeStructBase&               Value_Struct        (GpTypeStructBase& aStruct) const {return CastValueAs<GpTypeStructBase>(aStruct);}
    const GpSP<GpTypeStructBase>&   Value_StructSP      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpSP<GpTypeStructBase>>(aStruct);}
    GpSP<GpTypeStructBase>&         Value_StructSP      (GpTypeStructBase& aStruct) const {return CastValueAs<GpSP<GpTypeStructBase>>(aStruct);}
    const GpEnum&                   Value_Enum          (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpEnum>(aStruct);}
    GpEnum&                         Value_Enum          (GpTypeStructBase& aStruct) const {return CastValueAs<GpEnum>(aStruct);}

    const GpVector<u_int_8>&        Value_Vec_UInt8     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<u_int_8>>(aStruct);}
    GpVector<u_int_8>&              Value_Vec_UInt8     (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<u_int_8>>(aStruct);}
    const GpVector<s_int_8>&        Value_Vec_SInt8     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<s_int_8>>(aStruct);}
    GpVector<s_int_8>&              Value_Vec_SInt8     (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<s_int_8>>(aStruct);}
    const GpVector<u_int_16>&       Value_Vec_UInt16    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<u_int_16>>(aStruct);}
    GpVector<u_int_16>&             Value_Vec_UInt16    (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<u_int_16>>(aStruct);}
    const GpVector<s_int_16>&       Value_Vec_SInt16    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<s_int_16>>(aStruct);}
    GpVector<s_int_16>&             Value_Vec_SInt16    (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<s_int_16>>(aStruct);}
    const GpVector<u_int_32>&       Value_Vec_UInt32    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<u_int_32>>(aStruct);}
    GpVector<u_int_32>&             Value_Vec_UInt32    (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<u_int_32>>(aStruct);}
    const GpVector<s_int_32>&       Value_Vec_SInt32    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<s_int_32>>(aStruct);}
    GpVector<s_int_32>&             Value_Vec_SInt32    (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<s_int_32>>(aStruct);}
    const GpVector<u_int_64>&       Value_Vec_UInt64    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<u_int_64>>(aStruct);}
    GpVector<u_int_64>&             Value_Vec_UInt64    (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<u_int_64>>(aStruct);}
    const GpVector<s_int_64>&       Value_Vec_SInt64    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<s_int_64>>(aStruct);}
    GpVector<s_int_64>&             Value_Vec_SInt64    (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<s_int_64>>(aStruct);}
    const GpVector<double>&         Value_Vec_Double    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<double>>(aStruct);}
    GpVector<double>&               Value_Vec_Double    (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<double>>(aStruct);}
    const GpVector<float>&          Value_Vec_Float     (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<float>>(aStruct);}
    GpVector<float>&                Value_Vec_Float     (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<float>>(aStruct);}
    const GpVector<bool>&           Value_Vec_Bool      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<bool>>(aStruct);}
    GpVector<bool>&                 Value_Vec_Bool      (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<bool>>(aStruct);}
    const GpVector<GpUUID>&         Value_Vec_UUID      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<GpUUID>>(aStruct);}
    GpVector<GpUUID>&               Value_Vec_UUID      (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<GpUUID>>(aStruct);}
    const GpVector<std::string>&    Value_Vec_String    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<std::string>>(aStruct);}
    GpVector<std::string>&          Value_Vec_String    (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<std::string>>(aStruct);}
    const GpVector<GpBytesArray>&   Value_Vec_BLOB      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<GpBytesArray>>(aStruct);}
    GpVector<GpBytesArray>&         Value_Vec_BLOB      (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<GpBytesArray>>(aStruct);}
    //const GpTypeStructBase&       Value_Vec_Struct    (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<GpTypeStructBase>>(aStruct);}
    //GpTypeStructBase&             Value_Vec_Struct    (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<GpTypeStructBase>>(aStruct);}
    const GpVector<GpSP<GpTypeStructBase>>& Value_Vec_StructSP  (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<GpSP<GpTypeStructBase>>>(aStruct);}
    GpVector<GpSP<GpTypeStructBase>>&       Value_Vec_StructSP  (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<GpSP<GpTypeStructBase>>>(aStruct);}
    //const GpEnum&                 Value_Vec_Enum      (const GpTypeStructBase& aStruct) const {return CastValueAsConst<GpVector<GpEnum>>(aStruct);}
    //GpEnum&                       Value_Vec_Enum      (GpTypeStructBase& aStruct) const {return CastValueAs<GpVector<GpEnum>>(aStruct);}

private:
    template<typename T>
    const T&                        CastValueAsConst    (const GpTypeStructBase& aStruct) const;

    template<typename T>
    T&                              CastValueAs         (GpTypeStructBase& aStruct) const;

private:
    TypeTE                  iType;
    GpUUID                  iStructTypeUID;
    ContainerTE             iContainer;
    TypeTE                  iContainerKeyType;
    std::string             iName;
    size_t                  iAlign      = 0;
    size_t                  iSize       = 0;
    std::ptrdiff_t          iOffset     = 0;
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
