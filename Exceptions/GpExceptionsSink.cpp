#include "GpExceptionsSink.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include <iostream>

namespace GPlatform{

void    GpExceptionsSink::SSink
(
    const std::exception&   aException,
    const SourceLocationT&  aLocation
) noexcept
{
    SourceLocationT l = aLocation;

    std::cout << "[GpExceptionsSink::SSink]: std::exception"
              << ", what '" << aException.what() << "'"
              << ", sinked in file '" << l.file_name() << "'"
              << ", line " << l.line()
              << ", function " << l.function_name()
              << std::endl;
}

void    GpExceptionsSink::SSinkUnknown (const SourceLocationT& aLocation) noexcept
{
    SourceLocationT l = aLocation;

    std::cout << "[GpExceptionsSink::SSink]: unknown exception"
              << ", sinked in file '" << l.file_name() << "'"
              << ", line " << l.line()
              << ", function " << l.function_name()
              << std::endl;
}

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
