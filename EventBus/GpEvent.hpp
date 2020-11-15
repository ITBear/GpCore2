#pragma once

#include "../Config/GpConfig.hpp"

#if defined (GP_USE_EVENT_BUS)

#include "../GpCore_global.hpp"
#include "../Types/Classes/GpClassesDefines.hpp"
#include "../Types/Containers/GpContainersT.hpp"
#include "../Types/TypeSystem/GpTypeSystem.hpp"

namespace GPlatform {

class GPCORE_API GpEvent: public GpTypeStructBase
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpEvent)   
    CLASS_DECLARE_DEFAULTS(GpEvent)
    TYPE_STRUCT_DECLARE("72fee390-068b-41eb-a32f-7eef3173fad7"_sv)

public:
                        GpEvent     (void) noexcept;
    virtual             ~GpEvent    (void) noexcept;
};

}//GPlatform

#endif//#if defined (GP_USE_EVENT_BUS)
