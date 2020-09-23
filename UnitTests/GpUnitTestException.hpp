#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_UNIT_TESTS)

#include "../Exceptions/GpException.hpp"

namespace GPlatform {

class GPCORE_API GpUnitTestException final: public GpException
{   
                            GpUnitTestException (void) noexcept = delete;
                            GpUnitTestException (const GpException& aException) noexcept = delete;

    GpUnitTestException&    operator=           (const GpUnitTestException& aException) noexcept = delete;
    GpUnitTestException&    operator=           (GpUnitTestException&& aException) noexcept = delete;

public:
                            GpUnitTestException (GpUnitTestException&& aException) noexcept;
                            GpUnitTestException (std::string_view       aMsg,
                                                 const SourceLocationT& aLocation) noexcept;

    virtual                 ~GpUnitTestException(void) noexcept override final;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_UNIT_TESTS)
