#include <GpCore2/GpReflection/GpReflectObject.hpp>
#include <GpCore2/GpReflection/GpReflectManager.hpp>
#include <GpCore2/GpReflection/GpReflectUtils.hpp>
#include <GpCore2/GpReflection/GpReflectUtils_IsEqual.hpp>
#include <GpCore2/GpReflection/GpReflectUtils_TotalMemoryUse.hpp>

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

bool    GpReflectObject::ReflectIsEqual (const GpReflectObject& aOtherObject) const
{
    return GpReflectUtils_IsEqual::SDo(*this, aOtherObject, std::nullopt);
}

size_t  GpReflectObject::ReflectTotalMemoryUse (void) const noexcept
{
    return GpReflectUtils_TotalMemoryUse::SDo(*this, std::nullopt);
}

::GPlatform::GpReflectModel::CSP    GpReflectObject::_SReflectModelInit (void)
{
    static ::GPlatform::GpReflectModel::CSP sReflectModel = GpReflectObject::_SReflectCreateModel();
    return sReflectModel;
}

::GPlatform::GpReflectModel::CSP    GpReflectObject::_ReflectModel (void) const
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

::GPlatform::GpReflectModel::CSP    GpReflectObject::_SReflectCreateModel (void)
{
    GpReflectProp::SmallVecVal  props;
    GpReflectObjectFactory::SP  factory     = GpSP<Factory>::SNew();
    constexpr const GpUUID      modelUid    = GpReflectObject::SReflectModelUid();
    constexpr const GpUUID      groupId     = GpUUID::CE_FromString("96155f96-6bc0-434e-a2da-0f9e72368461"_sv);

    GpReflectModel::CSP reflectModelCSP = MakeCSP<GpReflectModel>
    (
        modelUid,
        GpUUID(),
        std::string(GpReflectUtils::SModelName<GpReflectObject>()),
        std::move(props),
        groupId,
        std::move(factory),
        alignof(GpReflectObject),
        sizeof(GpReflectObject)
    );

    ::GPlatform::GpReflectManager::_S_().Register(reflectModelCSP);

    return reflectModelCSP;
}

}// namespace GPlatform
