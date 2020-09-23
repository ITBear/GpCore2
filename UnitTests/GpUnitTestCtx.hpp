#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_UNIT_TESTS)

#include "../Types/Classes/GpClassesDefines.hpp"
#include "../Types/Containers/GpContainersT.hpp"
#include "GpUnitTestException.hpp"

namespace GPlatform {

class GPCORE_API GpUnitTestCtx
{
public:
    CLASS_DECLARE_DEFAULTS(GpUnitTestCtx)

public:
                        GpUnitTestCtx       (void) noexcept;
    virtual             ~GpUnitTestCtx      (void) noexcept;

    virtual void        OnTestFailure       (std::string_view           aTestName,
                                             const GpUnitTestException& aTestEx) = 0;
    virtual void        OnException         (std::string_view           aTestName,
                                             std::exception&            aEx) = 0;
    virtual void        OnUnknownException  (std::string_view           aTestName) = 0;
};

}//GPlatform

#endif//#if defined(GP_USE_UNIT_TESTS)
