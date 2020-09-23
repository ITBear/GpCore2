#include "GpUnitTestStreamOutCtx.hpp"

#if defined(GP_USE_UNIT_TESTS)

#include <iostream>

namespace GPlatform {

GpUnitTestStreamOutCtx::GpUnitTestStreamOutCtx (void) noexcept
{
}

GpUnitTestStreamOutCtx::GpUnitTestStreamOutCtx (GpUnitTestCtx::SP aSublayerCtx) noexcept:
GpUnitTestLayeredCtx(aSublayerCtx)
{
}

GpUnitTestStreamOutCtx::~GpUnitTestStreamOutCtx (void) noexcept
{
}

void    GpUnitTestStreamOutCtx::OnTestFailure (std::string_view             aTestName,
                                               const GpUnitTestException&   aTestEx)
{
    const SourceLocationT& sourceLocation = aTestEx.SourceLocation();

    std::cout << "TEST '"_sv << aTestName << "' failed: " << aTestEx.Message()
              << " in file '"_sv << sourceLocation.file_name() << "'"_sv
              << ", function '"_sv << sourceLocation.function_name() << "'"_sv
              << ", line "_sv << sourceLocation.line()
              << ", column "_sv << sourceLocation.column()
              << "\n";

    std::cout.flush();

    GpUnitTestLayeredCtx::OnTestFailure(aTestName, aTestEx);
}

void    GpUnitTestStreamOutCtx::OnException (std::string_view   aTestName,
                                             std::exception&    aEx)
{
    std::cout << "TEST '"_sv << aTestName << "' failed, unexpected exception was caught:\n"_sv
              << aEx.what()
              << "\n"_sv;

    std::cout.flush();

    GpUnitTestLayeredCtx::OnException(aTestName, aEx);
}

void    GpUnitTestStreamOutCtx::OnUnknownException (std::string_view aTestName)
{
    std::cout << "TEST '"_sv << aTestName << "' failed, unknown exception was caught"_sv
              << "\n"_sv;

    std::cout.flush();

    GpUnitTestLayeredCtx::OnUnknownException(aTestName);
}

}//namespace GPlatform

#endif//#if defined(GP_USE_UNIT_TESTS)
