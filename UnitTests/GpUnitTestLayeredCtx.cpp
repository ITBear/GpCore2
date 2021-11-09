#include "GpUnitTestLayeredCtx.hpp"

#if defined(GP_USE_UNIT_TESTS)

namespace GPlatform {

GpUnitTestLayeredCtx::GpUnitTestLayeredCtx (void) noexcept
{
}

GpUnitTestLayeredCtx::GpUnitTestLayeredCtx (GpUnitTestCtx::SP aSublayerCtx) noexcept:
iSublayerCtx(std::move(aSublayerCtx))
{
}

GpUnitTestLayeredCtx::~GpUnitTestLayeredCtx (void) noexcept
{
}

void    GpUnitTestLayeredCtx::OnTestFailure
(
    std::string_view            aTestName,
    const GpUnitTestException&  aTestEx
)
{
    if (iSublayerCtx.IsNotNULL())
    {
        iSublayerCtx.Vn().OnTestFailure(aTestName, aTestEx);
    }
}

void    GpUnitTestLayeredCtx::OnException
(
    std::string_view    aTestName,
    std::exception&     aEx
)
{
    if (iSublayerCtx.IsNotNULL())
    {
        iSublayerCtx.Vn().OnException(aTestName, aEx);
    }
}

void    GpUnitTestLayeredCtx::OnUnknownException (std::string_view aTestName)
{
    if (iSublayerCtx.IsNotNULL())
    {
        iSublayerCtx.Vn().OnUnknownException(aTestName);
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_UNIT_TESTS)
