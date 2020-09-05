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

    const GpTypeStructInfo&         TypeStructInfo      (void) const noexcept {return _TypeStructInfo();}
    GpTypeStructBase::SP            NewInstance         (void) const {return _NewInstance();}

protected:
    virtual void                    _type_id_tag_fn     (_type_id_tag_t&) const noexcept {}
    virtual const GpTypeStructInfo& _TypeStructInfo     (void) const noexcept = 0;
    virtual GpTypeStructBase::SP    _NewInstance        (void) const = 0;
    static const GpTypeStructInfo&  STypeStructInfo     (void);
};

//------------------------- TYPE_STRUCT_DECLARE -------------------------
#define TYPE_STRUCT_DECLARE() \
    using BaseT     = std::remove_reference<decltype(SGetBaseObjectType(&this_type::_type_id_tag_fn))>::type; \
\
    static const GpTypeStructInfo&  STypeStructInfo     (void); \
\
    class StructFactory final: public GpTypeStructFactory \
    { \
    public: \
        virtual GpSP<GpTypeStructBase>  NewInstance (void) const override final; \
    }; \
\
protected: \
    virtual void                    _type_id_tag_fn     (_type_id_tag_t&) const noexcept override{} \
    virtual const GpTypeStructInfo& _TypeStructInfo     (void) const noexcept override; \
    virtual GpTypeStructBase::SP    _NewInstance        (void) const override; \
\
private: \
    static GpTypeStructInfo         _SCollectStructInfo (const GpTypeStructInfo* aBaseStructInfo); \
    static void                     _SCollectStructProps(GpTypePropInfo::C::Vec::Val& aPropsOut); \

//------------------------- TYPE_STRUCT_IMPLEMENT -------------------------
#define MACRO_D_TO_STR(VAL) #VAL

#define TYPE_STRUCT_IMPLEMENT(T, TUUID, MODULE_UUID) \
\
    GpSP<GpTypeStructBase>  T::StructFactory::NewInstance (void) const {return T::SP::SNew();} \
\
    const GpTypeStructInfo& T::STypeStructInfo (void) \
    { \
        static const GpTypeStructInfo sStructInfo = T::_SCollectStructInfo(&T::BaseT::STypeStructInfo()); \
        return sStructInfo; \
    } \
\
    const GpTypeStructInfo& T::_TypeStructInfo (void) const noexcept \
    { \
        return T::STypeStructInfo(); \
    } \
\
    GpTypeStructBase::SP    T::_NewInstance (void) const \
    { \
        return T::SP::SNew(); \
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
        constexpr GpUUID typeUID    = GpUUID::CE_FromString(TUUID); \
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
        constexpr const auto    types           = GpTypeUtils::SDetectTypeContainer<PropT>(); \
        const GpUUID            structTypeUID   = GpTypeUtils::SDetectStructTypeUID<PropT>(); \
        constexpr const size_t  align           = alignof(PropT); \
        constexpr const size_t  size            = sizeof(PropT); \
        const std::ptrdiff_t    offset          = GpTypeUtils::SOffsetOf(&this_type::PROP_NAME); \
        aPropsOut.emplace_back(GpTypePropInfo(std::get<0>(types), \
                                              structTypeUID, \
                                              std::get<2>(types), \
                                              std::get<1>(types), \
                                              std::string(#PROP_NAME), \
                                              align, \
                                              size, \
                                              offset)); \
    }

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
