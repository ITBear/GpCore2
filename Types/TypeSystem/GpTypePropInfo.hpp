#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include <functional>

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

    using TypeT                 = GpType;
    using TypeTE                = TypeT::EnumT;
    using ContainerT            = GpTypeContainer;
    using ContainerTE           = ContainerT::EnumT;
    using FlagT                 = GpTypePropFlag;
    using FlagTE                = FlagT::EnumT;

    using ProcessCustomFn       = std::function<void(void* aPropPtr)>;
    using ProcessCustomFnOpt    = std::optional<ProcessCustomFn>;

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
                                                         const GpTypePropFlags& aFlags,
                                                         ProcessCustomFnOpt     aConstructCustomFn,
                                                         ProcessCustomFnOpt     aDestructCustomFn);
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

    void                            ConstructCustom     (void* aDataPtr) const;
    void                            DestructCustom      (void* aDataPtr) const;

    const u_int_8&                  Value_UInt8         (const void* aDataPtr) const {return CastValueAsConst<u_int_8>(aDataPtr);}
    u_int_8&                        Value_UInt8         (void* aDataPtr) const      {return CastValueAs<u_int_8>(aDataPtr);}
    const s_int_8&                  Value_SInt8         (const void* aDataPtr) const {return CastValueAsConst<s_int_8>(aDataPtr);}
    s_int_8&                        Value_SInt8         (void* aDataPtr) const      {return CastValueAs<s_int_8>(aDataPtr);}
    const u_int_16&                 Value_UInt16        (const void* aDataPtr) const {return CastValueAsConst<u_int_16>(aDataPtr);}
    u_int_16&                       Value_UInt16        (void* aDataPtr) const      {return CastValueAs<u_int_16>(aDataPtr);}
    const s_int_16&                 Value_SInt16        (const void* aDataPtr) const {return CastValueAsConst<s_int_16>(aDataPtr);}
    s_int_16&                       Value_SInt16        (void* aDataPtr) const      {return CastValueAs<s_int_16>(aDataPtr);}
    const u_int_32&                 Value_UInt32        (const void* aDataPtr) const {return CastValueAsConst<u_int_32>(aDataPtr);}
    u_int_32&                       Value_UInt32        (void* aDataPtr) const      {return CastValueAs<u_int_32>(aDataPtr);}
    const s_int_32&                 Value_SInt32        (const void* aDataPtr) const {return CastValueAsConst<s_int_32>(aDataPtr);}
    s_int_32&                       Value_SInt32        (void* aDataPtr) const      {return CastValueAs<s_int_32>(aDataPtr);}
    const u_int_64&                 Value_UInt64        (const void* aDataPtr) const {return CastValueAsConst<u_int_64>(aDataPtr);}
    u_int_64&                       Value_UInt64        (void* aDataPtr) const      {return CastValueAs<u_int_64>(aDataPtr);}
    const s_int_64&                 Value_SInt64        (const void* aDataPtr) const {return CastValueAsConst<s_int_64>(aDataPtr);}
    s_int_64&                       Value_SInt64        (void* aDataPtr) const      {return CastValueAs<s_int_64>(aDataPtr);}
    const double&                   Value_Double        (const void* aDataPtr) const {return CastValueAsConst<double>(aDataPtr);}
    double&                         Value_Double        (void* aDataPtr) const      {return CastValueAs<double>(aDataPtr);}
    const float&                    Value_Float         (const void* aDataPtr) const {return CastValueAsConst<float>(aDataPtr);}
    float&                          Value_Float         (void* aDataPtr) const      {return CastValueAs<float>(aDataPtr);}
    const bool&                     Value_Bool          (const void* aDataPtr) const {return CastValueAsConst<bool>(aDataPtr);}
    bool&                           Value_Bool          (void* aDataPtr) const      {return CastValueAs<bool>(aDataPtr);}
    const GpUUID&                   Value_UUID          (const void* aDataPtr) const {return CastValueAsConst<GpUUID>(aDataPtr);}
    GpUUID&                         Value_UUID          (void* aDataPtr) const      {return CastValueAs<GpUUID>(aDataPtr);}
    const std::string&              Value_String        (const void* aDataPtr) const {return CastValueAsConst<std::string>(aDataPtr);}
    std::string&                    Value_String        (void* aDataPtr) const      {return CastValueAs<std::string>(aDataPtr);}
    const GpBytesArray&             Value_BLOB          (const void* aDataPtr) const {return CastValueAsConst<GpBytesArray>(aDataPtr);}
    GpBytesArray&                   Value_BLOB          (void* aDataPtr) const      {return CastValueAs<GpBytesArray>(aDataPtr);}
    const GpTypeStructBase&         Value_Struct        (const void* aDataPtr) const {return CastValueAsConst<GpTypeStructBase>(aDataPtr);}
    GpTypeStructBase&               Value_Struct        (void* aDataPtr) const      {return CastValueAs<GpTypeStructBase>(aDataPtr);}
    const GpSP<GpTypeStructBase>&   Value_StructSP      (const void* aDataPtr) const {return CastValueAsConst<GpSP<GpTypeStructBase>>(aDataPtr);}
    GpSP<GpTypeStructBase>&         Value_StructSP      (void* aDataPtr) const      {return CastValueAs<GpSP<GpTypeStructBase>>(aDataPtr);}
    const GpEnum&                   Value_Enum          (const void* aDataPtr) const {return CastValueAsConst<GpEnum>(aDataPtr);}
    GpEnum&                         Value_Enum          (void* aDataPtr) const      {return CastValueAs<GpEnum>(aDataPtr);}
    const GpEnumFlags&              Value_EnumFlags     (const void* aDataPtr) const {return CastValueAsConst<GpEnumFlags>(aDataPtr);}
    GpEnumFlags&                    Value_EnumFlags     (void* aDataPtr) const      {return CastValueAs<GpEnumFlags>(aDataPtr);}

    const auto&                     Vec_UInt8           (const void* aDataPtr) const {return CastValueAsConst<GpVector<u_int_8>>(aDataPtr);}
    auto&                           Vec_UInt8           (void* aDataPtr) const      {return CastValueAs<GpVector<u_int_8>>(aDataPtr);}
    const auto&                     Vec_SInt8           (const void* aDataPtr) const {return CastValueAsConst<GpVector<s_int_8>>(aDataPtr);}
    auto&                           Vec_SInt8           (void* aDataPtr) const      {return CastValueAs<GpVector<s_int_8>>(aDataPtr);}
    const auto&                     Vec_UInt16          (const void* aDataPtr) const {return CastValueAsConst<GpVector<u_int_16>>(aDataPtr);}
    auto&                           Vec_UInt16          (void* aDataPtr) const      {return CastValueAs<GpVector<u_int_16>>(aDataPtr);}
    const auto&                     Vec_SInt16          (const void* aDataPtr) const {return CastValueAsConst<GpVector<s_int_16>>(aDataPtr);}
    auto&                           Vec_SInt16          (void* aDataPtr) const      {return CastValueAs<GpVector<s_int_16>>(aDataPtr);}
    const auto&                     Vec_UInt32          (const void* aDataPtr) const {return CastValueAsConst<GpVector<u_int_32>>(aDataPtr);}
    auto&                           Vec_UInt32          (void* aDataPtr) const      {return CastValueAs<GpVector<u_int_32>>(aDataPtr);}
    const auto&                     Vec_SInt32          (const void* aDataPtr) const {return CastValueAsConst<GpVector<s_int_32>>(aDataPtr);}
    auto&                           Vec_SInt32          (void* aDataPtr) const      {return CastValueAs<GpVector<s_int_32>>(aDataPtr);}
    const auto&                     Vec_UInt64          (const void* aDataPtr) const {return CastValueAsConst<GpVector<u_int_64>>(aDataPtr);}
    auto&                           Vec_UInt64          (void* aDataPtr) const      {return CastValueAs<GpVector<u_int_64>>(aDataPtr);}
    const auto&                     Vec_SInt64          (const void* aDataPtr) const {return CastValueAsConst<GpVector<s_int_64>>(aDataPtr);}
    auto&                           Vec_SInt64          (void* aDataPtr) const      {return CastValueAs<GpVector<s_int_64>>(aDataPtr);}
    const auto&                     Vec_Double          (const void* aDataPtr) const {return CastValueAsConst<GpVector<double>>(aDataPtr);}
    auto&                           Vec_Double          (void* aDataPtr) const      {return CastValueAs<GpVector<double>>(aDataPtr);}
    const auto&                     Vec_Float           (const void* aDataPtr) const {return CastValueAsConst<GpVector<float>>(aDataPtr);}
    auto&                           Vec_Float           (void* aDataPtr) const      {return CastValueAs<GpVector<float>>(aDataPtr);}
    const auto&                     Vec_Bool            (const void* aDataPtr) const {return CastValueAsConst<GpVector<bool>>(aDataPtr);}
    auto&                           Vec_Bool            (void* aDataPtr) const      {return CastValueAs<GpVector<bool>>(aDataPtr);}
    const auto&                     Vec_UUID            (const void* aDataPtr) const {return CastValueAsConst<GpVector<GpUUID>>(aDataPtr);}
    auto&                           Vec_UUID            (void* aDataPtr) const      {return CastValueAs<GpVector<GpUUID>>(aDataPtr);}
    const auto&                     Vec_String          (const void* aDataPtr) const {return CastValueAsConst<GpVector<std::string>>(aDataPtr);}
    auto&                           Vec_String          (void* aDataPtr) const      {return CastValueAs<GpVector<std::string>>(aDataPtr);}
    const auto&                     Vec_BLOB            (const void* aDataPtr) const {return CastValueAsConst<GpVector<GpBytesArray>>(aDataPtr);}
    auto&                           Vec_BLOB            (void* aDataPtr) const      {return CastValueAs<GpVector<GpBytesArray>>(aDataPtr);}
    //const auto&                   Vec_Struct          (const void* aDataPtr) const {return CastValueAsConst<GpVector<GpTypeStructBase>>(aDataPtr);}
    //auto&                         Vec_Struct          (void* aDataPtr) const      {return CastValueAs<GpVector<GpTypeStructBase>>(aDataPtr);}
    const auto&                     Vec_StructSP        (const void* aDataPtr) const {return CastValueAsConst<GpVector<GpSP<GpTypeStructBase>>>(aDataPtr);}
    auto&                           Vec_StructSP        (void* aDataPtr) const      {return CastValueAs<GpVector<GpSP<GpTypeStructBase>>>(aDataPtr);}
    //const auto&                   Vec_Enum            (const void* aDataPtr) const {return CastValueAsConst<GpVector<GpEnum>>(aDataPtr);}
    //auto&                         Vec_Enum            (void* aDataPtr) const      {return CastValueAs<GpVector<GpEnum>>(aDataPtr);}

    const auto&                     List_UInt8          (const void* aDataPtr) const {return CastValueAsConst<GpList<u_int_8>>(aDataPtr);}
    auto&                           List_UInt8          (void* aDataPtr) const      {return CastValueAs<GpList<u_int_8>>(aDataPtr);}
    const auto&                     List_SInt8          (const void* aDataPtr) const {return CastValueAsConst<GpList<s_int_8>>(aDataPtr);}
    auto&                           List_SInt8          (void* aDataPtr) const      {return CastValueAs<GpList<s_int_8>>(aDataPtr);}
    const auto&                     List_UInt16         (const void* aDataPtr) const {return CastValueAsConst<GpList<u_int_16>>(aDataPtr);}
    auto&                           List_UInt16         (void* aDataPtr) const      {return CastValueAs<GpList<u_int_16>>(aDataPtr);}
    const auto&                     List_SInt16         (const void* aDataPtr) const {return CastValueAsConst<GpList<s_int_16>>(aDataPtr);}
    auto&                           List_SInt16         (void* aDataPtr) const      {return CastValueAs<GpList<s_int_16>>(aDataPtr);}
    const auto&                     List_UInt32         (const void* aDataPtr) const {return CastValueAsConst<GpList<u_int_32>>(aDataPtr);}
    auto&                           List_UInt32         (void* aDataPtr) const      {return CastValueAs<GpList<u_int_32>>(aDataPtr);}
    const auto&                     List_SInt32         (const void* aDataPtr) const {return CastValueAsConst<GpList<s_int_32>>(aDataPtr);}
    auto&                           List_SInt32         (void* aDataPtr) const      {return CastValueAs<GpList<s_int_32>>(aDataPtr);}
    const auto&                     List_UInt64         (const void* aDataPtr) const {return CastValueAsConst<GpList<u_int_64>>(aDataPtr);}
    auto&                           List_UInt64         (void* aDataPtr) const      {return CastValueAs<GpList<u_int_64>>(aDataPtr);}
    const auto&                     List_SInt64         (const void* aDataPtr) const {return CastValueAsConst<GpList<s_int_64>>(aDataPtr);}
    auto&                           List_SInt64         (void* aDataPtr) const      {return CastValueAs<GpList<s_int_64>>(aDataPtr);}
    const auto&                     List_Double         (const void* aDataPtr) const {return CastValueAsConst<GpList<double>>(aDataPtr);}
    auto&                           List_Double         (void* aDataPtr) const      {return CastValueAs<GpList<double>>(aDataPtr);}
    const auto&                     List_Float          (const void* aDataPtr) const {return CastValueAsConst<GpList<float>>(aDataPtr);}
    auto&                           List_Float          (void* aDataPtr) const      {return CastValueAs<GpList<float>>(aDataPtr);}
    const auto&                     List_Bool           (const void* aDataPtr) const {return CastValueAsConst<GpList<bool>>(aDataPtr);}
    auto&                           List_Bool           (void* aDataPtr) const      {return CastValueAs<GpList<bool>>(aDataPtr);}
    const auto&                     List_UUID           (const void* aDataPtr) const {return CastValueAsConst<GpList<GpUUID>>(aDataPtr);}
    auto&                           List_UUID           (void* aDataPtr) const      {return CastValueAs<GpList<GpUUID>>(aDataPtr);}
    const auto&                     List_String         (const void* aDataPtr) const {return CastValueAsConst<GpList<std::string>>(aDataPtr);}
    auto&                           List_String         (void* aDataPtr) const      {return CastValueAs<GpList<std::string>>(aDataPtr);}
    const auto&                     List_BLOB           (const void* aDataPtr) const {return CastValueAsConst<GpList<GpBytesArray>>(aDataPtr);}
    auto&                           List_BLOB           (void* aDataPtr) const      {return CastValueAs<GpList<GpBytesArray>>(aDataPtr);}
    //const auto&                   List_Struct         (const void* aDataPtr) const {return CastValueAsConst<GpList<GpTypeStructBase>>(aDataPtr);}
    //auto&                         List_Struct         (void* aDataPtr) const      {return CastValueAs<GpList<GpTypeStructBase>>(aDataPtr);}
    const auto&                     List_StructSP       (const void* aDataPtr) const {return CastValueAsConst<GpList<GpSP<GpTypeStructBase>>>(aDataPtr);}
    auto&                           List_StructSP       (void* aDataPtr) const      {return CastValueAs<GpList<GpSP<GpTypeStructBase>>>(aDataPtr);}
    //const auto&                   List_Enum           (const void* aDataPtr) const {return CastValueAsConst<GpList<GpEnum>>(aDataPtr);}
    //auto&                         List_Enum           (void* aDataPtr) const      {return CastValueAs<GpList<GpEnum>>(aDataPtr);}

    const auto&                     Set_UInt8           (const void* aDataPtr) const {return CastValueAsConst<GpSet<u_int_8>>(aDataPtr);}
    auto&                           Set_UInt8           (void* aDataPtr) const      {return CastValueAs<GpSet<u_int_8>>(aDataPtr);}
    const auto&                     Set_SInt8           (const void* aDataPtr) const {return CastValueAsConst<GpSet<s_int_8>>(aDataPtr);}
    auto&                           Set_SInt8           (void* aDataPtr) const      {return CastValueAs<GpSet<s_int_8>>(aDataPtr);}
    const auto&                     Set_UInt16          (const void* aDataPtr) const {return CastValueAsConst<GpSet<u_int_16>>(aDataPtr);}
    auto&                           Set_UInt16          (void* aDataPtr) const      {return CastValueAs<GpSet<u_int_16>>(aDataPtr);}
    const auto&                     Set_SInt16          (const void* aDataPtr) const {return CastValueAsConst<GpSet<s_int_16>>(aDataPtr);}
    auto&                           Set_SInt16          (void* aDataPtr) const      {return CastValueAs<GpSet<s_int_16>>(aDataPtr);}
    const auto&                     Set_UInt32          (const void* aDataPtr) const {return CastValueAsConst<GpSet<u_int_32>>(aDataPtr);}
    auto&                           Set_UInt32          (void* aDataPtr) const      {return CastValueAs<GpSet<u_int_32>>(aDataPtr);}
    const auto&                     Set_SInt32          (const void* aDataPtr) const {return CastValueAsConst<GpSet<s_int_32>>(aDataPtr);}
    auto&                           Set_SInt32          (void* aDataPtr) const      {return CastValueAs<GpSet<s_int_32>>(aDataPtr);}
    const auto&                     Set_UInt64          (const void* aDataPtr) const {return CastValueAsConst<GpSet<u_int_64>>(aDataPtr);}
    auto&                           Set_UInt64          (void* aDataPtr) const      {return CastValueAs<GpSet<u_int_64>>(aDataPtr);}
    const auto&                     Set_SInt64          (const void* aDataPtr) const {return CastValueAsConst<GpSet<s_int_64>>(aDataPtr);}
    auto&                           Set_SInt64          (void* aDataPtr) const      {return CastValueAs<GpSet<s_int_64>>(aDataPtr);}
    const auto&                     Set_Double          (const void* aDataPtr) const {return CastValueAsConst<GpSet<double>>(aDataPtr);}
    auto&                           Set_Double          (void* aDataPtr) const      {return CastValueAs<GpSet<double>>(aDataPtr);}
    const auto&                     Set_Float           (const void* aDataPtr) const {return CastValueAsConst<GpSet<float>>(aDataPtr);}
    auto&                           Set_Float           (void* aDataPtr) const      {return CastValueAs<GpSet<float>>(aDataPtr);}
    const auto&                     Set_Bool            (const void* aDataPtr) const {return CastValueAsConst<GpSet<bool>>(aDataPtr);}
    auto&                           Set_Bool            (void* aDataPtr) const      {return CastValueAs<GpSet<bool>>(aDataPtr);}
    const auto&                     Set_UUID            (const void* aDataPtr) const {return CastValueAsConst<GpSet<GpUUID>>(aDataPtr);}
    auto&                           Set_UUID            (void* aDataPtr) const      {return CastValueAs<GpSet<GpUUID>>(aDataPtr);}
    const auto&                     Set_String          (const void* aDataPtr) const {return CastValueAsConst<GpSet<std::string>>(aDataPtr);}
    auto&                           Set_String          (void* aDataPtr) const      {return CastValueAs<GpSet<std::string>>(aDataPtr);}
    const auto&                     Set_BLOB            (const void* aDataPtr) const {return CastValueAsConst<GpSet<GpBytesArray>>(aDataPtr);}
    auto&                           Set_BLOB            (void* aDataPtr) const      {return CastValueAs<GpSet<GpBytesArray>>(aDataPtr);}
    //const auto&                   Set_Struct          (const void* aDataPtr) const {return CastValueAsConst<GpSet<GpTypeStructBase>>(aDataPtr);}
    //auto&                         Set_Struct          (void* aDataPtr) const      {return CastValueAs<GpSet<GpTypeStructBase>>(aDataPtr);}
    const auto&                     Set_StructSP        (const void* aDataPtr) const {return CastValueAsConst<GpSet<GpSP<GpTypeStructBase>>>(aDataPtr);}
    auto&                           Set_StructSP        (void* aDataPtr) const      {return CastValueAs<GpSet<GpSP<GpTypeStructBase>>>(aDataPtr);}
    //const auto&                   Set_Enum            (const void* aDataPtr) const {return CastValueAsConst<GpSet<GpEnum>>(aDataPtr);}
    //auto&                         Set_Enum            (void* aDataPtr) const      {return CastValueAs<GpSet<GpEnum>>(aDataPtr);}

    template<typename Key> const auto&  Map_UInt8       (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, u_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UInt8       (void* aDataPtr) const      {return CastValueAs<GpMap<Key, u_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SInt8       (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, s_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SInt8       (void* aDataPtr) const      {return CastValueAs<GpMap<Key, s_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UInt16      (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, u_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UInt16      (void* aDataPtr) const      {return CastValueAs<GpMap<Key, u_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SInt16      (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, s_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SInt16      (void* aDataPtr) const      {return CastValueAs<GpMap<Key, s_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UInt32      (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, u_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UInt32      (void* aDataPtr) const      {return CastValueAs<GpMap<Key, u_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SInt32      (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, s_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SInt32      (void* aDataPtr) const      {return CastValueAs<GpMap<Key, s_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UInt64      (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, u_int_64, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UInt64      (void* aDataPtr) const      {return CastValueAs<GpMap<Key, u_int_64, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SInt64      (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, s_int_64, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SInt64      (void* aDataPtr) const      {return CastValueAs<GpMap<Key, s_int_64, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_Double      (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, double, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_Double      (void* aDataPtr) const      {return CastValueAs<GpMap<Key, double, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_Float       (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, float, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_Float       (void* aDataPtr) const      {return CastValueAs<GpMap<Key, float, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_Bool        (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, bool, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_Bool        (void* aDataPtr) const      {return CastValueAs<GpMap<Key, bool, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UUID        (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, GpUUID, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UUID        (void* aDataPtr) const      {return CastValueAs<GpMap<Key, GpUUID, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_String      (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, std::string, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_String      (void* aDataPtr) const      {return CastValueAs<GpMap<Key, std::string, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_BLOB        (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, GpBytesArray, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_BLOB        (void* aDataPtr) const      {return CastValueAs<GpMap<Key, GpBytesArray, std::less<>>>(aDataPtr);}
    //template<typename Key> const auto&Map_Struct      (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, GpTypeStructBase, std::less<>>>(aDataPtr);}
    //template<typename Key> auto&      Map_Struct      (void* aDataPtr) const      {return CastValueAs<GpMap<Key, GpTypeStructBase, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_StructSP    (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, GpSP<GpTypeStructBase>, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_StructSP    (void* aDataPtr) const      {return CastValueAs<GpMap<Key, GpSP<GpTypeStructBase>, std::less<>>>(aDataPtr);}
    //template<typename Key> const auto&Map_Enum        (const void* aDataPtr) const {return CastValueAsConst<GpMap<Key, GpEnum>, std::less<>>(aDataPtr);}
    //template<typename Key> auto&      Map_Enum        (void* aDataPtr) const      {return CastValueAs<GpMap<Key, GpEnum>, std::less<>>(aDataPtr);}

private:
    template<typename T>
    const T&                            CastValueAsConst(const void* aDataPtr) const;

    template<typename T>
    T&                                  CastValueAs     (void* aDataPtr) const;

    inline const void*                  PropPtrConst    (const void* aDataPtr) const;
    inline void*                        PropPtr         (void* aDataPtr) const;

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

    ProcessCustomFnOpt      iConstructCustomFn;
    ProcessCustomFnOpt      iDestructCustomFn;
};

template<typename T>
const T&    GpTypePropInfo::CastValueAsConst (const void* aDataPtr) const
{
    return *reinterpret_cast<const T*>(PropPtrConst(aDataPtr));
}

template<typename T>
T&  GpTypePropInfo::CastValueAs (void* aDataPtr) const
{
    return *reinterpret_cast<T*>(PropPtr(aDataPtr));
}

const void* GpTypePropInfo::PropPtrConst (const void* aDataPtr) const
{
    return reinterpret_cast<const char*>(aDataPtr) + iOffset;
}

void*   GpTypePropInfo::PropPtr (void* aDataPtr) const
{
    return reinterpret_cast<char*>(aDataPtr) + iOffset;
}

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
