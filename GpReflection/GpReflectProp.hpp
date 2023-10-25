#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include <functional>

#include "GpReflectType.hpp"
#include "GpReflectContainerType.hpp"
#include "GpReflectPropFlags.hpp"
#include "../GpUtils/Types/Containers/GpVectorWrapper.hpp"

namespace GPlatform {

class GpReflectObject;

using GpVectorReflectObjWrapBase    = GpVectorWrapperBase<GpReflectObject>;

template<typename T>
using GpVectorReflectObjWrap        = GpVectorWrapper<T, GpReflectObject, std::vector>;

class GP_REFLECTION_API GpReflectProp
{
public:
    CLASS_DD(GpReflectProp)

    using TypeT                 = GpReflectType;
    using TypeTE                = TypeT::EnumT;
    using ContainerT            = GpReflectContainerType;
    using ContainerTE           = ContainerT::EnumT;
    using FlagT                 = GpReflectPropFlag;
    using FlagTE                = FlagT::EnumT;
    using FlagsT                = GpReflectPropFlags;
    using FlagArgsT             = std::map<GpReflectPropFlags::EnumT, std::u8string>;

    using FromStringFnT         = std::function<void(const GpReflectProp& aProp, void* aDataPtr, std::u8string_view aSrc)>;
    using FromStringFnMapT      = std::map<std::u8string, FromStringFnT, std::less<>>;

    using GenFnT                = std::function<void(const GpReflectProp& aProp, void* aDataPtr)>;
    using GenFnOptT             = std::optional<GenFnT>;
    using ProcessCustomFnT      = std::function<void(void* aPropPtr)>;
    using ProcessCustomFnOptT   = std::optional<ProcessCustomFnT>;

public:
                                    GpReflectProp           (void) noexcept;
                                    GpReflectProp           (const TypeTE           aType,
                                                             const GpUUID           aModelUid,
                                                             const ContainerTE      aContainer,
                                                             const TypeTE           aContainerKeyType,
                                                             std::u8string&&        aName,
                                                             const size_t           aAlign,
                                                             const size_t           aSize,
                                                             const std::ptrdiff_t   aOffset,
                                                             const FlagsT           aFlags,
                                                             FlagArgsT&&            aFlagArgs,
                                                             GenFnOptT              aGenFn,
                                                             ProcessCustomFnOptT    aConstructCustomFn,
                                                             ProcessCustomFnOptT    aDestructCustomFn,
                                                             FromStringFnMapT&&     aFromStringFns) noexcept;
                                    GpReflectProp           (const GpReflectProp& aProp);
                                    GpReflectProp           (GpReflectProp&& aProp) noexcept;
                                    ~GpReflectProp          (void) noexcept;

    GpReflectProp&                  operator=               (const GpReflectProp& aProp);
    GpReflectProp&                  operator=               (GpReflectProp&& aProp) noexcept;

    TypeTE                          Type                    (void) const noexcept {return iType;}
    const GpUUID&                   ModelUid                (void) const noexcept {return iModelUid;}
    ContainerTE                     Container               (void) const noexcept {return iContainer;}
    TypeTE                          ContainerKeyType        (void) const noexcept {return iContainerKeyType;}
    std::u8string_view              Name                    (void) const noexcept {return iName;}
    size_t                          Align                   (void) const noexcept {return iAlign;}
    size_t                          Size                    (void) const noexcept {return iSize;}
    std::ptrdiff_t                  Offset                  (void) const noexcept {return iOffset;}
    const GpReflectPropFlags&       Flags                   (void) const noexcept {return iFlags;}
    bool                            FlagTest                (const GpReflectPropFlag::EnumT aFlag) const noexcept {return iFlags.Test(aFlag);}
    std::optional<std::u8string_view>FlagArg                (const GpReflectPropFlag::EnumT aFlag) const noexcept;
    const GpReflectProp&            UnwrapContainerKeyProp  (void) const;
    bool                            GenFn                   (void* aDataPtr) const;
    bool                            FromStringFn            (std::u8string_view aFnName,
                                                             void*              aDataPtr,
                                                             std::u8string_view aStr) const;
    GpReflectProp&                  AddFromStringFn         (std::u8string_view aFnName,
                                                             FromStringFnT      aFn);

    void                            ConstructCustom         (void* aDataPtr) const;
    void                            DestructCustom          (void* aDataPtr) const;

    const u_int_8&                  Value_UInt8             (const void* aDataPtr) const{return CastValueAsConst<u_int_8>(aDataPtr);}
    u_int_8&                        Value_UInt8             (void* aDataPtr) const      {return CastValueAs<u_int_8>(aDataPtr);}
    const s_int_8&                  Value_SInt8             (const void* aDataPtr) const{return CastValueAsConst<s_int_8>(aDataPtr);}
    s_int_8&                        Value_SInt8             (void* aDataPtr) const      {return CastValueAs<s_int_8>(aDataPtr);}
    const u_int_16&                 Value_UInt16            (const void* aDataPtr) const{return CastValueAsConst<u_int_16>(aDataPtr);}
    u_int_16&                       Value_UInt16            (void* aDataPtr) const      {return CastValueAs<u_int_16>(aDataPtr);}
    const s_int_16&                 Value_SInt16            (const void* aDataPtr) const{return CastValueAsConst<s_int_16>(aDataPtr);}
    s_int_16&                       Value_SInt16            (void* aDataPtr) const      {return CastValueAs<s_int_16>(aDataPtr);}
    const u_int_32&                 Value_UInt32            (const void* aDataPtr) const{return CastValueAsConst<u_int_32>(aDataPtr);}
    u_int_32&                       Value_UInt32            (void* aDataPtr) const      {return CastValueAs<u_int_32>(aDataPtr);}
    const s_int_32&                 Value_SInt32            (const void* aDataPtr) const{return CastValueAsConst<s_int_32>(aDataPtr);}
    s_int_32&                       Value_SInt32            (void* aDataPtr) const      {return CastValueAs<s_int_32>(aDataPtr);}
    const u_int_64&                 Value_UInt64            (const void* aDataPtr) const{return CastValueAsConst<u_int_64>(aDataPtr);}
    u_int_64&                       Value_UInt64            (void* aDataPtr) const      {return CastValueAs<u_int_64>(aDataPtr);}
    const s_int_64&                 Value_SInt64            (const void* aDataPtr) const{return CastValueAsConst<s_int_64>(aDataPtr);}
    s_int_64&                       Value_SInt64            (void* aDataPtr) const      {return CastValueAs<s_int_64>(aDataPtr);}

    const unix_ts_s_t&              Value_UnixTSs           (const void* aDataPtr) const{return CastValueAsConst<unix_ts_s_t>(aDataPtr);}
    unix_ts_s_t&                    Value_UnixTSs           (void* aDataPtr) const      {return CastValueAs<unix_ts_s_t>(aDataPtr);}

    const double&                   Value_Double            (const void* aDataPtr) const{return CastValueAsConst<double>(aDataPtr);}
    double&                         Value_Double            (void* aDataPtr) const      {return CastValueAs<double>(aDataPtr);}
    const float&                    Value_Float             (const void* aDataPtr) const{return CastValueAsConst<float>(aDataPtr);}
    float&                          Value_Float             (void* aDataPtr) const      {return CastValueAs<float>(aDataPtr);}
    const bool&                     Value_Bool              (const void* aDataPtr) const{return CastValueAsConst<bool>(aDataPtr);}
    bool&                           Value_Bool              (void* aDataPtr) const      {return CastValueAs<bool>(aDataPtr);}
    const GpUUID&                   Value_UUID              (const void* aDataPtr) const{return CastValueAsConst<GpUUID>(aDataPtr);}
    GpUUID&                         Value_UUID              (void* aDataPtr) const      {return CastValueAs<GpUUID>(aDataPtr);}
    const std::u8string&            Value_String            (const void* aDataPtr) const{return CastValueAsConst<std::u8string>(aDataPtr);}
    std::u8string&                  Value_String            (void* aDataPtr) const      {return CastValueAs<std::u8string>(aDataPtr);}
    const GpBytesArray&             Value_BLOB              (const void* aDataPtr) const{return CastValueAsConst<GpBytesArray>(aDataPtr);}
    GpBytesArray&                   Value_BLOB              (void* aDataPtr) const      {return CastValueAs<GpBytesArray>(aDataPtr);}
    const GpReflectObject&          Value_Object            (const void* aDataPtr) const{return CastValueAsConst<GpReflectObject>(aDataPtr);}
    GpReflectObject&                Value_Object            (void* aDataPtr) const      {return CastValueAs<GpReflectObject>(aDataPtr);}
    const GpSP<GpReflectObject>&    Value_ObjectSP          (const void* aDataPtr) const{return CastValueAsConst<GpSP<GpReflectObject>>(aDataPtr);}
    GpSP<GpReflectObject>&          Value_ObjectSP          (void* aDataPtr) const      {return CastValueAs<GpSP<GpReflectObject>>(aDataPtr);}
    const GpEnum&                   Value_Enum              (const void* aDataPtr) const{return CastValueAsConst<GpEnum>(aDataPtr);}
    GpEnum&                         Value_Enum              (void* aDataPtr) const      {return CastValueAs<GpEnum>(aDataPtr);}
    const GpEnumFlags&              Value_EnumFlags         (const void* aDataPtr) const{return CastValueAsConst<GpEnumFlags>(aDataPtr);}
    GpEnumFlags&                    Value_EnumFlags         (void* aDataPtr) const      {return CastValueAs<GpEnumFlags>(aDataPtr);}

    const auto&                     Vec_UInt8               (const void* aDataPtr) const{return CastValueAsConst<std::vector<u_int_8>>(aDataPtr);}
    auto&                           Vec_UInt8               (void* aDataPtr) const      {return CastValueAs<std::vector<u_int_8>>(aDataPtr);}
    const auto&                     Vec_SInt8               (const void* aDataPtr) const{return CastValueAsConst<std::vector<s_int_8>>(aDataPtr);}
    auto&                           Vec_SInt8               (void* aDataPtr) const      {return CastValueAs<std::vector<s_int_8>>(aDataPtr);}
    const auto&                     Vec_UInt16              (const void* aDataPtr) const{return CastValueAsConst<std::vector<u_int_16>>(aDataPtr);}
    auto&                           Vec_UInt16              (void* aDataPtr) const      {return CastValueAs<std::vector<u_int_16>>(aDataPtr);}
    const auto&                     Vec_SInt16              (const void* aDataPtr) const{return CastValueAsConst<std::vector<s_int_16>>(aDataPtr);}
    auto&                           Vec_SInt16              (void* aDataPtr) const      {return CastValueAs<std::vector<s_int_16>>(aDataPtr);}
    const auto&                     Vec_UInt32              (const void* aDataPtr) const{return CastValueAsConst<std::vector<u_int_32>>(aDataPtr);}
    auto&                           Vec_UInt32              (void* aDataPtr) const      {return CastValueAs<std::vector<u_int_32>>(aDataPtr);}
    const auto&                     Vec_SInt32              (const void* aDataPtr) const{return CastValueAsConst<std::vector<s_int_32>>(aDataPtr);}
    auto&                           Vec_SInt32              (void* aDataPtr) const      {return CastValueAs<std::vector<s_int_32>>(aDataPtr);}
    const auto&                     Vec_UInt64              (const void* aDataPtr) const{return CastValueAsConst<std::vector<u_int_64>>(aDataPtr);}
    auto&                           Vec_UInt64              (void* aDataPtr) const      {return CastValueAs<std::vector<u_int_64>>(aDataPtr);}
    const auto&                     Vec_SInt64              (const void* aDataPtr) const{return CastValueAsConst<std::vector<s_int_64>>(aDataPtr);}
    auto&                           Vec_SInt64              (void* aDataPtr) const      {return CastValueAs<std::vector<s_int_64>>(aDataPtr);}

    const auto&                     Vec_UnixTSs             (const void* aDataPtr) const{return CastValueAsConst<std::vector<unix_ts_s_t>>(aDataPtr);}
    auto&                           Vec_UnixTSs             (void* aDataPtr) const      {return CastValueAs<std::vector<unix_ts_s_t>>(aDataPtr);}

    const auto&                     Vec_Double              (const void* aDataPtr) const{return CastValueAsConst<std::vector<double>>(aDataPtr);}
    auto&                           Vec_Double              (void* aDataPtr) const      {return CastValueAs<std::vector<double>>(aDataPtr);}
    const auto&                     Vec_Float               (const void* aDataPtr) const{return CastValueAsConst<std::vector<float>>(aDataPtr);}
    auto&                           Vec_Float               (void* aDataPtr) const      {return CastValueAs<std::vector<float>>(aDataPtr);}
    const auto&                     Vec_Bool                (const void* aDataPtr) const{return CastValueAsConst<std::vector<bool>>(aDataPtr);}
    auto&                           Vec_Bool                (void* aDataPtr) const      {return CastValueAs<std::vector<bool>>(aDataPtr);}
    const auto&                     Vec_UUID                (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpUUID>>(aDataPtr);}
    auto&                           Vec_UUID                (void* aDataPtr) const      {return CastValueAs<std::vector<GpUUID>>(aDataPtr);}
    const auto&                     Vec_String              (const void* aDataPtr) const{return CastValueAsConst<std::vector<std::u8string>>(aDataPtr);}
    auto&                           Vec_String              (void* aDataPtr) const      {return CastValueAs<std::vector<std::u8string>>(aDataPtr);}
    const auto&                     Vec_BLOB                (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpBytesArray>>(aDataPtr);}
    auto&                           Vec_BLOB                (void* aDataPtr) const      {return CastValueAs<std::vector<GpBytesArray>>(aDataPtr);}
    //const auto&                   Vec_Object              (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpReflectObject>>(aDataPtr);}
    //auto&                         Vec_Object              (void* aDataPtr) const      {return CastValueAs<std::vector<GpReflectObject>>(aDataPtr);}
    const auto&                     Vec_ObjectSP            (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpSP<GpReflectObject>>>(aDataPtr);}
    auto&                           Vec_ObjectSP            (void* aDataPtr) const      {return CastValueAs<std::vector<GpSP<GpReflectObject>>>(aDataPtr);}
    //const auto&                   Vec_Enum                (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpEnum>>(aDataPtr);}
    //auto&                         Vec_Enum                (void* aDataPtr) const      {return CastValueAs<std::vector<GpEnum>>(aDataPtr);}
    //const auto&                   Vec_EnumFlags           (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpEnumFlags>>(aDataPtr);}
    //auto&                         Vec_EnumFlags           (void* aDataPtr) const      {return CastValueAs<std::vector<GpEnumFlags>>(aDataPtr);}

    const GpVectorReflectObjWrapBase&   VecWrap_Object      (const void* aDataPtr) const{return CastValueAsConst<GpVectorReflectObjWrapBase>(aDataPtr);}
    GpVectorReflectObjWrapBase&         VecWrap_Object      (void* aDataPtr) const      {return CastValueAs<GpVectorReflectObjWrapBase>(aDataPtr);}

    template<typename Key> const auto&  Map_UInt8           (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, u_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UInt8           (void* aDataPtr) const      {return CastValueAs<std::map<Key, u_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SInt8           (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, s_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SInt8           (void* aDataPtr) const      {return CastValueAs<std::map<Key, s_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UInt16          (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, u_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UInt16          (void* aDataPtr) const      {return CastValueAs<std::map<Key, u_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SInt16          (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, s_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SInt16          (void* aDataPtr) const      {return CastValueAs<std::map<Key, s_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UInt32          (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, u_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UInt32          (void* aDataPtr) const      {return CastValueAs<std::map<Key, u_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SInt32          (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, s_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SInt32          (void* aDataPtr) const      {return CastValueAs<std::map<Key, s_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UInt64          (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, u_int_64, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UInt64          (void* aDataPtr) const      {return CastValueAs<std::map<Key, u_int_64, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SInt64          (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, s_int_64, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SInt64          (void* aDataPtr) const      {return CastValueAs<std::map<Key, s_int_64, std::less<>>>(aDataPtr);}

    template<typename Key> const auto&  Map_UnixTSs         (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, unix_ts_s_t, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UnixTSs         (void* aDataPtr) const      {return CastValueAs<std::map<Key, unix_ts_s_t, std::less<>>>(aDataPtr);}

    template<typename Key> const auto&  Map_Double          (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, double, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_Double          (void* aDataPtr) const      {return CastValueAs<std::map<Key, double, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_Float           (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, float, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_Float           (void* aDataPtr) const      {return CastValueAs<std::map<Key, float, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_Bool            (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, bool, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_Bool            (void* aDataPtr) const      {return CastValueAs<std::map<Key, bool, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UUID            (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpUUID, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UUID            (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpUUID, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_String          (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, std::u8string, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_String          (void* aDataPtr) const      {return CastValueAs<std::map<Key, std::u8string, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_BLOB            (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpBytesArray, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_BLOB            (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpBytesArray, std::less<>>>(aDataPtr);}
    //template<typename Key> const auto&Map_Object          (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpReflectObject, std::less<>>>(aDataPtr);}
    //template<typename Key> auto&      Map_Object          (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpReflectObject, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_ObjectSP        (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpSP<GpReflectObject>, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_ObjectSP        (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpSP<GpReflectObject>, std::less<>>>(aDataPtr);}
    //template<typename Key> const auto&Map_Enum            (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpEnum>, std::less<>>(aDataPtr);}
    //template<typename Key> auto&      Map_Enum            (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpEnum>, std::less<>>(aDataPtr);}
    //template<typename Key> const auto&Map_EnumFlags       (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpEnumFlags>, std::less<>>(aDataPtr);}
    //template<typename Key> auto&      Map_EnumFlags       (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpEnumFlags>, std::less<>>(aDataPtr);}

private:
    template<typename T>
    const T&                            CastValueAsConst    (const void* aDataPtr) const;

    template<typename T>
    T&                                  CastValueAs         (void* aDataPtr) const;

    inline const void*                  PropPtrConst        (const void* aDataPtr) const;
    inline void*                        PropPtr             (void* aDataPtr) const;

private:
    TypeTE              iType;
    GpUUID              iModelUid;
    ContainerTE         iContainer;
    TypeTE              iContainerKeyType;
    std::u8string       iName;
    size_t              iAlign      = 0;
    size_t              iSize       = 0;
    std::ptrdiff_t      iOffset     = 0;
    GpReflectPropFlags  iFlags;
    FlagArgsT           iFlagArgs;

    GenFnOptT           iGenFn;
    ProcessCustomFnOptT iConstructCustomFn;
    ProcessCustomFnOptT iDestructCustomFn;

    FromStringFnMapT    iFromStringFns;
};

template<typename T
         >
const T&    GpReflectProp::CastValueAsConst (const void* aDataPtr) const
{
    return *reinterpret_cast<const T*>(PropPtrConst(aDataPtr));
}

template<typename T>
T&  GpReflectProp::CastValueAs (void* aDataPtr) const
{
    return *reinterpret_cast<T*>(PropPtr(aDataPtr));
}

const void* GpReflectProp::PropPtrConst (const void* aDataPtr) const
{
    return reinterpret_cast<const char*>(aDataPtr) + iOffset;
}

void*   GpReflectProp::PropPtr (void* aDataPtr) const
{
    return reinterpret_cast<char*>(aDataPtr) + iOffset;
}

}//GPlatform

#endif//GP_USE_REFLECTION
