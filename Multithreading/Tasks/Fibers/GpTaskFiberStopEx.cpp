#include "GpTaskFiberStopEx.hpp"

#if defined(GP_USE_MULTITHREADING)
#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

GpTaskFiberStopEx::GpTaskFiberStopEx (GpTaskFiberStopEx&& aException) noexcept:
GpException(std::move(aException))
{
}

GpTaskFiberStopEx::GpTaskFiberStopEx (std::string_view          aMsg,
                                      const SourceLocationT&    aSourceLocation) noexcept:
GpException(aMsg, aSourceLocation)
{
}

GpTaskFiberStopEx::~GpTaskFiberStopEx (void) noexcept
{
}

}//namespace GPlatform

#endif//#if defined(GP_USE_MULTITHREADING_FIBERS)
#endif//#if defined(GP_USE_MULTITHREADING)
