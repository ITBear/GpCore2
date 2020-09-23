#include "GpUnitTestException.hpp"

#if defined(GP_USE_UNIT_TESTS)

namespace GPlatform {

GpUnitTestException::GpUnitTestException (GpUnitTestException&& aException) noexcept:
GpException(std::move(aException))
{
}

GpUnitTestException::GpUnitTestException (std::string_view          aMsg,
                                          const SourceLocationT&    aLocation) noexcept:
GpException(aMsg, aLocation)
{
}

GpUnitTestException::~GpUnitTestException(void) noexcept
{
}

}//GPlatform

#endif//#if defined(GP_USE_UNIT_TESTS)
