#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_MULTITHREADING)

#include "GpTask.hpp"

namespace GPlatform {

class GPCORE_API GpTaskBase: public GpTask
{
public:
    CLASS_REMOVE_CTRS(GpTaskBase)
    CLASS_DECLARE_DEFAULTS(GpTaskBase)

    using EventOptRefT  = std::optional<std::reference_wrapper<GpEvent>>;

protected:
                            GpTaskBase      (std::string_view aName);
    virtual                 ~GpTaskBase     (void) noexcept override;

    virtual ResT            Do              (GpThreadStopToken aStopToken) noexcept override final;
    virtual void            Terminate       (void) noexcept override;

protected:
    virtual void            OnStart         (void) = 0;
    virtual ResT            OnStep          (EventOptRefT aEvent) = 0;
    virtual void            OnStop          (void) noexcept = 0;

private:
    bool                    iIsStarted = false;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)
