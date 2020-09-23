#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_UNIT_TESTS)

#include "GpUnitTestCtx.hpp"

namespace GPlatform {

class GPCORE_API GpUnitTestLayeredCtx: public GpUnitTestCtx
{
public:
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpUnitTestLayeredCtx)
    CLASS_DECLARE_DEFAULTS(GpUnitTestLayeredCtx)

public:
                        GpUnitTestLayeredCtx    (void) noexcept;
                        GpUnitTestLayeredCtx    (GpUnitTestCtx::SP aSublayerCtx) noexcept;
    virtual             ~GpUnitTestLayeredCtx   (void) noexcept override;

    virtual void        OnTestFailure           (std::string_view           aTestName,
                                                 const GpUnitTestException& aTestEx) override;
    virtual void        OnException             (std::string_view           aTestName,
                                                 std::exception&            aEx) override;
    virtual void        OnUnknownException      (std::string_view           aTestName) override;

private:
    GpUnitTestCtx::SP   iSublayerCtx;
};

}//GPlatform

#endif//#if defined(GP_USE_UNIT_TESTS)
