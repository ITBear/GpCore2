#include "GpReflectObjectDynamic.hpp"
#include "GpReflectObjectDynamicUtils.hpp"
#include "GpReflectManager.hpp"

#include <GpCore2/GpReflection/GpReflectUtils_AssignValue.hpp>

namespace GPlatform {

GpReflectObject::SP GpReflectObjectDynamic::Factory::NewInstanceSP (const GpUUID& aModelUid) const
{
    return GpReflectObjectDynamicUtils::SConstruct(aModelUid);
}

void    GpReflectObjectDynamic::Factory::ConstructInplace (void* /*aDataPtr*/) const
{
    THROW_GP("Use GpReflectObjectDynamic::SP");
}

void    GpReflectObjectDynamic::Factory::DestructInplace (void* /*aDataPtr*/) const
{
    THROW_GP("Use GpReflectObjectDynamic::SP");
}

const GpReflectObjectFactory::VecWrapInfoT& GpReflectObjectDynamic::Factory::VecWrapInfo (void) const noexcept
{
    // TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

GpReflectObjectDynamic::~GpReflectObjectDynamic (void) noexcept
{
    if (iData != nullptr)
    {
        GpReflectModel::CSP model = _ReflectModel();
        GpReflectObjectDynamicUtils::SDestroy(model.Vn(), iData);
        iData = nullptr;
    }
}

GpReflectModel::CSP GpReflectObjectDynamic::_ReflectModel (void) const
{
    return GpReflectManager::S().Find(iModelUid);
}

GpReflectObject::SP GpReflectObjectDynamic::_ReflectNewInstance (void) const
{
    Factory factory;
    return factory.NewInstanceSP(iModelUid);
}

GpReflectObject::SP GpReflectObjectDynamic::_ReflectClone (void) const
{
    // Factory factory
    GpReflectObject::SP clonedObjectSP = _ReflectNewInstance();

    // Copy
    GpReflectUtils_AssignValue::SDo(clonedObjectSP.Vn(), *this);

    return clonedObjectSP;
}

const void* GpReflectObjectDynamic::_ReflectDataPtr (void) const noexcept
{
    return iData;
}

void*   GpReflectObjectDynamic::_ReflectDataPtr (void) noexcept
{
    return iData;
}

}// namespace GPlatform
