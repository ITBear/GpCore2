#pragma once

#include "../../../Config/GpConfig.hpp"

#if defined(GP_USE_TIMERS)

#include "../../GpUtils_global.hpp"
#include "../../Macro/GpMacroClass.hpp"
#include "../../Types/Containers/GpContainersT.hpp"

namespace GPlatform {

class GpTimerShotEvent;

class GP_UTILS_API GpTimerShotEventFactory
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTimerShotEventFactory)
    CLASS_DD(GpTimerShotEventFactory)

public:
                                    GpTimerShotEventFactory     (void) noexcept = default;
    virtual                         ~GpTimerShotEventFactory    (void) noexcept = default;

    virtual GpSP<GpTimerShotEvent>  NewInstance                 (void) const;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_TIMERS)
