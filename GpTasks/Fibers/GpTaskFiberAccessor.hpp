#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "GpTaskFiber.hpp"

namespace GPlatform {

class GpTaskFiberAccessor
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpTaskFiberAccessor)

public:
    static void             SFiberFn    (GpTaskFiber&       aTaskFiber,
                                         GpThreadStopToken  aStopToken) {aTaskFiber.FiberFn(aStopToken);}
    static GpTaskFiberCtx*  SCtx        (void) {return GpTaskFiber::SCtx();}
    static void             SClearCtx   (GpTaskFiber& aTaskFiber) {aTaskFiber.ClearCtx();}
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
