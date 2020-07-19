#pragma once

#include "../Config/GpConfig.hpp"

#if defined (GP_USE_EVENT_BUS)

#include "../GpCore_global.hpp"
#include "../Types/Classes/GpClassesDefines.hpp"
#include "../Types/Containers/GpContainersT.hpp"

namespace GPlatform {

class GPCORE_API GpEvent
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpEvent)
    CLASS_DECLARE_DEFAULTS(GpEvent)

public:
                        GpEvent     (void) noexcept = default;
    virtual             ~GpEvent    (void) noexcept = default;
};

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
