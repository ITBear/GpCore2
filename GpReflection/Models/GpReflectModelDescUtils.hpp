#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_REFLECTION)

#include "../GpReflectModel.hpp"
#include "GpReflectModelDesc.hpp"

namespace GPlatform {

class GpReflectPropBuilder;

class GP_REFLECTION_API GpReflectModelDescUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectModelDescUtils)

public:
    static GpReflectModel                   SDescToModel            (const GpReflectModelDesc& aDesc);
    static GpReflectModelDesc::SP           SModelToDesc            (const GpReflectModel& aModel);

private:
    static GpReflectPropDesc::C::Vec::SP    SModelToDescProps       (const GpReflectProp::C::Vec::Val& aProps);
    static GpReflectPropBuilder             SDescToModelPropBuilder (const GpReflectPropDesc::C::Vec::SP& aPropsDesc);

    static void                             SAddProps_Val           (const GpReflectPropDesc&   aPropDesc,
                                                                     GpReflectPropBuilder&      aPropBuilder);
    static void                             SAddProps_Vec           (const GpReflectPropDesc&   aPropDesc,
                                                                     GpReflectPropBuilder&      aPropBuilder);
    static void                             SAddProps_VecWrap       (const GpReflectPropDesc&   aPropDesc,
                                                                     GpReflectPropBuilder&      aPropBuilder);
    static void                             SAddProps_Map           (const GpReflectPropDesc&   aPropDesc,
                                                                     GpReflectPropBuilder&      aPropBuilder);
};

}//namespace GPlatform

#endif//GP_USE_REFLECTION
