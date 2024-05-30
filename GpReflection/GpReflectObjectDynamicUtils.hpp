#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#include "GpReflectObjectDynamic.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectObjectDynamicUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectObjectDynamicUtils)

public:
    static GpReflectObjectDynamic::SP       SConstruct  (const GpReflectModel& aModel);
    static GpReflectObjectDynamic::SP       SConstruct  (const GpUUID&  aModelUid);
    static void                             SDestroy    (const GpReflectModel&  aModel,
                                                         void*                  aDataPtr);
};

}// namespace GPlatform
