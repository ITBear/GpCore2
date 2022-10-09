#pragma once

#include "GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectModel.hpp"
#include "GpReflectObjectFactory.hpp"

#if defined(GP_REFLECTION_STATIC_ADD_TO_MANAGER)
#   define GP_REFLECTION_STATIC_TYPE_REG_IMPL(T)        const GpReflectModel& T::_sReflectModel = T::_SReflectModelInit();
#   define GP_REFLECTION_STATIC_TYPE_REG_DECL    static const GpReflectModel&    _sReflectModel;
#else
#   define GP_REFLECTION_STATIC_TYPE_REG_IMPL(T)
#   define GP_REFLECTION_STATIC_TYPE_REG_DECL
#endif//#if defined(GP_REFLECTION_STATIC_ADD_TO_MANAGER)

namespace GPlatform {

class GP_REFLECTION_API GpReflectObject
{
public:
    CLASS_DD(GpReflectObject)

    struct  _type_id_tag_t{};
    template<typename T> static T& SBaseType(void(T::*)(typename T::_type_id_tag_t&) const);

    using BaseT = std::false_type;

    class Factory final: public GpReflectObjectFactory
    {
    public:
        virtual GpReflectObject::SP NewInstance         (const GpUUID& aModelUid) const override final;
        virtual void                Construct           (void* aDataPtr) const override final;
        virtual void                Destruct            (void* aDataPtr) const override final;
    };

public:
                                    GpReflectObject         (void) noexcept = default;
    explicit                        GpReflectObject         (const GpReflectObject&) noexcept = default;
    explicit                        GpReflectObject         (GpReflectObject&&) noexcept = default;
    virtual                         ~GpReflectObject        (void) noexcept = default;

    static const GpReflectModel&    SReflectModel           (void) noexcept {return GpReflectObject::_sReflectModel;}
    static constexpr GpUUID         SReflectModelUid        (void) noexcept
    {
        constexpr const GpUUID uid = GpUUID::CE_FromString("10000000-0000-0000-0000-000000000001"_sv);
        return uid;
    }

    const GpReflectModel&           ReflectModel            (void) const {return _ReflectModel();}
    const GpUUID                    ReflectModelUid         (void) const noexcept {return _ReflectModel().Uid();}
    GpReflectObject::SP             ReflectNewInstance      (void) const {return _ReflectNewInstance();}
    GpReflectObject::SP             ReflectClone            (void) const {return _ReflectClone();}
    const void*                     ReflectDataPtr          (void) const noexcept {return _ReflectDataPtr();}
    void*                           ReflectDataPtr          (void) noexcept {return _ReflectDataPtr();}

    static const GpReflectModel&    _SReflectModelInit      (void);

    template<typename T>
    typename T::SP                  ReflectClone            (void) const;

protected:
    virtual void                    _type_id_tag_fn         (_type_id_tag_t&) const noexcept {}
    virtual const GpReflectModel&   _ReflectModel           (void) const;
    virtual GpReflectObject::SP     _ReflectNewInstance     (void) const;
    virtual GpReflectObject::SP     _ReflectClone           (void) const;
    virtual const void*             _ReflectDataPtr         (void) const noexcept;
    virtual void*                   _ReflectDataPtr         (void) noexcept;

private:
    static const GpReflectModel&    _SReflectCreateModel    (void);
    GP_REFLECTION_STATIC_TYPE_REG_DECL;
};

template<typename T>
typename T::SP  GpReflectObject::ReflectClone (void) const
{
    GpReflectObject::SP val = ReflectClone();
    return val.CastUpAs<typename T::SP>();
}

}//GPlatform

//------------------------- REFLECT_DECLARE -------------------------
#define REFLECT_DECLARE(TUUID) \
    using BaseT = std::remove_reference<decltype(SBaseType(&this_type::_type_id_tag_fn))>::type; \
\
    class Factory final: public ::GPlatform::GpReflectObjectFactory \
    { \
    public: \
        virtual ::GPlatform::GpSP<::GPlatform::GpReflectObject> NewInstance (const GpUUID& aModelUid) const override final; \
        virtual void                                            Construct   (void* aDataPtr) const override final; \
        virtual void                                            Destruct    (void* aDataPtr) const override final; \
    }; \
\
    static const ::GPlatform::GpReflectModel&   SReflectModel           (void) noexcept {return this_type::_sReflectModel;} \
    constexpr static const ::GPlatform::GpUUID  SReflectModelUid        (void) noexcept \
    { \
        constexpr const ::GPlatform::GpUUID uid(TUUID); \
        return uid; \
    } \
\
    static const GpReflectModel&                _SReflectModelInit      (void); \
\
protected: \
    virtual void                                _type_id_tag_fn         (_type_id_tag_t&) const noexcept override{} \
    virtual const ::GPlatform::GpReflectModel&  _ReflectModel           (void) const override; \
    virtual ::GPlatform::GpReflectObject::SP    _ReflectNewInstance     (void) const override; \
    virtual ::GPlatform::GpReflectObject::SP    _ReflectClone           (void) const override; \
\
private: \
    static const ::GPlatform::GpReflectModel&   _SReflectCreateModel    (const ::GPlatform::GpReflectModel& aBaseReflectionModel); \
    static void                                 _SReflectCollectProps   (::GPlatform::GpReflectProp::C::Vec::Val& aPropsOut);\
\
    GP_REFLECTION_STATIC_TYPE_REG_DECL;

//------------------------- REFLECT_IMPLEMENT -------------------------
#define MACRO_D_TO_STR(VAL) #VAL

#define REFLECT_IMPLEMENT(T, MODULE_UUID) \
\
    GP_REFLECTION_STATIC_TYPE_REG_IMPL(T); \
\
    ::GPlatform::GpSP<::GPlatform::GpReflectObject> T::Factory::NewInstance (const GpUUID& /*aModelUid*/) const {return MakeSP<T>();} \
\
    void    T::Factory::Construct (void* aDataPtr) const {MemOps::SConstruct<T>(static_cast<T*>(aDataPtr), 1);} \
\
    void    T::Factory::Destruct (void* aDataPtr) const {MemOps::SDestruct<T>(static_cast<T*>(aDataPtr), 1);} \
\
    const ::GPlatform::GpReflectModel&  T::_SReflectModelInit (void) \
    { \
        static const ::GPlatform::GpReflectModel& sReflectModel = T::_SReflectCreateModel(T::BaseT::_SReflectModelInit()); \
        return sReflectModel; \
    } \
\
    const ::GPlatform::GpReflectModel&  T::_ReflectModel (void) const \
    { \
        return T::SReflectModel(); \
    } \
\
    ::GPlatform::GpReflectObject::SP    T::_ReflectNewInstance (void) const \
    { \
        return MakeSP<T>(); \
    } \
\
    ::GPlatform::GpReflectObject::SP    T::_ReflectClone (void) const \
    { \
        return MakeSP<T>(*this); \
    } \
\
    const ::GPlatform::GpReflectModel&  T::_SReflectCreateModel (const ::GPlatform::GpReflectModel& aBaseReflectionModel) \
    { \
        ::GPlatform::GpReflectProp::C::Vec::Val props   = aBaseReflectionModel.Props(); \
        ::GPlatform::GpReflectObjectFactory::SP factory = ::GPlatform::GpSP<Factory>::SNew(); \
        _SReflectCollectProps(props); \
        \
        constexpr const ::GPlatform::GpUUID modelUid    = T::SReflectModelUid(); \
        constexpr const ::GPlatform::GpUUID groupId     = ::GPlatform::GpUUID::CE_FromString(std::string_view(MACRO_D_TO_STR(MODULE_UUID))); \
        \
        ::GPlatform::GpReflectModel reflectModel\
        (\
            modelUid, \
            aBaseReflectionModel.Uid(), \
            std::string(::GPlatform::GpReflectUtils::SModelName<T>()), \
            std::move(props), \
            groupId, \
            std::move(factory), \
            alignof(T), \
            sizeof(T) \
        ); \
        \
        return ::GPlatform::GpReflectManager::_S_().Register(reflectModel);\
    }

//------------------------- PROP -------------------------
#define PROP(PROP_NAME) \
{ \
    ::GPlatform::GpReflectUtils::SAddProp<decltype(PROP_NAME)> \
    ( \
        std::string(#PROP_NAME), \
        ::GPlatform::GpReflectUtils::SOffsetOf(&this_type::PROP_NAME), \
        {}, \
        GpReflectProp::FlagArgsT(), \
        std::nullopt, \
        aPropsOut \
    ); \
}

#define PROP_F(PROP_NAME, FLAGS) \
{ \
    ::GPlatform::GpReflectUtils::SAddProp<decltype(PROP_NAME)> \
    ( \
        std::string(#PROP_NAME), \
        ::GPlatform::GpReflectUtils::SOffsetOf(&this_type::PROP_NAME), \
        FLAGS, \
        GpReflectProp::FlagArgsT(), \
        std::nullopt, \
        aPropsOut \
    ); \
}

#define PROP_FG(PROP_NAME, FLAGS, GEN_FN) \
{ \
    ::GPlatform::GpReflectUtils::SAddProp<decltype(PROP_NAME)> \
    ( \
        std::string(#PROP_NAME), \
        ::GPlatform::GpReflectUtils::SOffsetOf(&this_type::PROP_NAME), \
        FLAGS, \
        GpReflectProp::FlagArgsT(), \
        GEN_FN, \
        aPropsOut \
    ); \
}

#define PROP_FA(PROP_NAME, FLAGS, FLAG_ARGS) \
{ \
    ::GPlatform::GpReflectUtils::SAddProp<decltype(PROP_NAME)> \
    ( \
        std::string(#PROP_NAME), \
        ::GPlatform::GpReflectUtils::SOffsetOf(&this_type::PROP_NAME), \
        FLAGS, \
        std::move(FLAG_ARGS), \
        std::nullopt, \
        aPropsOut \
    ); \
}

#endif//GP_USE_REFLECTION
