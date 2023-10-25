#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "GpException.hpp"
#include "../Types/Enums/GpEnum.hpp"

namespace GPlatform {

class GP_UTILS_API GpExceptionCode: public GpException
{
private:
                                GpExceptionCode     (void) noexcept = delete;

protected:
    inline explicit             GpExceptionCode     (const GpExceptionCode& aException);
    inline explicit             GpExceptionCode     (GpExceptionCode&& aException);
    inline explicit             GpExceptionCode     (std::u8string_view     aMsg,
                                                     const SourceLocationT& aSourceLocation) noexcept;

public:
    virtual                     ~GpExceptionCode    (void) noexcept override;

    GpEnum::value_type          CodeId              (void) const noexcept {return _CodeId();}
    std::u8string_view          CodeAsText          (void) const noexcept {return _CodeAsText();}

protected:
    virtual GpEnum::value_type  _CodeId             (void) const noexcept = 0;
    virtual std::u8string_view  _CodeAsText         (void) const noexcept = 0;
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
    std::u8string_view      aMsg,
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
                                             std::u8string_view     aMsg, \
                                             const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept; \
    virtual                     ~NAME       (void) noexcept override final; \
 \
    CodeTE                      Code        (void) const noexcept {return iCode;} \
 \
protected: \
    virtual GpEnum::value_type  _CodeId     (void) const noexcept override final; \
    virtual std::u8string_view  _CodeAsText (void) const noexcept override final; \
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
    std::u8string_view      aMsg, \
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
    std::u8string_view      aMsg, \
    const SourceLocationT&  aSourceLocation = SourceLocationT::current() \
) \
{ \
    throw NAME(aCode, aMsg, aSourceLocation); \
} \
 \
[[noreturn]] inline void THROW_##SHORT_NAME \
( \
    const NAME##Code::EnumT aCode, \
    std::string_view        aMsg, \
    const SourceLocationT&  aSourceLocation = SourceLocationT::current() \
) \
{ \
    throw NAME(aCode, ::GPlatform::GpUTF::S_STR_To_UTF8(aMsg), aSourceLocation); \
} \
 \
inline void THROW_COND_##SHORT_NAME \
( \
    const bool              aCondition, \
    const NAME##Code::EnumT aCode, \
    std::u8string_view      aMsg, \
    const SourceLocationT&  aSourceLocation = SourceLocationT::current() \
) \
{ \
    if (!aCondition) [[unlikely]] \
    { \
        throw NAME(aCode, aMsg, aSourceLocation); \
    } \
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
    if (!aCondition) [[unlikely]] \
    { \
        throw NAME(aCode, ::GPlatform::GpUTF::S_STR_To_UTF8(aMsg), aSourceLocation); \
    } \
} \
 \
inline void THROW_COND_##SHORT_NAME \
( \
    const bool                          aCondition, \
    const NAME##Code::EnumT             aCode, \
    std::function<std::u8string()>&&    aMsgGenFn, \
    const SourceLocationT&              aSourceLocation = SourceLocationT::current() \
) \
{ \
    if (!aCondition) [[unlikely]] \
    { \
        throw NAME(aCode, aMsgGenFn(), aSourceLocation); \
    } \
} \
 \
inline void THROW_COND_##SHORT_NAME \
( \
    const bool                      aCondition, \
    const NAME##Code::EnumT         aCode, \
    std::function<std::string()>&&  aMsgGenFn, \
    const SourceLocationT&          aSourceLocation = SourceLocationT::current() \
) \
{ \
    if (!aCondition) [[unlikely]] \
    { \
        const auto msg = aMsgGenFn(); \
        throw NAME(aCode, GpUTF::S_STR_To_UTF8(msg), aSourceLocation); \
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
        throw NAME(aCode, u8"", aSourceLocation); \
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
std::u8string_view  NAME::_CodeAsText (void) const noexcept \
{ \
    return CodeT::SToString(iCode); \
}

}//namespace GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
