#include "GpTaskFiberCtxForceUnwind.hpp"

#if defined(GP_USE_MULTITHREADING_FIBERS)

namespace GPlatform {

GpTaskFiberCtxForceUnwind::~GpTaskFiberCtxForceUnwind (void) noexcept
{
}

}// namespace GPlatform

#endif// #if defined(GP_USE_MULTITHREADING_FIBERS)
