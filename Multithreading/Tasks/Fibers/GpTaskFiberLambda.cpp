#include "GpTaskFiberLambda.hpp"

namespace GPlatform {

GpTaskFiberLambda::GpTaskFiberLambda
(
    std::optional<OnStartFn>    aOnStartFn,
    std::optional<OnStepFn>     aOnStepFn,
    std::optional<OnStoptFn>    aOnStoptFn
):
GpTaskFiberBase(""_sv),
iOnStartFn(aOnStartFn),
iOnStepFn(aOnStepFn),
iOnStoptFn(aOnStoptFn)
{
}

GpTaskFiberLambda::~GpTaskFiberLambda (void) noexcept
{
}

void    GpTaskFiberLambda::OnStart (void)
{
    if (iOnStartFn.has_value())
    {
        iOnStartFn.value()();
    }
}

GpTaskFiberLambda::ResT GpTaskFiberLambda::OnStep (EventOptRefT aEvent)
{
    if (iOnStepFn.has_value())
    {
        return iOnStepFn.value()(aEvent);
    }

    return ResT::DONE;
}

void    GpTaskFiberLambda::OnStop (void) noexcept
{
    if (iOnStoptFn.has_value())
    {
        iOnStoptFn.value()();
    }
}

}//namespace GPlatform
