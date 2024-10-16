#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_EXCEPTIONS)

#include <GpCore2/GpUtils/Exceptions/GpException.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>

namespace GPlatform {

class GP_UTILS_API GpExceptionTextCode: public GpException
{
private:
                            GpExceptionTextCode     (void) noexcept = delete;

public:
    inline explicit         GpExceptionTextCode     (const GpExceptionTextCode& aException);
    inline explicit         GpExceptionTextCode     (GpExceptionTextCode&& aException);
    inline explicit         GpExceptionTextCode     (std::string_view       aMsg,
                                                     std::string            aCode,
                                                     const SourceLocationT& aSourceLocation) noexcept;

public:
    virtual                 ~GpExceptionTextCode    (void) noexcept override;

    std::string_view        Code                    (void) const noexcept {return iCode;}

private:
    std::string             iCode;
};

GpExceptionTextCode::GpExceptionTextCode (const GpExceptionTextCode& aException):
GpException{aException},
iCode      {aException.iCode}
{
}

GpExceptionTextCode::GpExceptionTextCode (GpExceptionTextCode&& aException):
GpException{std::move(aException)},
iCode      {std::move(aException.iCode)}
{
}

GpExceptionTextCode::GpExceptionTextCode
(
    std::string_view        aMsg,
    std::string             aCode,
    const SourceLocationT&  aSourceLocation
) noexcept:
GpException{aMsg, aSourceLocation},
iCode      {std::move(aCode)}
{
}

[[noreturn]] inline void    THROW_TEXT_CODE_GP
(
    std::string_view        aMsg,
    std::string             aCode,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    throw GpExceptionTextCode(aMsg, std::move(aCode), aSourceLocation);
}

inline void THROW_COND_TEXT_CODE_GP
(
    const bool              aCondition,
    std::string_view        aMsg,
    std::string             aCode,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition) [[unlikely]]
    {
        throw GpExceptionTextCode(aMsg, std::move(aCode), aSourceLocation);
    }
}

inline void THROW_COND_TEXT_CODE_GP
(
    const bool                      aCondition,
    std::function<std::string()>&&  aMsgGenFn,
    std::string                     aCode,
    const SourceLocationT&          aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition) [[unlikely]]
    {
        throw GpExceptionTextCode(aMsgGenFn(), std::move(aCode), aSourceLocation);
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
