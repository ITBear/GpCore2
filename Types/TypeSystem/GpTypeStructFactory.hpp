#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "../Classes/GpClassesDefines.hpp"
#include "../Containers/GpContainersT.hpp"
#include "../Pointers/GpSharedPtr.hpp"

namespace GPlatform {

class GpTypeStructBase;
class GpTypeStructInfo;

class GpTypeStructFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTypeStructFactory)
    CLASS_DECLARE_DEFAULTS(GpTypeStructFactory)

protected:
                                    GpTypeStructFactory     (void) noexcept {}

public:
    virtual                         ~GpTypeStructFactory    (void) noexcept {}

    virtual GpSP<GpTypeStructBase>  NewInstance             (const GpTypeStructInfo& aStructInfo) const = 0;
    virtual void                    ConstructStruct         (void* aStructDataPtr) const = 0;
    virtual void                    DestructStruct          (void* aStructDataPtr) const = 0;
};

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
