#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectPropUtils.hpp>
#include <GpCore2/GpReflection/GpReflectObjectFactory.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>

#if defined(GP_REFLECTION_STATIC_ADD_TO_MANAGER)
#   define GP_REFLECTION_STATIC_TYPE_REG_IMPL(T)        GpReflectModel::CSP T::_sReflectModel = T::_SReflectModelInit();
#   define GP_REFLECTION_STATIC_TYPE_REG_DECL    static GpReflectModel::CSP    _sReflectModel;
#else
#   define GP_REFLECTION_STATIC_TYPE_REG_IMPL(T)
#   define GP_REFLECTION_STATIC_TYPE_REG_DECL
#endif// #if defined(GP_REFLECTION_STATIC_ADD_TO_MANAGER)

namespace GPlatform {

class GP_REFLECTION_API GpReflectObject
{
public:
    CLASS_DD(GpReflectObject)

    TAG_SET(GpReflectObject)

    struct  _type_id_tag_t{};
    template<typename T> static T& SBaseType(void(T::*)(typename T::_type_id_tag_t&) const);

    using BaseT = std::false_type;

    class Factory final: public GpReflectObjectFactory
    {
    public:
        using VecWrapT = GpVectorReflectObjWrap<GpReflectObject>;

    public:
        virtual GpReflectObject::SP NewInstanceSP           (const GpUUID& aModelUid) const override final;
        virtual void                ConstructInplace        (void* aDataPtr) const override final;
        virtual void                DestructInplace         (void* aDataPtr) const override final;
        virtual const VecWrapInfoT& VecWrapInfo             (void) const noexcept override final;

    private:
        const VecWrapInfoT          iVecWrapInfo =
        {
            ._align_of  = alignof(VecWrapT),
            ._size_of   = sizeof(VecWrapT)
        };
    };

public:
                                    GpReflectObject         (void) noexcept = default;
    explicit                        GpReflectObject         (const GpReflectObject&) noexcept = default;
    explicit                        GpReflectObject         (GpReflectObject&&) noexcept = default;
    virtual                         ~GpReflectObject        (void) noexcept = default;

    static GpReflectModel::CSP      SReflectModel           (void) noexcept {return GpReflectObject::_sReflectModel;}
    static constexpr GpUUID         SReflectModelUid        (void) noexcept
    {
        constexpr const GpUUID uid = "10000000-0000-0000-0000-000000000001"_uuid;
        return uid;
    }

    GpReflectModel::CSP             ReflectModel            (void) const {return _ReflectModel();}
    const GpUUID                    ReflectModelUid         (void) const noexcept {return _ReflectModel().Vn().Uid();}
    GpReflectObject::SP             ReflectNewInstance      (void) const {return _ReflectNewInstance();}
    GpReflectObject::SP             ReflectClone            (void) const {return _ReflectClone();}
    const void*                     ReflectDataPtr          (void) const noexcept {return _ReflectDataPtr();}
    void*                           ReflectDataPtr          (void) noexcept {return _ReflectDataPtr();}
    bool                            ReflectIsEqual          (const GpReflectObject& aOtherObject) const;
    size_t                          ReflectTotalMemoryUse   (void) const noexcept;

    static GpReflectModel::CSP      _SReflectModelInit      (void);

    template<typename T>
    typename T::SP                  ReflectClone            (void) const;

protected:
    virtual void                    _type_id_tag_fn         (_type_id_tag_t&) const noexcept {}
    virtual GpReflectModel::CSP     _ReflectModel           (void) const;
    virtual GpReflectObject::SP     _ReflectNewInstance     (void) const;
    virtual GpReflectObject::SP     _ReflectClone           (void) const;
    virtual const void*             _ReflectDataPtr         (void) const noexcept;
    virtual void*                   _ReflectDataPtr         (void) noexcept;

private:
    static GpReflectModel::CSP      _SReflectCreateModel    (void);
    GP_REFLECTION_STATIC_TYPE_REG_DECL
};

template<typename T>
typename T::SP  GpReflectObject::ReflectClone (void) const
{
    GpReflectObject::SP val = ReflectClone();
    return val.CastAs<typename T::SP>();
}

}// namespace GPlatform

//------------------------- REFLECT_DECLARE -------------------------
#define REFLECT_DECLARE(TUUID) \
    using BaseT = std::remove_reference<decltype(SBaseType(&this_type::_type_id_tag_fn))>::type; \
\
    class Factory final: public ::GPlatform::GpReflectObjectFactory \
    { \
    public: \
        using VecWrapT = GpVectorReflectObjWrap<this_type>; \
    \
    public: \
        virtual ::GPlatform::GpSP<::GPlatform::GpReflectObject> NewInstanceSP       (const GpUUID& aModelUid) const override final; \
        virtual void                                            ConstructInplace    (void* aDataPtr) const override final; \
        virtual void                                            DestructInplace     (void* aDataPtr) const override final; \
        virtual const VecWrapInfoT&                             VecWrapInfo         (void) const noexcept override final; \
    \
    private: \
        const VecWrapInfoT iVecWrapInfo = \
        { \
            ._align_of  = alignof(VecWrapT), \
            ._size_of   = sizeof(VecWrapT) \
        }; \
    }; \
\
    static ::GPlatform::GpReflectModel::CSP                     SReflectModel       (void) noexcept {return this_type::_sReflectModel;} \
    static constexpr const ::GPlatform::GpUUID                  SReflectModelUid    (void) noexcept \
    { \
        constexpr const ::GPlatform::GpUUID uid(TUUID); \
        return uid; \
    } \
\
    static GpReflectModel::CSP                  _SReflectModelInit          (void); \
\
protected: \
    virtual void                                _type_id_tag_fn             (_type_id_tag_t&) const noexcept override{} \
    virtual ::GPlatform::GpReflectModel::CSP    _ReflectModel               (void) const override; \
    virtual ::GPlatform::GpReflectObject::SP    _ReflectNewInstance         (void) const override; \
    virtual ::GPlatform::GpReflectObject::SP    _ReflectClone               (void) const override; \
\
private: \
    static ::GPlatform::GpReflectModel::CSP     _SReflectCreateModel        (const ::GPlatform::GpReflectModel& aBaseReflectionModel); \
    static void                                 _SReflectCollectProps       (::GPlatform::GpReflectProp::SmallVecVal& aPropsOut);\
\
    GP_REFLECTION_STATIC_TYPE_REG_DECL

//------------------------- REFLECT_IMPLEMENT -------------------------
#define MACRO_D_TO_STR(VAL) #VAL

#define REFLECT_IMPLEMENT(T, MODULE_UUID) \
\
    GP_REFLECTION_STATIC_TYPE_REG_IMPL(T) \
\
    ::GPlatform::GpSP<::GPlatform::GpReflectObject> T::Factory::NewInstanceSP (const GpUUID& /*aModelUid*/) const {return MakeSP<T>();} \
\
    void    T::Factory::ConstructInplace (void* aDataPtr) const {MemOps::SConstruct<T>(static_cast<T*>(aDataPtr), 1);} \
\
    void    T::Factory::DestructInplace (void* aDataPtr) const {MemOps::SDestruct<T>(static_cast<T*>(aDataPtr), 1);} \
\
    const ::GPlatform::GpReflectObjectFactory::VecWrapInfoT&    T::Factory::VecWrapInfo (void) const noexcept {return iVecWrapInfo;} \
\
    ::GPlatform::GpReflectModel::CSP    T::_SReflectModelInit (void) \
    { \
        static ::GPlatform::GpReflectModel::CSP sReflectModel = T::_SReflectCreateModel(T::BaseT::_SReflectModelInit().Vn()); \
        return sReflectModel; \
    } \
\
    ::GPlatform::GpReflectModel::CSP    T::_ReflectModel (void) const \
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
    ::GPlatform::GpReflectModel::CSP    T::_SReflectCreateModel (const ::GPlatform::GpReflectModel& aBaseReflectionModel) \
    { \
        ::GPlatform::GpReflectProp::SmallVecVal props   = aBaseReflectionModel.Props(); \
        ::GPlatform::GpReflectObjectFactory::SP factory = ::GPlatform::GpSP<Factory>::SNew(); \
        _SReflectCollectProps(props); \
        \
        constexpr const ::GPlatform::GpUUID modelUid    = T::SReflectModelUid(); \
        constexpr const ::GPlatform::GpUUID groupId     = ::GPlatform::GpUUID::CE_FromString(std::string_view(MACRO_D_TO_STR(MODULE_UUID))); \
        \
        ::GPlatform::GpReflectModel::CSP reflectModelCSP = MakeCSP<::GPlatform::GpReflectModel> \
        ( \
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
        ::GPlatform::GpReflectManager::_S_().Register(reflectModelCSP);\
        \
        return reflectModelCSP; \
    }

//------------------------- PROP -------------------------
#define PROP(PROP_NAME) \
    ::GPlatform::GpReflectPropUtils::SAddProp<decltype(PROP_NAME)> \
    ( \
        std::string(#PROP_NAME), \
        ::GPlatform::GpReflectUtils::SOffsetOf(&this_type::PROP_NAME), \
        {}, \
        GpReflectProp::FlagArgsT(), \
        std::nullopt, \
        aPropsOut, \
        {} \
    )

#define PROP_F(PROP_NAME, FLAGS) \
    ::GPlatform::GpReflectPropUtils::SAddProp<decltype(PROP_NAME)> \
    ( \
        std::string(#PROP_NAME), \
        ::GPlatform::GpReflectUtils::SOffsetOf(&this_type::PROP_NAME), \
        FLAGS, \
        GpReflectProp::FlagArgsT(), \
        std::nullopt, \
        aPropsOut, \
        {} \
    )

#define PROP_FG(PROP_NAME, FLAGS, GEN_FN) \
    ::GPlatform::GpReflectPropUtils::SAddProp<decltype(PROP_NAME)> \
    ( \
        std::string(#PROP_NAME), \
        ::GPlatform::GpReflectUtils::SOffsetOf(&this_type::PROP_NAME), \
        FLAGS, \
        GpReflectProp::FlagArgsT(), \
        GEN_FN, \
        aPropsOut, \
        {} \
    )

#define PROP_FA(PROP_NAME, FLAGS, FLAG_ARGS) \
    ::GPlatform::GpReflectPropUtils::SAddProp<decltype(PROP_NAME)> \
    ( \
        std::string(#PROP_NAME), \
        ::GPlatform::GpReflectUtils::SOffsetOf(&this_type::PROP_NAME), \
        FLAGS, \
        std::move(FLAG_ARGS), \
        std::nullopt, \
        aPropsOut, \
        {} \
    )
