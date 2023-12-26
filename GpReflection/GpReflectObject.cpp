#include "GpReflectObject.hpp"
#include "GpReflectManager.hpp"
#include "GpReflectUtils.hpp"

#if defined(GP_USE_REFLECTION)

namespace GPlatform {

GP_REFLECTION_STATIC_TYPE_REG_IMPL(GpReflectObject)

GpReflectObject::SP GpReflectObject::Factory::NewInstanceSP (const GpUUID& /*aModelUid*/) const
{
    return MakeSP<GpReflectObject>();
}

void    GpReflectObject::Factory::ConstructInplace (void* aDataPtr) const
{
    MemOps::SConstruct<GpReflectObject>
    (
        static_cast<GpReflectObject*>(aDataPtr),
        1
    );
}

void    GpReflectObject::Factory::DestructInplace (void* aDataPtr) const
{
    MemOps::SDestruct<GpReflectObject>
    (
        static_cast<GpReflectObject*>(aDataPtr),
        1
    );
}

const GpReflectObjectFactory::VecWrapInfoT& GpReflectObject::Factory::VecWrapInfo (void) const noexcept
{
    return iVecWrapInfo;
}

const GpReflectModel&   GpReflectObject::_SReflectModelInit (void)
{
    static const GpReflectModel& sReflectModel = GpReflectObject::_SReflectCreateModel();
    return sReflectModel;
}

const GpReflectModel&   GpReflectObject::_ReflectModel (void) const
{
    return GpReflectObject::SReflectModel();
}

GpReflectObject::SP GpReflectObject::_ReflectNewInstance (void) const
{
    return MakeSP<GpReflectObject>();
}

GpReflectObject::SP GpReflectObject::_ReflectClone (void) const
{
    return MakeSP<GpReflectObject>(*this);
}

const void* GpReflectObject::_ReflectDataPtr (void) const noexcept
{
    return this;
}

void*   GpReflectObject::_ReflectDataPtr (void) noexcept
{
    return this;
}

const GpReflectModel&   GpReflectObject::_SReflectCreateModel (void)
{
    GpReflectProp::C::Vec::Val  props;
    GpReflectObjectFactory::SP  factory     = GpSP<Factory>::SNew();
    constexpr const GpUUID      modelUid    = GpReflectObject::SReflectModelUid();
    constexpr const GpUUID      groupId     = GpUUID::CE_FromString(u8"96155f96-6bc0-434e-a2da-0f9e72368461"_sv);

    GpReflectModel reflectModel
    (
        modelUid,
        GpUUID(),
        std::u8string(GpUTF::S_As_UTF8(GpReflectUtils::SModelName<GpReflectObject>())),
        std::move(props),
        groupId,
        std::move(factory),
        alignof(GpReflectObject),
        sizeof(GpReflectObject)
    );

    return ::GPlatform::GpReflectManager::_S_().Register(reflectModel);
}

}//namespace GPlatform

#endif//GP_USE_REFLECTION
