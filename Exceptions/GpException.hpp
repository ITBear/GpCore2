#pragma once

#include "../GpCore_global.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "../Types/Strings/GpStringLiterals.hpp"

#include <exception>
#include <string_view>
#include <string>
#include <functional>

#if  __has_include(<source_location>)
#   include <source_location>
    namespace GPlatform{
        using SourceLocationT = std::source_location;
    }
#elif  __has_include(<experimental/source_location>)
#   include <experimental/source_location>
    namespace GPlatform{
        using SourceLocationT = std::experimental::source_location;
    }
#else
    struct source_location
    {
        static constexpr source_location    current         (const char*    __file = __builtin_FILE(),
                                                             const char*    __func = __builtin_FUNCTION(),
                                                             const int      __line = __builtin_LINE(),
                                                             const int      __col   = 0) noexcept
        {
            source_location __loc;

            __loc._M_file   = __file;
            __loc._M_func   = __func;
            __loc._M_line   = uint_least32_t(__line);
            __loc._M_col    = uint_least32_t(__col);

            return __loc;
        }

        constexpr                           source_location (void) noexcept:
                                                _M_file("unknown"),
                                                _M_func(_M_file),
                                                _M_line(0),
                                                _M_col(0)
        {}

        constexpr uint_least32_t            line            (void) const noexcept { return _M_line; }
        constexpr uint_least32_t            column          (void) const noexcept { return _M_col; }
        constexpr const char*               file_name       (void) const noexcept { return _M_file; }
        constexpr const char*               function_name   (void) const noexcept { return _M_func; }

    private:
        const char*     _M_file;
        const char*     _M_func;
        uint_least32_t  _M_line;
        uint_least32_t  _M_col;
    };

    namespace GPlatform{
        using SourceLocationT = source_location;
    }
#endif

namespace GPlatform{

class GPCORE_API GpException: public std::exception
{
private:
                            GpException     (void) noexcept = delete;
                            GpException     (const GpException& aException) noexcept = delete;

    GpException&            operator=       (const GpException& aException) noexcept = delete;
    GpException&            operator=       (GpException&& aException) noexcept = delete;

public:
                            GpException     (GpException&& aException) noexcept;
                            GpException     (std::string_view       aMsg,
                                             const SourceLocationT& aSourceLocation) noexcept;

    virtual                 ~GpException    (void) noexcept override;

    virtual const char*     what            (void) const noexcept override {return iWhat.data();}
    std::string_view        Message         (void) const noexcept {return iMsg;}
    const SourceLocationT&  SourceLocation  (void) const noexcept {return iSourceLocation;}

private:
    std::string             iWhat;
    std::string_view        iMsg;
    SourceLocationT         iSourceLocation;
};

using ThrowMsgGenT = std::function<std::string()>;

[[noreturn]] inline void    THROW_GPE
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    throw GpException(aMsg, aSourceLocation);
}

[[noreturn]] inline void    THROW_GPE_NOT_IMPLEMENTED
(
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    throw GpException("Not implemented yet..."_sv, aSourceLocation);
}

inline void THROW_GPE_COND
(
    const bool              aCondition,
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition)
    {
        throw GpException(aMsg, aSourceLocation);
    }
}

inline void THROW_GPE_COND
(
    const bool              aCondition,
    ThrowMsgGenT            aMsgGenFn,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition)
    {
        throw GpException(aMsgGenFn(), aSourceLocation);
    }
}

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)


