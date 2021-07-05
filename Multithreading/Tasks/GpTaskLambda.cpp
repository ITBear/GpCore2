#include "GpTaskLambda.hpp"

namespace GPlatform {

GpTaskLambda::GpTaskLambda
(
    std::optional<OnStartFn>    aOnStartFn,
    std::optional<OnStepFn>     aOnStepFn,
    std::optional<OnStoptFn>    aOnStoptFn
):
GpTaskBase(""_sv),
iOnStartFn(aOnStartFn),
iOnStepFn(aOnStepFn),
iOnStoptFn(aOnStoptFn)
{
}

GpTaskLambda::~GpTaskLambda (void) noexcept
{
}

void    GpTaskLambda::OnStart (void)
{
    if (iOnStartFn.has_value())
    {
        iOnStartFn.value()();
    }
}

GpTaskLambda::ResT  GpTaskLambda::OnStep (EventOptRefT aEvent)
{
    if (iOnStepFn.has_value())
    {
        return iOnStepFn.value()(aEvent);
    }

    return ResT::DONE;
}

void    GpTaskLambda::OnStop (void) noexcept
{
    if (iOnStoptFn.has_value())
    {
        iOnStoptFn.value()();
    }
}

}//namespace GPlatform
