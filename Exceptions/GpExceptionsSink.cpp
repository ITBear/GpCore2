#include "GpExceptionsSink.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include <iostream>
#include <mutex>

namespace GPlatform{

std::shared_mutex       GpExceptionsSink::sExceptionsSinkMutex;
GpExceptionsSink        GpExceptionsSink::sDefaultExceptionsSink;
GpExceptionsSink::RefT  GpExceptionsSink::sExceptionsSink = GpExceptionsSink::sDefaultExceptionsSink;

void    GpExceptionsSink::OnSink
(
    std::string_view        aMsg,
    const SourceLocationT&  aLocation
) noexcept
{
    SourceLocationT l = aLocation;

    std::cout << "[GpExceptionsSink::SSink]:"_sv
              << " what '" << aMsg << "'"
              << ", sinked in file '" << l.file_name() << "'"
              << ", line " << l.line()
              << ", function " << l.function_name()
              << std::endl;
}

void    GpExceptionsSink::SSetGlobalSink (GpExceptionsSink& sSink) noexcept
{
    std::scoped_lock lock(sExceptionsSinkMutex);
    sExceptionsSink = sSink;
}

void    GpExceptionsSink::SSink
(
    const std::exception&   aException,
    const SourceLocationT&  aLocation
) noexcept
{
    SSink(aException.what(), aLocation);
}

void    GpExceptionsSink::SSink
(
    std::string_view        aMsg,
    const SourceLocationT&  aLocation
) noexcept
{
    std::shared_lock lock(sExceptionsSinkMutex);
    sDefaultExceptionsSink.OnSink(aMsg, aLocation);
}

void    GpExceptionsSink::SSinkUnknown (const SourceLocationT& aLocation) noexcept
{
    SSink("Unknown exception"_sv, aLocation);
}

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
