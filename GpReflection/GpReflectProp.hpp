#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/Config/IncludeExt/boost_flat_map.hpp>

#include "GpReflectType.hpp"
#include "GpReflectContainerType.hpp"
#include "GpReflectPropFlags.hpp"
#include "GpReflectObjWrapVector.hpp"
#include "GpReflectUtils.hpp"

namespace GPlatform {

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
    using FlagArgsT             = boost::container::flat_map<GpReflectPropFlags::EnumT, std::string>;

    using FromStringFnT         = std::function<void(const GpReflectProp& aProp, void* aDataPtr, std::string_view aSrc)>;
    using FromStringFnMapT      = boost::container::flat_map<std::string, FromStringFnT, std::less<>>;

    using GenFnT                = std::function<void(const GpReflectProp& aProp, void* aDataPtr)>;
    using GenFnOptT             = std::optional<GenFnT>;
    using ProcessCustomFnT      = std::function<void(void* aPropPtr)>;
    using ProcessCustomFnOptT   = std::optional<ProcessCustomFnT>;

    using SmallVecVal           = C::SmallVec<24>::Val;
    using SmallVecCRef          = C::SmallVec<24>::CRef;

public:
                                    GpReflectProp       (void) noexcept;
                                    GpReflectProp       (TypeTE                 aType,
                                                         GpUUID                 aModelUid,
                                                         ContainerTE            aContainer,
                                                         TypeTE                 aContainerKeyType,
                                                         std::string&&          aName,
                                                         size_t                 aAlign,
                                                         size_t                 aSize,
                                                         std::ptrdiff_t         aOffset,
                                                         FlagsT                 aFlags,
                                                         FlagArgsT&&            aFlagArgs,
                                                         GenFnOptT              aGenFn,
                                                         ProcessCustomFnOptT    aConstructCustomFn,
                                                         ProcessCustomFnOptT    aDestructCustomFn,
                                                         FromStringFnMapT&&     aFromStringFns,
                                                         size_t                 aIdxInProps) noexcept;
                                    GpReflectProp       (const GpReflectProp& aProp);
                                    GpReflectProp       (GpReflectProp&& aProp) noexcept;
                                    ~GpReflectProp      (void) noexcept;

    GpReflectProp&                  operator=           (const GpReflectProp& aProp);
    GpReflectProp&                  operator=           (GpReflectProp&& aProp) noexcept;

    bool                            IsEqual             (const GpReflectProp& aProp) const noexcept;

    TypeTE                          Type                (void) const noexcept {return iType;}
    const GpUUID&                   ModelUid            (void) const noexcept {return iModelUid;}
    ContainerTE                     Container           (void) const noexcept {return iContainer;}
    TypeTE                          ContainerKeyType    (void) const noexcept {return iContainerKeyType;}
    std::string_view                Name                (void) const noexcept {return iName;}
    size_t                          Align               (void) const noexcept {return iAlign;}
    size_t                          Size                (void) const noexcept {return iSize;}
    std::ptrdiff_t                  Offset              (void) const noexcept {return iOffset;}
    const GpReflectPropFlags&       Flags               (void) const noexcept {return iFlags;}
    bool                            FlagTest            (const GpReflectPropFlag::EnumT aFlag) const noexcept {return iFlags.Test(aFlag);}
    std::optional<std::string_view> FlagArg             (const GpReflectPropFlag::EnumT aFlag) const noexcept;
    bool                            GenFn               (void* aDataPtr) const;
    bool                            FromStringFn        (std::string_view   aFnName,
                                                         void*              aDataPtr,
                                                         std::string_view   aStr) const;
    GpReflectProp&                  AddFromStringFn     (std::string_view   aFnName,
                                                         FromStringFnT      aFn);
    size_t                          IdxInProps          (void) const noexcept {return iIdxInProps;}

    void                            ConstructCustom     (void* aDataPtr) const;
    void                            DestructCustom      (void* aDataPtr) const;

    template<typename T>
    const T&                        Value               (const void* aDataPtr) const;

    template<typename T>
    T&                              Value               (void* aDataPtr) const;

    const u_int_8&                  Value_UI8           (const void* aDataPtr) const{return CastValueAsConst<u_int_8>(aDataPtr);}
    u_int_8&                        Value_UI8           (void* aDataPtr) const      {return CastValueAs<u_int_8>(aDataPtr);}
    const s_int_8&                  Value_SI8           (const void* aDataPtr) const{return CastValueAsConst<s_int_8>(aDataPtr);}
    s_int_8&                        Value_SI8           (void* aDataPtr) const      {return CastValueAs<s_int_8>(aDataPtr);}
    const u_int_16&                 Value_UI16          (const void* aDataPtr) const{return CastValueAsConst<u_int_16>(aDataPtr);}
    u_int_16&                       Value_UI16          (void* aDataPtr) const      {return CastValueAs<u_int_16>(aDataPtr);}
    const s_int_16&                 Value_SI16          (const void* aDataPtr) const{return CastValueAsConst<s_int_16>(aDataPtr);}
    s_int_16&                       Value_SI16          (void* aDataPtr) const      {return CastValueAs<s_int_16>(aDataPtr);}
    const u_int_32&                 Value_UI32          (const void* aDataPtr) const{return CastValueAsConst<u_int_32>(aDataPtr);}
    u_int_32&                       Value_UI32          (void* aDataPtr) const      {return CastValueAs<u_int_32>(aDataPtr);}
    const s_int_32&                 Value_SI32          (const void* aDataPtr) const{return CastValueAsConst<s_int_32>(aDataPtr);}
    s_int_32&                       Value_SI32          (void* aDataPtr) const      {return CastValueAs<s_int_32>(aDataPtr);}
    const u_int_64&                 Value_UI64          (const void* aDataPtr) const{return CastValueAsConst<u_int_64>(aDataPtr);}
    u_int_64&                       Value_UI64          (void* aDataPtr) const      {return CastValueAs<u_int_64>(aDataPtr);}
    const s_int_64&                 Value_SI64          (const void* aDataPtr) const{return CastValueAsConst<s_int_64>(aDataPtr);}
    s_int_64&                       Value_SI64          (void* aDataPtr) const      {return CastValueAs<s_int_64>(aDataPtr);}

    const double&                   Value_Double        (const void* aDataPtr) const{return CastValueAsConst<double>(aDataPtr);}
    double&                         Value_Double        (void* aDataPtr) const      {return CastValueAs<double>(aDataPtr);}
    const float&                    Value_Float         (const void* aDataPtr) const{return CastValueAsConst<float>(aDataPtr);}
    float&                          Value_Float         (void* aDataPtr) const      {return CastValueAs<float>(aDataPtr);}
    const bool&                     Value_Bool          (const void* aDataPtr) const{return CastValueAsConst<bool>(aDataPtr);}
    bool&                           Value_Bool          (void* aDataPtr) const      {return CastValueAs<bool>(aDataPtr);}
    const GpUUID&                   Value_UUID          (const void* aDataPtr) const{return CastValueAsConst<GpUUID>(aDataPtr);}
    GpUUID&                         Value_UUID          (void* aDataPtr) const      {return CastValueAs<GpUUID>(aDataPtr);}
    const std::string&              Value_String        (const void* aDataPtr) const{return CastValueAsConst<std::string>(aDataPtr);}
    std::string&                    Value_String        (void* aDataPtr) const      {return CastValueAs<std::string>(aDataPtr);}
    const GpBytesArray&             Value_BLOB          (const void* aDataPtr) const{return CastValueAsConst<GpBytesArray>(aDataPtr);}
    GpBytesArray&                   Value_BLOB          (void* aDataPtr) const      {return CastValueAs<GpBytesArray>(aDataPtr);}
    const GpReflectObject&          Value_Object        (const void* aDataPtr) const{return CastValueAsConst<GpReflectObject>(aDataPtr);}
    GpReflectObject&                Value_Object        (void* aDataPtr) const      {return CastValueAs<GpReflectObject>(aDataPtr);}
    const GpSP<GpReflectObject>&    Value_ObjectSP      (const void* aDataPtr) const{return CastValueAsConst<GpSP<GpReflectObject>>(aDataPtr);}
    GpSP<GpReflectObject>&          Value_ObjectSP      (void* aDataPtr) const      {return CastValueAs<GpSP<GpReflectObject>>(aDataPtr);}
    const GpEnum&                   Value_Enum          (const void* aDataPtr) const{return CastValueAsConst<GpEnum>(aDataPtr);}
    GpEnum&                         Value_Enum          (void* aDataPtr) const      {return CastValueAs<GpEnum>(aDataPtr);}
    const GpEnumFlags&              Value_EnumFlags     (const void* aDataPtr) const{return CastValueAsConst<GpEnumFlags>(aDataPtr);}
    GpEnumFlags&                    Value_EnumFlags     (void* aDataPtr) const      {return CastValueAs<GpEnumFlags>(aDataPtr);}

    const auto&                     Vec_UI8             (const void* aDataPtr) const{return CastValueAsConst<std::vector<u_int_8>>(aDataPtr);}
    auto&                           Vec_UI8             (void* aDataPtr) const      {return CastValueAs<std::vector<u_int_8>>(aDataPtr);}
    const auto&                     Vec_SI8             (const void* aDataPtr) const{return CastValueAsConst<std::vector<s_int_8>>(aDataPtr);}
    auto&                           Vec_SI8             (void* aDataPtr) const      {return CastValueAs<std::vector<s_int_8>>(aDataPtr);}
    const auto&                     Vec_UI16            (const void* aDataPtr) const{return CastValueAsConst<std::vector<u_int_16>>(aDataPtr);}
    auto&                           Vec_UI16            (void* aDataPtr) const      {return CastValueAs<std::vector<u_int_16>>(aDataPtr);}
    const auto&                     Vec_SI16            (const void* aDataPtr) const{return CastValueAsConst<std::vector<s_int_16>>(aDataPtr);}
    auto&                           Vec_SI16            (void* aDataPtr) const      {return CastValueAs<std::vector<s_int_16>>(aDataPtr);}
    const auto&                     Vec_UI32            (const void* aDataPtr) const{return CastValueAsConst<std::vector<u_int_32>>(aDataPtr);}
    auto&                           Vec_UI32            (void* aDataPtr) const      {return CastValueAs<std::vector<u_int_32>>(aDataPtr);}
    const auto&                     Vec_SI32            (const void* aDataPtr) const{return CastValueAsConst<std::vector<s_int_32>>(aDataPtr);}
    auto&                           Vec_SI32            (void* aDataPtr) const      {return CastValueAs<std::vector<s_int_32>>(aDataPtr);}
    const auto&                     Vec_UI64            (const void* aDataPtr) const{return CastValueAsConst<std::vector<u_int_64>>(aDataPtr);}
    auto&                           Vec_UI64            (void* aDataPtr) const      {return CastValueAs<std::vector<u_int_64>>(aDataPtr);}
    const auto&                     Vec_SI64            (const void* aDataPtr) const{return CastValueAsConst<std::vector<s_int_64>>(aDataPtr);}
    auto&                           Vec_SI64            (void* aDataPtr) const      {return CastValueAs<std::vector<s_int_64>>(aDataPtr);}

    const auto&                     Vec_Double          (const void* aDataPtr) const{return CastValueAsConst<std::vector<double>>(aDataPtr);}
    auto&                           Vec_Double          (void* aDataPtr) const      {return CastValueAs<std::vector<double>>(aDataPtr);}
    const auto&                     Vec_Float           (const void* aDataPtr) const{return CastValueAsConst<std::vector<float>>(aDataPtr);}
    auto&                           Vec_Float           (void* aDataPtr) const      {return CastValueAs<std::vector<float>>(aDataPtr);}
    //const auto&                   Vec_Bool            (const void* aDataPtr) const{return CastValueAsConst<std::vector<bool>>(aDataPtr);}
    //auto&                         Vec_Bool            (void* aDataPtr) const      {return CastValueAs<std::vector<bool>>(aDataPtr);}
    const auto&                     Vec_UUID            (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpUUID>>(aDataPtr);}
    auto&                           Vec_UUID            (void* aDataPtr) const      {return CastValueAs<std::vector<GpUUID>>(aDataPtr);}
    const auto&                     Vec_String          (const void* aDataPtr) const{return CastValueAsConst<std::vector<std::string>>(aDataPtr);}
    auto&                           Vec_String          (void* aDataPtr) const      {return CastValueAs<std::vector<std::string>>(aDataPtr);}
    const auto&                     Vec_BLOB            (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpBytesArray>>(aDataPtr);}
    auto&                           Vec_BLOB            (void* aDataPtr) const      {return CastValueAs<std::vector<GpBytesArray>>(aDataPtr);}
    //const auto&                   Vec_Object          (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpReflectObject>>(aDataPtr);}
    //auto&                         Vec_Object          (void* aDataPtr) const      {return CastValueAs<std::vector<GpReflectObject>>(aDataPtr);}
    const GpVectorReflectObjWrapBase&   VecWrap_Object  (const void* aDataPtr) const{return CastValueAsConst<GpVectorReflectObjWrapBase>(aDataPtr);}
    GpVectorReflectObjWrapBase&         VecWrap_Object  (void* aDataPtr) const      {return CastValueAs<GpVectorReflectObjWrapBase>(aDataPtr);}
    const auto&                     Vec_ObjectSP        (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpSP<GpReflectObject>>>(aDataPtr);}
    auto&                           Vec_ObjectSP        (void* aDataPtr) const      {return CastValueAs<std::vector<GpSP<GpReflectObject>>>(aDataPtr);}
    //const auto&                   Vec_Enum            (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpEnum>>(aDataPtr);}
    //auto&                         Vec_Enum            (void* aDataPtr) const      {return CastValueAs<std::vector<GpEnum>>(aDataPtr);}
    //const auto&                   Vec_EnumFlags       (const void* aDataPtr) const{return CastValueAsConst<std::vector<GpEnumFlags>>(aDataPtr);}
    //auto&                         Vec_EnumFlags       (void* aDataPtr) const      {return CastValueAs<std::vector<GpEnumFlags>>(aDataPtr);}

    template<typename Key> const auto&  Map_UI8         (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, u_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UI8         (void* aDataPtr) const      {return CastValueAs<std::map<Key, u_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SI8         (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, s_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SI8         (void* aDataPtr) const      {return CastValueAs<std::map<Key, s_int_8, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UI16        (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, u_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UI16        (void* aDataPtr) const      {return CastValueAs<std::map<Key, u_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SI16        (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, s_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SI16        (void* aDataPtr) const      {return CastValueAs<std::map<Key, s_int_16, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UI32        (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, u_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UI32        (void* aDataPtr) const      {return CastValueAs<std::map<Key, u_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SI32        (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, s_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SI32        (void* aDataPtr) const      {return CastValueAs<std::map<Key, s_int_32, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UI64        (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, u_int_64, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UI64        (void* aDataPtr) const      {return CastValueAs<std::map<Key, u_int_64, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_SI64        (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, s_int_64, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_SI64        (void* aDataPtr) const      {return CastValueAs<std::map<Key, s_int_64, std::less<>>>(aDataPtr);}

    template<typename Key> const auto&  Map_Double      (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, double, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_Double      (void* aDataPtr) const      {return CastValueAs<std::map<Key, double, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_Float       (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, float, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_Float       (void* aDataPtr) const      {return CastValueAs<std::map<Key, float, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_Bool        (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, bool, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_Bool        (void* aDataPtr) const      {return CastValueAs<std::map<Key, bool, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_UUID        (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpUUID, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_UUID        (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpUUID, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_String      (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, std::string, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_String      (void* aDataPtr) const      {return CastValueAs<std::map<Key, std::string, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_BLOB        (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpBytesArray, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_BLOB        (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpBytesArray, std::less<>>>(aDataPtr);}
    //template<typename Key> const auto&Map_Object      (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpReflectObject, std::less<>>>(aDataPtr);}
    //template<typename Key> auto&      Map_Object      (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpReflectObject, std::less<>>>(aDataPtr);}
    template<typename Key> const auto&  Map_ObjectSP    (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpSP<GpReflectObject>, std::less<>>>(aDataPtr);}
    template<typename Key> auto&        Map_ObjectSP    (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpSP<GpReflectObject>, std::less<>>>(aDataPtr);}
    //template<typename Key> const auto&Map_Enum        (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpEnum>, std::less<>>(aDataPtr);}
    //template<typename Key> auto&      Map_Enum        (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpEnum>, std::less<>>(aDataPtr);}
    //template<typename Key> const auto&Map_EnumFlags   (const void* aDataPtr) const{return CastValueAsConst<std::map<Key, GpEnumFlags>, std::less<>>(aDataPtr);}
    //template<typename Key> auto&      Map_EnumFlags   (void* aDataPtr) const      {return CastValueAs<std::map<Key, GpEnumFlags>, std::less<>>(aDataPtr);}

private:
    template<typename T>
    const T&                            CastValueAsConst(const void* aDataPtr) const;

    template<typename T>
    T&                                  CastValueAs     (void* aDataPtr) const;

    inline const void*                  PropPtrConst    (const void* aDataPtr) const;
    inline void*                        PropPtr         (void* aDataPtr) const;

private:
    TypeTE              iType               = GpReflectType::NOT_SET;
    GpUUID              iModelUid;
    ContainerTE         iContainer          = GpReflectContainerType::NO;
    TypeTE              iContainerKeyType   = GpReflectType::NOT_SET;
    std::string         iName;
    size_t              iAlign              = 0;
    size_t              iSize               = 0;
    std::ptrdiff_t      iOffset             = 0;
    GpReflectPropFlags  iFlags;
    FlagArgsT           iFlagArgs;

    GenFnOptT           iGenFn;
    ProcessCustomFnOptT iConstructCustomFn;
    ProcessCustomFnOptT iDestructCustomFn;

    FromStringFnMapT    iFromStringFns;

    size_t              iIdxInProps = 0;
};

template<typename T>
const T&    GpReflectProp::Value (const void* aDataPtr) const
{
    constexpr const auto types          = GpReflectUtils::SDetectTypes<T>();
    constexpr const auto valueType      = std::get<0>(types);
    constexpr const auto keyType        = std::get<1>(types);
    constexpr const auto containerType  = std::get<2>(types);

    if constexpr(containerType == GpReflectContainerType::NO)
    {
             if constexpr (valueType == GpReflectType::U_INT_8)     return Value_UI8(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_8)     return Value_SI8(aDataPtr);
        else if constexpr (valueType == GpReflectType::U_INT_16)    return Value_UI16(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_16)    return Value_SI16(aDataPtr);
        else if constexpr (valueType == GpReflectType::U_INT_32)    return Value_UI32(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_32)    return Value_SI32(aDataPtr);
        else if constexpr (valueType == GpReflectType::U_INT_64)    return Value_UI64(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_64)    return Value_SI64(aDataPtr);
        else if constexpr (valueType == GpReflectType::DOUBLE)      return Value_Double(aDataPtr);
        else if constexpr (valueType == GpReflectType::FLOAT)       return Value_Float(aDataPtr);
        else if constexpr (valueType == GpReflectType::BOOLEAN)     return Value_Bool(aDataPtr);
        else if constexpr (valueType == GpReflectType::UUID)        return Value_UUID(aDataPtr);
        else if constexpr (valueType == GpReflectType::STRING)      return Value_String(aDataPtr);
        else if constexpr (valueType == GpReflectType::BLOB)        return Value_BLOB(aDataPtr);
        else if constexpr (valueType == GpReflectType::OBJECT)      return Value_Object(aDataPtr);
        else if constexpr (valueType == GpReflectType::OBJECT_SP)   return Value_ObjectSP(aDataPtr);
        else if constexpr (valueType == GpReflectType::ENUM)        return Value_Enum<T>(aDataPtr);
        else if constexpr (valueType == GpReflectType::ENUM_FLAGS)  return Value_EnumFlags<T>(aDataPtr);
        else
        {
            // if constexpr (valueType == GpReflectType::NOT_SET) ...

            GpThrowCe<GpException>("Unsupported type '"_sv + GpReflectUtils::SModelName<T>());
        }
    } else if constexpr(containerType == GpReflectContainerType::VECTOR)
    {
             if constexpr (valueType == GpReflectType::U_INT_8)     return Vec_UI8(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_8)     return Vec_SI8(aDataPtr);
        else if constexpr (valueType == GpReflectType::U_INT_16)    return Vec_UI16(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_16)    return Vec_SI16(aDataPtr);
        else if constexpr (valueType == GpReflectType::U_INT_32)    return Vec_UI32(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_32)    return Vec_SI32(aDataPtr);
        else if constexpr (valueType == GpReflectType::U_INT_64)    return Vec_UI64(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_64)    return Vec_SI64(aDataPtr);
        else if constexpr (valueType == GpReflectType::DOUBLE)      return Vec_Double(aDataPtr);
        else if constexpr (valueType == GpReflectType::FLOAT)       return Vec_Float(aDataPtr);
        else if constexpr (valueType == GpReflectType::UUID)        return Vec_UUID(aDataPtr);
        else if constexpr (valueType == GpReflectType::STRING)      return Vec_String(aDataPtr);
        else if constexpr (valueType == GpReflectType::BLOB)        return Vec_BLOB(aDataPtr);
        else if constexpr (valueType == GpReflectType::OBJECT_SP)   return Vec_ObjectSP(aDataPtr);
        else
        {
            // if constexpr (valueType == GpReflectType::OBJECT) ...
            // if constexpr (valueType == GpReflectType::ENUM_FLAGS) ...
            // if constexpr (valueType == GpReflectType::ENUM) ...
            // if constexpr (valueType == GpReflectType::BOOLEAN) ...
            // if constexpr (valueType == GpReflectType::NOT_SET) ...

            GpThrowCe<GpException>("Unsupported type '"_sv + GpReflectUtils::SModelName<T>());
        }
    } else if constexpr(containerType == GpReflectContainerType::VECTOR_WRAP)
    {
        return VecWrap_Object(aDataPtr);
    } else if constexpr(containerType == GpReflectContainerType::MAP)
    {
        using KeyT = typename T::key_type;

        static_assert(std::is_same_v<typename GpReflectUtils::DecltypeT<keyType>, KeyT>);

             if constexpr (valueType == GpReflectType::U_INT_8)     return Map_UI8<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_8)     return Map_SI8<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::U_INT_16)    return Map_UI16<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_16)    return Map_SI16<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::U_INT_32)    return Map_UI32<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_32)    return Map_SI32<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::U_INT_64)    return Map_UI64<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::S_INT_64)    return Map_SI64<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::DOUBLE)      return Map_Double<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::FLOAT)       return Map_Float<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::BOOLEAN)     return Map_Bool<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::UUID)        return Map_Uuid<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::STRING)      return Map_String<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::BLOB)        return Map_Blob<KeyT>(aDataPtr);
        else if constexpr (valueType == GpReflectType::OBJECT_SP)   return Map_ObjectSP<KeyT>(aDataPtr);
        {
            // if constexpr (valueType == GpReflectType::OBJECT) ...
            // if constexpr (valueType == GpReflectType::ENUM_FLAGS) ...
            // if constexpr (valueType == GpReflectType::ENUM) ...
            // if constexpr (valueType == GpReflectType::NOT_SET) ...

            GpThrowCe<GpException>("Unsupported type '"_sv + GpReflectUtils::SModelName<T>());
        }
    }

    GpThrowCe<GpException>("Unsupported type '"_sv + GpReflectUtils::SModelName<T>());
}

template<typename T>
T&  GpReflectProp::Value (void* aDataPtr) const
{
    return const_cast<T&>(Value<T>(static_cast<const void*>(aDataPtr)));
}

template<typename T>
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

}// namespace GPlatform
