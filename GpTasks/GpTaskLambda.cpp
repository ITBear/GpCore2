#include "GpTaskLambda.hpp"

namespace GPlatform {

GpTaskLambda::GpTaskLambda
(
    std::optional<OnStartFnT>   aOnStartFn,
    std::optional<OnStepFnT>    aOnStepFn,
    std::optional<OnStoptFnT>   aOnStoptFn
):
GpTaskBase(u8""),
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

GpTaskDoRes GpTaskLambda::OnStep (EventOptRefT aEvent)
{
    if (iOnStepFn.has_value())
    {
        return iOnStepFn.value()(aEvent);
    }

    return GpTaskDoRes::DONE;
}

void    GpTaskLambda::OnStop (void) noexcept
{
    if (iOnStoptFn.has_value())
    {
        iOnStoptFn.value()();
    }
}

}//namespace GPlatform
