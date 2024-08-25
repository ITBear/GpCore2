#include <GpCore2/GpTasks/Fibers/GpTaskFiberCtxForceUnwind.hpp>

#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

GpTaskFiberCtxForceUnwind::GpTaskFiberCtxForceUnwind (void) noexcept
{
}

GpTaskFiberCtxForceUnwind::GpTaskFiberCtxForceUnwind (const GpTaskFiberCtxForceUnwind& aException):
GpException{aException}
{
}

GpTaskFiberCtxForceUnwind::GpTaskFiberCtxForceUnwind (GpTaskFiberCtxForceUnwind&& aException):
GpException{std::move(aException)}
{
}

GpTaskFiberCtxForceUnwind::GpTaskFiberCtxForceUnwind
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept:
GpException{aMsg, aSourceLocation}
{
}

GpTaskFiberCtxForceUnwind::~GpTaskFiberCtxForceUnwind (void) noexcept
{
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
