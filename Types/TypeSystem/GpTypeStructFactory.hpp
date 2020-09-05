#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_TYPE_SYSTEM)

#include "../Classes/GpClassesDefines.hpp"
#include "../Containers/GpContainersT.hpp"
#include "../Pointers/GpSharedPtr.hpp"

namespace GPlatform {

class GpTypeStructBase;

class GpTypeStructFactory
{
public:
    CLASS_DECLARE_DEFAULTS(GpTypeStructFactory)

protected:
                                    GpTypeStructFactory     (void) noexcept {}

public:
    virtual                         ~GpTypeStructFactory    (void) noexcept {}

    virtual GpSP<GpTypeStructBase>  NewInstance             (void) const = 0;
};

}//GPlatform

#endif//GP_USE_TYPE_SYSTEM
