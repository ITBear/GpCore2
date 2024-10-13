#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_EXCEPTIONS)

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

#include <optional>

namespace GPlatform {

class GP_UTILS_API GpExceptionUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpExceptionUtils)

public:
    class ToStrResT
    {
    public:
        std::string     fullMessage;
        std::string     message;
    };

public:
    static ToStrResT    SToString   (const std::string_view             aMessage,
                                     const SourceLocationT&             aSourceLocation,
                                     const std::optional<std::string>&  aStackTrace);
    static ToStrResT    SToString   (const GpException& aExeption);
};

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
