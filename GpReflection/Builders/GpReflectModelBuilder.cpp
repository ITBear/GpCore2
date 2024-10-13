#include <GpCore2/GpReflection/Builders/GpReflectModelBuilder.hpp>
#include <GpCore2/GpReflection/GpReflectObjectDynamic.hpp>

namespace GPlatform {

GpReflectModelBuilder::GpReflectModelBuilder (void) noexcept
{
}

GpReflectModelBuilder::~GpReflectModelBuilder (void) noexcept
{
}

GpReflectModel::CSP GpReflectModelBuilder::Build (void)
{
    if (iPropBuilder.has_value())
    {
        GpReflectModelPropBuilder& propBuilder = iPropBuilder.value();

        return MakeCSP<GpReflectModel>
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
    } else// no props (even in base structure)
    {
        return MakeCSP<GpReflectModel>
        (
            iUID,
            iBaseUID,
            std::move(iName),
            iGroupID,
            GpSP<GpReflectObjectDynamic::Factory>::SNew()
        );
    }
}

}// namespace GPlatform
