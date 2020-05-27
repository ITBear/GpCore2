#include "GpRunnable.hpp"
#include "../../Exceptions/GpExceptionsSink.hpp"

#if defined(GP_USE_MULTITHREADING)

namespace GPlatform {

GpRunnable::GpRunnable (GpConditionVar::SP aCondVar) noexcept:
iCondVar(std::move(aCondVar))
{
}

GpRunnable::~GpRunnable (void) noexcept
{
}

}//GPlatform

#endif//#if defined(GP_USE_MULTITHREADING)