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
        aCtx.V().OnTestFailure(aTestName, aTestEx);
    } catch (std::exception& aEx)
    {
        aCtx.V().OnException(aTestName, aEx);
    } catch (...)
    {
        aCtx.V().OnUnknownException(aTestName);
    }
}

}//GPlatform

#endif//#if defined(GP_USE_UNIT_TESTS)
