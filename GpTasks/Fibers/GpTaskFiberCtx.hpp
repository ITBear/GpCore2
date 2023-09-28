#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

#include "../../GpUtils/Macro/GpMacroClass.hpp"
#include "../../GpUtils/Types/Containers/GpContainersT.hpp"
#include "../../GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp"
#include "../GpTaskEnums.hpp"

namespace GPlatform {

class GpTaskFiber;

class GpTaskFiberCtx
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFiberCtx)
    CLASS_DD(GpTaskFiberCtx)

protected:
                                        GpTaskFiberCtx  (void) noexcept = default;

public:
    virtual                             ~GpTaskFiberCtx (void) noexcept = default;

    virtual std::optional<GpException>  Clear           (void) noexcept = 0;
    virtual GpTaskRunRes::EnumT         Enter           (GpTaskFiber& aTaskFiber) = 0;
    virtual void                        Yield           (const GpTaskRunRes::EnumT aRunRes) = 0;
    virtual void                        Yield           (const milliseconds_t aTimeout) = 0;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
