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

    std::cout << "[GpExceptionsSink::SSink]: std::exception"_sv
              << ", what '" << aException.what() << "'"
              << ", sinked in file '" << l.file_name() << "'"
              << ", line " << l.line()
              << ", function " << l.function_name()
              << std::endl;
}

void    GpExceptionsSink::SSink
(
    std::string_view        aMsg,
    const SourceLocationT&  aLocation
) noexcept
{
    SourceLocationT l = aLocation;

    std::cout << "[GpExceptionsSink::SSink]: "_sv
              << aMsg
              << ", sinked in file '" << l.file_name() << "'"
              << ", line " << l.line()
              << ", function " << l.function_name()
              << std::endl;
}

void    GpExceptionsSink::SSinkUnknown (const SourceLocationT& aLocation) noexcept
{
    SSink("Unknown exception"_sv, aLocation);
}

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
