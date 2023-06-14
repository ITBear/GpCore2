#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include <optional>

#include "../Macro/GpMacroClass.hpp"
#include "GpException.hpp"
#include "../Types/Strings/GpUTF.hpp"

namespace GPlatform{

class GP_UTILS_API GpExceptionUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpExceptionUtils)

public:
    class ToStrResT
    {
    public:
        std::u8string       fullMessage;
        std::u8string_view  message;//view of part of fullMessage
    };

    enum class ExceptionType
    {
        STD,
        GP
    };

public:
    static ToStrResT            SToString   (const std::u8string_view       aMessage,
                                             const SourceLocationT&         aSourceLocation,
                                             const ExceptionType            aExceptionType,
                                             std::optional<std::u8string>   aStackTrace);
    static inline std::u8string SToString   (const std::exception&  aException,
                                             const SourceLocationT& aSourceLocation = SourceLocationT::current());
    static inline std::u8string SToString   (const GpException&     aException);
};

std::u8string   GpExceptionUtils::SToString
(
    const std::exception&   aException,
    const SourceLocationT&  aSourceLocation
)
{
    return SToString
    (
        GpUTF::S_STR_To_UTF8(aException.what()),
        aSourceLocation,
        ExceptionType::STD,
        std::nullopt
    ).fullMessage;
}

std::u8string   GpExceptionUtils::SToString (const GpException& aException)
{
    return std::u8string(GpUTF::S_STR_To_UTF8(aException.what()));
}

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
