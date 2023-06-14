#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "GpException.hpp"
#include "../Types/Enums/GpEnum.hpp"

namespace GPlatform {

class GP_UTILS_API GpExceptionTextCode: public GpException
{
private:
                                GpExceptionTextCode     (void) noexcept = delete;

public:
    inline explicit             GpExceptionTextCode     (const GpExceptionTextCode& aException);
    inline explicit             GpExceptionTextCode     (GpExceptionTextCode&& aException);
    inline explicit             GpExceptionTextCode     (std::u8string_view     aMsg,
                                                         std::u8string          aCode,
                                                         const SourceLocationT& aSourceLocation) noexcept;

public:
    virtual                     ~GpExceptionTextCode    (void) noexcept override;

    std::u8string_view          Code                    (void) const noexcept {return iCode;}

private:
    std::u8string               iCode;
};

GpExceptionTextCode::GpExceptionTextCode (const GpExceptionTextCode& aException):
GpException(aException),
iCode(aException.iCode)
{
}

GpExceptionTextCode::GpExceptionTextCode (GpExceptionTextCode&& aException):
GpException(std::move(aException)),
iCode(std::move(aException.iCode))
{
}

GpExceptionTextCode::GpExceptionTextCode
(
    std::u8string_view      aMsg,
    std::u8string           aCode,
    const SourceLocationT&  aSourceLocation
) noexcept:
GpException(aMsg, aSourceLocation),
iCode(std::move(aCode))
{
}

[[noreturn]] inline void    THROW_TEXT_CODE_GP
(
    std::u8string_view      aMsg,
    std::u8string           aCode,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    throw GpExceptionTextCode(aMsg, std::move(aCode), aSourceLocation);
}

inline void THROW_COND_TEXT_CODE_GP
(
    const bool              aCondition,
    std::u8string_view      aMsg,
    std::u8string           aCode,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition)
    {
        throw GpExceptionTextCode(aMsg, std::move(aCode), aSourceLocation);
    }
}

inline void THROW_COND_TEXT_CODE_GP
(
    const bool                      aCondition,
    std::function<std::u8string()>  aMsgGenFn,
    std::u8string                   aCode,
    const SourceLocationT&          aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition)
    {
        throw GpExceptionTextCode(aMsgGenFn(), std::move(aCode), aSourceLocation);
    }
}

}//namespace GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
