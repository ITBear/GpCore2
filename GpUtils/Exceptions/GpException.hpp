#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_EXCEPTIONS)

#include "../GpUtils_global.hpp"
#include "../Debugging/GpSourceLocation.hpp"
#include "../Types/Strings/GpStringLiterals.hpp"
#include "../Types/Strings/GpUTF.hpp"

#include <exception>
#include <functional>
#include <optional>

namespace GPlatform {

class GP_UTILS_API GpException: public std::exception
{
public:
    struct C {
        using Opt = std::optional<GpException>;
    };

public:
                            GpException     (void) noexcept = default;
    inline                  GpException     (const GpException& aException);
    inline                  GpException     (GpException&& aException);
                            GpException     (std::string_view       aMsg,
                                             const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;
    virtual                 ~GpException    (void) noexcept override;

    virtual const char*     what            (void) const noexcept override {return reinterpret_cast<const char*>(std::data(iWhat));}
    std::string_view        Message         (void) const noexcept {return iMsg;}
    const SourceLocationT&  SourceLocation  (void) const noexcept {return iSourceLocation;}

    inline GpException&     operator=       (const GpException& aException);
    inline GpException&     operator=       (GpException&&  aException);

private:
    std::string             iWhat;
    std::string_view        iMsg;
    SourceLocationT         iSourceLocation;
};

GpException::GpException (const GpException& aException):
iWhat          (aException.iWhat),
iMsg           (std::data(iWhat) + (std::data(aException.iMsg) - std::data(aException.iWhat)), aException.iMsg.length()),
iSourceLocation(aException.iSourceLocation)
{
}

GpException::GpException (GpException&& aException):
iWhat          (aException.iWhat),//do not std::move
iMsg           (std::data(iWhat) + (std::data(aException.iMsg) - std::data(aException.iWhat)), aException.iMsg.length()),
iSourceLocation(aException.iSourceLocation)//do not std::move
{
}

GpException&    GpException::operator= (const GpException&  aException)
{
    iWhat           = aException.iWhat;
    iMsg            = std::string_view(std::data(iWhat) + (std::data(aException.iMsg) - std::data(aException.iWhat)), aException.iMsg.length());
    iSourceLocation = aException.iSourceLocation;

    return *this;
}

GpException&    GpException::operator= (GpException&& aException)
{
    iWhat           = aException.iWhat;
    iMsg            = std::string_view(std::data(iWhat) + (std::data(aException.iMsg) - std::data(aException.iWhat)), aException.iMsg.length());
    iSourceLocation = aException.iSourceLocation;

    return *this;
}

[[noreturn]] inline void    THROW_GP
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    throw GpException(aMsg, aSourceLocation);
}

[[noreturn]] inline void    THROW_GP_NOT_IMPLEMENTED
(
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    throw GpException("Not implemented yet..."_sv, aSourceLocation);
}

inline void THROW_COND_GP
(
    const bool              aCondition,
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition) [[unlikely]]
    {
        throw GpException(aMsg, aSourceLocation);
    }
}

inline void THROW_COND_GP
(
    const bool                      aCondition,
    std::function<std::string()>&&  aMsgGenFn,
    const SourceLocationT&          aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition) [[unlikely]]
    {
        throw GpException(aMsgGenFn(), aSourceLocation);
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
