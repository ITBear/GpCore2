#include "GpTaskFiberLambda.hpp"

namespace GPlatform {

/*GpTaskFiberLambda::GpTaskFiberLambda
(
    std::u8string               aName,
    std::optional<FnT>          aBeforeStartFn,
    std::optional<OnStepFnT>    aOnStepFn,
    std::optional<FnT>          aAfterStopFn
) noexcept:
GpTaskFiberBase(std::move(aName)),
iBeforeStartFn(aBeforeStartFn),
iOnStepFn(aOnStepFn),
iAfterStopFn(aAfterStopFn)
{
}

GpTaskFiberLambda::~GpTaskFiberLambda (void) noexcept
{
}

void    GpTaskFiberLambda::OnStart (void)
{
    if (iBeforeStartFn.has_value())
    {
        return iBeforeStartFn.value()(*this);
    }
}

GpTaskDoRes GpTaskFiberLambda::OnStep (EventOptRefT aEvent)
{
    if (iOnStepFn.has_value())
    {
        return iOnStepFn.value()(aEvent);
    }

    return GpTaskDoRes::DONE;
}

void    GpTaskFiberLambda::OnStop (void) noexcept
{
    if (iAfterStopFn.has_value())
    {
        return iAfterStopFn.value()(*this);
    }
}*/

}//namespace GPlatform
