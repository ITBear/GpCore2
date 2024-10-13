#pragma once

#include <GpCore2/Config/GpConfig.hpp>
#include <GpCore2/GpReflection/GpReflectProp.hpp>
#include <GpCore2/GpReflection/GpReflectObject.hpp>

namespace GPlatform {

class GP_REFLECTION_API GpReflectUtils_AssignValue
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpReflectUtils_AssignValue)

public:
    static void     SDo     (void*                  aDataPtrDst,
                             const GpReflectProp&   aPropDst,
                             const void*            aDataPtrSrc,
                             const GpReflectProp&   aPropSrc);

    static void     SDo     (GpReflectObject&       aObjDst,
                             const GpReflectObject& aObjSrc);
};

}// namespace GPlatform
