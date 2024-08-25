#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)

#include <GpCore2/GpTasks/GpTaskEnums.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Containers/GpContainersT.hpp>
#include <GpCore2/GpUtils/Types/Units/SI/GpUnitsSI_Time.hpp>

namespace GPlatform {

class GpTaskFiber;

class GpTaskFiberCtx
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpTaskFiberCtx)
    CLASS_DD(GpTaskFiberCtx)

    enum class TimeoutRes
    {
        WAKEUP_BY_TIMEOUT,
        WAKEUP_BY_OTHER_EVENT
    };

protected:
                                        GpTaskFiberCtx  (void) noexcept = default;

public:
    virtual                             ~GpTaskFiberCtx (void) noexcept = default;

    virtual GpException::C::Opt         Clear           (void) noexcept = 0;
    virtual GpTaskRunRes::EnumT         Enter           (GpTaskFiber& aTaskFiber) = 0;
    virtual void                        CallYield       (GpTaskRunRes::EnumT aRunRes) = 0;
    [[nodiscard]] virtual TimeoutRes    CallYield       (milliseconds_t aTimeout) = 0;
};

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
