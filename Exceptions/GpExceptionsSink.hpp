#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "GpException.hpp"

namespace GPlatform{

class GPCORE_API GpExceptionsSink
{
private:
                    GpExceptionsSink    (void) = delete;
                    GpExceptionsSink    (const GpExceptionsSink&) = delete;

public:
    static void     SSink               (const std::exception&  aException,
                                         const SourceLocationT& aLocation = SourceLocationT::current()) noexcept;
    static void     SSink               (std::string_view aMsg,
                                         const SourceLocationT& aLocation = SourceLocationT::current()) noexcept;
    static void     SSinkUnknown        (const SourceLocationT& aLocation = SourceLocationT::current()) noexcept;
};

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
