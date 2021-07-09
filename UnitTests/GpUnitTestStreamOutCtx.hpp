#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_UNIT_TESTS)

#include "GpUnitTestLayeredCtx.hpp"

namespace GPlatform {

class GPCORE_API GpUnitTestStreamOutCtx final: public GpUnitTestLayeredCtx
{
public:
    CLASS_REMOVE_CTRS_MOVE_COPY(GpUnitTestStreamOutCtx)
    CLASS_DECLARE_DEFAULTS(GpUnitTestStreamOutCtx)

public:
                        GpUnitTestStreamOutCtx  (void) noexcept;
                        GpUnitTestStreamOutCtx  (GpUnitTestCtx::SP aSublayerCtx) noexcept;
    virtual             ~GpUnitTestStreamOutCtx (void) noexcept override final;

    virtual void        OnTestFailure           (std::string_view           aTestName,
                                                 const GpUnitTestException& aTestEx) override final;
    virtual void        OnException             (std::string_view           aTestName,
                                                 std::exception&            aEx) override final;
    virtual void        OnUnknownException      (std::string_view           aTestName) override final;
};

}//namespace GPlatform

#endif//#if defined(GP_USE_UNIT_TESTS)
