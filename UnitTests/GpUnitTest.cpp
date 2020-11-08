#include "GpUnitTest.hpp"

#if defined(GP_USE_UNIT_TESTS)

namespace GPlatform {

void    GpUnitTest::TEST (std::string_view      aTestName,
                          GpUnitTestCtx::SP     aCtx,
                          std::function<void()> aFn)

{
    try
    {
        aFn();
    } catch (const GpUnitTestException& aTestEx)
    {
        aCtx->OnTestFailure(aTestName, aTestEx);
    } catch (std::exception& aEx)
    {
        aCtx->OnException(aTestName, aEx);
    } catch (...)
    {
        aCtx->OnUnknownException(aTestName);
    }
}

}//GPlatform

#endif//#if defined(GP_USE_UNIT_TESTS)
