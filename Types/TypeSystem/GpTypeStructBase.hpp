#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructInfo.hpp"

namespace GPlatform {

class GPCORE_API GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypeStructBase)

    struct  _type_id_tag_t{};
    template<typename T> static T& SGetBaseObjectType(void(T::*)(typename T::_type_id_tag_t&) const);

    using   BaseT = std::false_type;

protected:
                                    GpTypeStructBase    (void) noexcept {}

public:
    virtual                         ~GpTypeStructBase   (void) noexcept {}

    const GpTypeStructInfo&         TypeInfo            (void) const noexcept {return _TypeInfo();}
    GpTypeStructBase::SP            NewInstance         (void) const {return _NewInstance();}

protected:
    virtual void                    _type_id_tag_fn     (_type_id_tag_t&) const noexcept {}
    virtual const GpTypeStructInfo& _TypeInfo           (void) const noexcept = 0;
    virtual GpTypeStructBase::SP    _NewInstance        (void) const = 0;
    static const GpTypeStructInfo&  STypeInfo           (void);
    static const GpUUID&            STypeUID            (void) noexcept;
};

//------------------------- TYPE_STRUCT_DECLARE -------------------------
#if defined(GP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER)
#   define TYPE_STRUCT_DECLARE_STATIC_TO_TYPE_MANAGER static const size_t _sTYPE_MANAGER
#   define TYPE_STRUCT_IMPLEMENT_STATIC_TO_TYPE_MANAGER(T) const size_t T::_sTYPE_MANAGER = GpTypeManager::S().Register(T::STypeInfo());
#else
#   define TYPE_STRUCT_DECLARE_STATIC_TO_TYPE_MANAGER
#   define TYPE_STRUCT_IMPLEMENT_STATIC_TO_TYPE_MANAGER(T)
#endif//#if defined(GP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER)

#define TYPE_STRUCT_DECLARE(TUUID) \
    using BaseT     = std::remove_reference<decltype(SGetBaseObjectType(&this_type::_type_id_tag_fn))>::type; \
\
    static const GpTypeStructInfo&  STypeInfo   (void); \
    constexpr static const GpUUID   STypeUID    (void) noexcept \
    { \
        constexpr const GpUUID structUID = GpUUID::CE_FromString(TUUID); \
        return structUID; \
    } \
\
    class StructFactory final: public GpTypeStructFactory \
    { \
    public: \
        virtual GpSP<GpTypeStructBase>  NewInstance (void) const override final; \
    }; \
\
protected: \
    virtual void                    _type_id_tag_fn (_type_id_tag_t&) const noexcept override{} \
    virtual const GpTypeStructInfo& _TypeInfo       (void) const noexcept override; \
    virtual GpTypeStructBase::SP    _NewInstance    (void) const override; \
\
private: \
    static GpTypeStructInfo         _SCollectStructInfo (const GpTypeStructInfo* aBaseStructInfo); \
    static void                     _SCollectStructProps(GpTypePropInfo::C::Vec::Val& aPropsOut); \
    TYPE_STRUCT_DECLARE_STATIC_TO_TYPE_MANAGER;
\

//------------------------- TYPE_STRUCT_IMPLEMENT -------------------------
#define MACRO_D_TO_STR(VAL) #VAL

#define TYPE_STRUCT_IMPLEMENT(T, MODULE_UUID) \
\
    TYPE_STRUCT_IMPLEMENT_STATIC_TO_TYPE_MANAGER(T); \
\
    GpSP<GpTypeStructBase>  T::StructFactory::NewInstance (void) const {return MakeSP<T>();} \
\
    const GpTypeStructInfo& T::STypeInfo (void) \
    { \
        static const GpTypeStructInfo sStructInfo = T::_SCollectStructInfo(&T::BaseT::STypeInfo()); \
        return sStructInfo; \
    } \
\
    const GpTypeStructInfo& T::_TypeInfo (void) const noexcept \
    { \
        return T::STypeInfo(); \
    } \
\
    GpTypeStructBase::SP    T::_NewInstance (void) const \
    { \
        return MakeSP<T>(); \
    } \
\
    GpTypeStructInfo    T::_SCollectStructInfo (const GpTypeStructInfo* aBaseStructInfo) \
    { \
        GpTypePropInfo::C::Vec::Val props; \
        if (aBaseStructInfo != nullptr) \
        { \
            props = aBaseStructInfo->Props(); \
        } \
        GpTypeStructFactory::SP factory = GpSP<StructFactory>::SNew(); \
        _SCollectStructProps(props); \
        constexpr GpUUID typeUID    = T::STypeUID(); \
        constexpr GpUUID groupUID   = GpUUID::CE_FromString(std::string_view(MACRO_D_TO_STR(MODULE_UUID))); \
        GpTypeStructInfo structInfo(typeUID, \
                                    (aBaseStructInfo != nullptr) ? aBaseStructInfo->UID() : GpUUID(), \
                                    std::string(GpTypeUtils::STypeName<T>()), \
                                    std::move(props), \
                                    groupUID, \
                                    factory); \
        return structInfo;\
    }

//------------------------- PROP -------------------------
#define PROP(PROP_NAME) \
    { \
        using PropT = decltype(PROP_NAME); \
        constexpr const auto    types   = GpTypeUtils::SDetectTypeContainer<PropT>(); \
        const GpUUID            typeUID = GpTypeUtils::SDetectTypeUID<PropT>(); \
        constexpr const size_t  align   = alignof(PropT); \
        constexpr const size_t  size    = sizeof(PropT); \
        const std::ptrdiff_t    offset  = GpTypeUtils::SOffsetOf(&this_type::PROP_NAME); \
        aPropsOut.emplace_back(GpTypePropInfo(std::get<0>(types), \
                                              typeUID, \
                                              std::get<2>(types), \
                                              std::get<1>(types), \
                                              std::string(#PROP_NAME), \
                                              align, \
                                              size, \
                                              offset)); \
    }

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
