#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "GpTypeStructDynamic.hpp"

namespace GPlatform {

class GPCORE_API GpTypeStructDynamicUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTypeStructDynamicUtils);

public:
    static GpTypeStructDynamic::SP  SConstruct              (const GpTypeStructInfo& aTypeInfo);
    static void                     SDestroy                (const GpTypeStructInfo&    aTypeInfo,
                                                             void*                      aStructDataPtr);
};

}//namespace GPlatform

#endif//GP_USE_TYPE_SYSTEM
