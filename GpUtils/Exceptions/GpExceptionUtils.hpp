#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_EXCEPTIONS)

#include <optional>

#include "../Macro/GpMacroClass.hpp"
#include "GpException.hpp"
#include "../Types/Strings/GpUTF.hpp"

namespace GPlatform {

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
    static ToStrResT            SToString   (const std::string_view     aMessage,
                                             const SourceLocationT&     aSourceLocation,
                                             const ExceptionType        aExceptionType,
                                             std::optional<std::string> aStackTrace);
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
        aException.what(),
        aSourceLocation,
        ExceptionType::STD,
        std::nullopt
    ).fullMessage;
}

std::string GpExceptionUtils::SToString (const GpException& aException)
{
    return aException.what();
}

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
