#pragma once

#include "../GpUtils_global.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "../GpMacro.hpp"
#include "GpException.hpp"

namespace GPlatform{

class GP_UTILS_API GpExceptionUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpExceptionUtils)

public:
    class ToStrResT
    {
    public:
        std::string         fullMessage;
        std::string_view    message;//view of part of fullMessage
    };

    enum class ExceptionType
    {
        STD,
        GP
    };

public:
    static ToStrResT            SToString   (const std::string_view aMessage,
                                             const SourceLocationT& aSourceLocation,
                                             const ExceptionType    aExceptionType);
    static inline std::string   SToString   (const std::exception&  aException,
                                             const SourceLocationT& aSourceLocation = SourceLocationT::current());
    static inline std::string   SToString   (const GpException&     aException);
};

std::string GpExceptionUtils::SToString
(
    const std::exception&   aException,
    const SourceLocationT&  aSourceLocation
)
{
    return SToString
    (
        std::string_view(aException.what()),
        aSourceLocation,
        ExceptionType::STD
    ).fullMessage;
}

std::string GpExceptionUtils::SToString (const GpException& aException)
{
    return std::string(aException.what());
}

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
