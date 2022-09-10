#pragma once

#include "GpException.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "../Types/Enums/GpEnum.hpp"

namespace GPlatform {

class GP_UTILS_API GpExceptionCode: public GpException
{
private:
                                GpExceptionCode     (void) noexcept = delete;

protected:
    inline explicit             GpExceptionCode     (const GpExceptionCode& aException);
    inline explicit             GpExceptionCode     (GpExceptionCode&& aException);
    inline explicit             GpExceptionCode     (std::string_view       aMsg,
                                                     const SourceLocationT& aSourceLocation) noexcept;

public:
    virtual                     ~GpExceptionCode    (void) noexcept override = default;

    GpEnum::value_type          CodeId              (void) const noexcept {return _CodeId();}
    std::string_view            CodeAsText          (void) const noexcept {return _CodeAsText();}

protected:
    virtual GpEnum::value_type  _CodeId             (void) const noexcept = 0;
    virtual std::string_view    _CodeAsText         (void) const noexcept = 0;
};

GpExceptionCode::GpExceptionCode (const GpExceptionCode& aException):
GpException(aException)
{
}

GpExceptionCode::GpExceptionCode (GpExceptionCode&& aException):
GpException(std::move(aException))
{
}

GpExceptionCode::GpExceptionCode
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept:
GpException(aMsg, aSourceLocation)
{
}

#define EXCEPTION_CODE(PREFIX, NAME, SHORT_NAME) \
class PREFIX NAME final: public GpExceptionCode \
{ \
public: \
    using CodeT     = NAME##Code; \
    using CodeTE    = CodeT::EnumT; \
 \
private: \
                                NAME            (void) noexcept = delete; \
 \
public: \
    inline                      NAME        (const NAME&    aException); \
    inline                      NAME        (NAME&&         aException); \
    inline                      NAME        (CodeTE                 aCode, \
                                             std::string_view       aMsg, \
                                             const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept; \
    virtual                     ~NAME       (void) noexcept override final; \
 \
    CodeTE                      Code        (void) const noexcept {return iCode;} \
 \
protected: \
    virtual GpEnum::value_type  _CodeId     (void) const noexcept override final; \
    virtual std::string_view    _CodeAsText (void) const noexcept override final; \
 \
private: \
    const CodeTE                iCode; \
}; \
 \
NAME::NAME (const NAME& aException): \
GpExceptionCode(aException), \
iCode(aException.iCode) \
{ \
} \
 \
NAME::NAME (NAME&& aException): \
GpExceptionCode(std::move(aException)), \
iCode(std::move(aException.iCode)) \
{ \
} \
 \
NAME::NAME \
( \
    CodeTE                  aCode, \
    std::string_view        aMsg, \
    const SourceLocationT&  aSourceLocation \
) noexcept: \
GpExceptionCode(aMsg, aSourceLocation), \
iCode(aCode) \
{ \
} \
 \
[[noreturn]] inline void THROW_##SHORT_NAME \
( \
    const NAME##Code::EnumT aCode, \
    std::string_view        aMsg, \
    const SourceLocationT&  aSourceLocation = SourceLocationT::current() \
) \
{ \
    throw NAME(aCode, aMsg, aSourceLocation); \
} \
 \
inline void THROW_COND_##SHORT_NAME \
( \
    const bool              aCondition, \
    const NAME##Code::EnumT aCode, \
    std::string_view        aMsg, \
    const SourceLocationT&  aSourceLocation = SourceLocationT::current() \
) \
{ \
    if (!aCondition) \
    { \
        throw NAME(aCode, aMsg, aSourceLocation); \
    } \
} \
 \
inline void THROW_COND_##SHORT_NAME \
( \
    const bool              aCondition, \
    const NAME##Code::EnumT aCode, \
    ThrowMsgGenT            aMsgGenFn, \
    const SourceLocationT&  aSourceLocation = SourceLocationT::current() \
) \
{ \
    if (!aCondition) \
    { \
        throw NAME(aCode, aMsgGenFn(), aSourceLocation); \
    } \
} \
 \
inline void THROW_COND_##SHORT_NAME \
( \
    const bool              aCondition, \
    const NAME##Code::EnumT aCode, \
    const SourceLocationT&  aSourceLocation = SourceLocationT::current() \
) \
{ \
    if (!aCondition) \
    { \
        throw NAME(aCode, "", aSourceLocation); \
    } \
}


#define EXCEPTION_CODE_IMPL(NAME) \
 \
NAME::~NAME (void) noexcept \
{ \
} \
 \
GpEnum::value_type  NAME::_CodeId (void) const noexcept \
{ \
    return GpEnum::value_type(iCode); \
} \
 \
std::string_view    NAME::_CodeAsText (void) const noexcept \
{ \
    return CodeT::SToString(iCode); \
}

}//namespace GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
