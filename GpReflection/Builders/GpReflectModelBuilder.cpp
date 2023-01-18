#include "GpReflectModelBuilder.hpp"

#if defined(GP_USE_REFLECTION)

#include "../GpReflectObjectDynamic.hpp"

namespace GPlatform {

GpReflectModelBuilder::GpReflectModelBuilder (void) noexcept
{
}

GpReflectModelBuilder::~GpReflectModelBuilder (void) noexcept
{
}

GpReflectModel  GpReflectModelBuilder::Build (void)
{
    if (iPropBuilder.has_value())
    {
        GpReflectPropBuilder& propBuilder = iPropBuilder.value();

        return GpReflectModel
        (
            iUID,
            iBaseUID,
            std::move(iName),
            propBuilder.MoveProps(),
            iGroupID,
            GpSP<GpReflectObjectDynamic::Factory>::SNew(),
            propBuilder.MaxAlign(),
            propBuilder.TotalSize()
        );
    } else//no props (even in base structure)
    {
        return GpReflectModel
        (
            iUID,
            iBaseUID,
            std::move(iName),
            iGroupID,
            GpSP<GpReflectObjectDynamic::Factory>::SNew()
        );
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_REFLECTION)
