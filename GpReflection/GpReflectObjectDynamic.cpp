#include "GpReflectObjectDynamic.hpp"
#include "GpReflectObjectDynamicUtils.hpp"
#include "GpReflectManager.hpp"

namespace GPlatform {

GpReflectObject::SP GpReflectObjectDynamic::Factory::NewInstance (const GpUUID& aModelUid) const
{
    return GpReflectObjectDynamicUtils::SConstruct(aModelUid);
}

void    GpReflectObjectDynamic::Factory::Construct (void* /*aDataPtr*/) const
{
    THROW_GP("Use GpReflectObjectDynamic::SP"_sv);
}

void    GpReflectObjectDynamic::Factory::Destruct (void* /*aDataPtr*/) const
{
    THROW_GP("Use GpReflectObjectDynamic::SP"_sv);
}

GpReflectObjectDynamic::~GpReflectObjectDynamic (void) noexcept
{
    if (iData != nullptr)
    {
        const GpReflectModel& model = _ReflectModel();
        GpReflectObjectDynamicUtils::SDestroy(model, iData);
        iData = nullptr;
    }
}

const GpReflectModel&   GpReflectObjectDynamic::_ReflectModel (void) const
{
    return GpReflectManager::S().Find(iModelUid);
}

GpReflectObject::SP GpReflectObjectDynamic::_ReflectNewInstance (void) const
{
    Factory factory;
    return factory.NewInstance(iModelUid);
}

GpReflectObject::SP GpReflectObjectDynamic::_ReflectClone (void) const
{
    //Factory factory;
    //GpReflectObject::SP objectSP = factory.NewInstance(iModelUid);

    //TODO: implement
    THROW_GP_NOT_IMPLEMENTED();
}

const void* GpReflectObjectDynamic::_ReflectDataPtr (void) const noexcept
{
    return iData;
}

void*   GpReflectObjectDynamic::_ReflectDataPtr (void) noexcept
{
    return iData;
}

}//namespace GPlatform
