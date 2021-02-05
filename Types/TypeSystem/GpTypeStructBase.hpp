#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructInfo.hpp"

#if defined(GP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER)
#   define GP_TYPE_STRUCT_STATIC_TYPE_REG_IMPL(T) const size_t T::_sStaticTypeReg = ::GPlatform::GpTypeManager::S().Register(T::STypeInfo())
#   define GP_TYPE_STRUCT_STATIC_TYPE_REG_DECL    static const size_t _sStaticTypeReg
#else
#   define GP_TYPE_STRUCT_STATIC_TYPE_REG_IMPL(T)
#   define GP_TYPE_STRUCT_STATIC_TYPE_REG_DECL
#endif//#if defined(GP_TYPE_SYSTEM_STATIC_ADD_TO_MANAGER)

namespace GPlatform {

class GPCORE_API GpTypeStructBase
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypeStructBase)

    struct  _type_id_tag_t{};
    template<typename T> static T& SGetBaseObjectType(void(T::*)(typename T::_type_id_tag_t&) const);

    using BaseT = std::false_type;

    class StructFactory final: public GpTypeStructFactory
    {
    public:
        virtual GpTypeStructBase::SP    NewInstance (void) const override final;
    };

public:
                                    GpTypeStructBase    (void) noexcept {}
    virtual                         ~GpTypeStructBase   (void) noexcept {}

    static const GpTypeStructInfo&  STypeInfo           (void);
    static constexpr GpUUID         STypeUID            (void) noexcept
    {
        constexpr const GpUUID structUID = GpUUID::CE_FromString("000056df-0000-4be7-0000-97c0c4060000"_sv);
        return structUID;
    }

    const GpTypeStructInfo&         TypeInfo            (void) const noexcept {return _TypeInfo();}
    const GpUUID                    TypeUID             (void) const noexcept {return TypeInfo().UID();}
    GpTypeStructBase::SP            NewInstance         (void) const {return _NewInstance();}

    static size_t                   SStaticTypeReg      (void);

protected:
    virtual void                    _type_id_tag_fn     (_type_id_tag_t&) const noexcept {}
    virtual const GpTypeStructInfo& _TypeInfo           (void) const noexcept;
    virtual GpTypeStructBase::SP    _NewInstance        (void) const;   

private:
    static GpTypeStructInfo         _SCollectStructInfo (void);
    GP_TYPE_STRUCT_STATIC_TYPE_REG_DECL;
};

}//GPlatform

//------------------------- TYPE_STRUCT_DECLARE -------------------------
#define TYPE_STRUCT_DECLARE(TUUID) \
    using BaseT = std::remove_reference<decltype(SGetBaseObjectType(&this_type::_type_id_tag_fn))>::type; \
\
    class StructFactory final: public ::GPlatform::GpTypeStructFactory \
    { \
    public: \
        virtual ::GPlatform::GpSP<::GPlatform::GpTypeStructBase>    NewInstance (void) const override final; \
    }; \
\
    static const ::GPlatform::GpTypeStructInfo& STypeInfo   (void); \
    constexpr static const ::GPlatform::GpUUID  STypeUID    (void) noexcept \
    { \
        constexpr const ::GPlatform::GpUUID structUID = ::GPlatform::GpUUID::CE_FromString(TUUID); \
        return structUID; \
    } \
\
    static size_t                                   SStaticTypeReg  (void); \
\
protected: \
    virtual void                                    _type_id_tag_fn (_type_id_tag_t&) const noexcept override{} \
    virtual const ::GPlatform::GpTypeStructInfo&    _TypeInfo       (void) const noexcept override; \
    virtual ::GPlatform::GpTypeStructBase::SP       _NewInstance    (void) const override; \
\
private: \
    static ::GPlatform::GpTypeStructInfo            _SCollectStructInfo (const ::GPlatform::GpTypeStructInfo* aBaseStructInfo); \
    static void                                     _SCollectStructProps(::GPlatform::GpTypePropInfo::C::Vec::Val& aPropsOut);\
\
    GP_TYPE_STRUCT_STATIC_TYPE_REG_DECL;


//------------------------- TYPE_STRUCT_IMPLEMENT -------------------------
#define MACRO_D_TO_STR(VAL) #VAL

#define TYPE_STRUCT_IMPLEMENT(T, MODULE_UUID) \
\
    GP_TYPE_STRUCT_STATIC_TYPE_REG_IMPL(T); \
\
    ::GPlatform::GpSP<::GPlatform::GpTypeStructBase>    T::StructFactory::NewInstance (void) const {return MakeSP<T>();} \
\
    const ::GPlatform::GpTypeStructInfo&    T::STypeInfo (void) \
    { \
        static const ::GPlatform::GpTypeStructInfo sStructInfo = T::_SCollectStructInfo(&T::BaseT::STypeInfo()); \
        return sStructInfo; \
    } \
\
    size_t  T::SStaticTypeReg (void) \
    { \
        return T::_sStaticTypeReg; \
    } \
\
    const ::GPlatform::GpTypeStructInfo&    T::_TypeInfo (void) const noexcept \
    { \
        return T::STypeInfo(); \
    } \
\
    ::GPlatform::GpTypeStructBase::SP   T::_NewInstance (void) const \
    { \
        return MakeSP<T>(); \
    } \
\
    ::GPlatform::GpTypeStructInfo   T::_SCollectStructInfo (const ::GPlatform::GpTypeStructInfo* aBaseStructInfo) \
    { \
        ::GPlatform::GpTypePropInfo::C::Vec::Val props; \
        if (aBaseStructInfo != nullptr) \
        { \
            props = aBaseStructInfo->Props(); \
        } \
        ::GPlatform::GpTypeStructFactory::SP factory = ::GPlatform::GpSP<StructFactory>::SNew(); \
        _SCollectStructProps(props); \
        constexpr ::GPlatform::GpUUID typeUID   = T::STypeUID(); \
        constexpr ::GPlatform::GpUUID groupUID  = ::GPlatform::GpUUID::CE_FromString(std::string_view(MACRO_D_TO_STR(MODULE_UUID))); \
        ::GPlatform::GpTypeStructInfo structInfo \
        (\
            typeUID, \
            (aBaseStructInfo != nullptr) ? aBaseStructInfo->UID() : ::GPlatform::GpUUID(), \
            std::string(::GPlatform::GpTypeUtils::STypeName<T>()), \
            std::move(props), \
            groupUID, \
            factory \
        ); \
        return structInfo;\
    }

//------------------------- PROP -------------------------
#define PROP(PROP_NAME) \
{ \
    ::GPlatform::GpTypeUtils::SAddProp<decltype(PROP_NAME)> \
    ( \
        std::string(#PROP_NAME), \
        ::GPlatform::GpTypeUtils::SOffsetOf(&this_type::PROP_NAME), \
        {}, \
        aPropsOut \
    ); \
}

#define PROP_F(PROP_NAME, FLAGS) \
{ \
    ::GPlatform::GpTypeUtils::SAddProp<decltype(PROP_NAME)> \
    ( \
        std::string(#PROP_NAME), \
        ::GPlatform::GpTypeUtils::SOffsetOf(&this_type::PROP_NAME), \
        FLAGS, \
        aPropsOut \
    ); \
}

#endif//GP_USE_TYPE_SYSTEM
