#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectModel.hpp>
#include <GpCore2/GpReflection/Models/GpReflectModelDesc.hpp>

namespace GPlatform {

class GpReflectModelPropBuilder;

class GP_REFLECTION_API GpReflectModelDescUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectModelDescUtils)

public:
    static GpReflectModel::CSP              SDescToModel            (const GpReflectModelDesc& aDesc);
    static GpReflectModelDesc::SP           SModelToDesc            (const GpReflectModel& aModel);

private:
    static GpReflectPropDesc::C::Vec::SP    SModelToDescProps       (const GpReflectProp::SmallVecVal& aProps);
    static GpReflectModelPropBuilder        SDescToModelPropBuilder (const GpReflectPropDesc::C::Vec::SP& aPropsDesc);

    static void                             SAddProps_Val           (const GpReflectPropDesc&   aPropDesc,
                                                                     GpReflectModelPropBuilder& aPropBuilder);
    static void                             SAddProps_Vec           (const GpReflectPropDesc&   aPropDesc,
                                                                     GpReflectModelPropBuilder& aPropBuilder);
    static void                             SAddProps_Map           (const GpReflectPropDesc&   aPropDesc,
                                                                     GpReflectModelPropBuilder& aPropBuilder);
};

}// namespace GPlatform
