#pragma once

#include "GpReflection_global.hpp"

#if defined(GP_USE_REFLECTION)

#include "GpReflectObjectDynamic.hpp"

namespace GPlatform {

class GP_REFLECTION_API GpReflectObjectDynamicUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectObjectDynamicUtils);

public:
    static GpReflectObjectDynamic::SP       SConstruct  (const GpReflectModel& aModel);
    static GpReflectObjectDynamic::SP       SConstruct  (const GpUUID&  aModelUid);
    static void                             SDestroy    (const GpReflectModel&  aModel,
                                                         void*                  aDataPtr);
};

}//namespace GPlatform

#endif//GP_USE_REFLECTION
