#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_EXCEPTIONS)

#include "../GpUtils_global.hpp"
#include "../Types/Strings/GpStringLiterals.hpp"
#include "../Types/Strings/GpUTF.hpp"
#include <exception>
#include <functional>

#if  (__cplusplus >= CPP_VERSION_20) && __has_include(<source_location>) && __has_builtin(__builtin_source_location)
#   include <source_location>
    namespace GPlatform{
        using SourceLocationT = std::source_location;
    }
#elif  (__cplusplus >= CPP_VERSION_20) && __has_include(<experimental/source_location>)
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

class GP_UTILS_API GpException: public std::exception
{
public:
                            GpException     (void) noexcept = default;
    inline                  GpException     (const GpException& aException);
    inline                  GpException     (GpException&& aException);
                            GpException     (std::u8string_view     aMsg,
                                             const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;
    inline                  GpException     (std::string_view       aMsg,
                                             const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;
    virtual                 ~GpException    (void) noexcept override;

    virtual const char*     what            (void) const noexcept override {return reinterpret_cast<const char*>(iWhat.data());}
    std::u8string_view      Message         (void) const noexcept {return iMsg;}
    const SourceLocationT&  SourceLocation  (void) const noexcept {return iSourceLocation;}

    inline GpException&     operator=       (const GpException& aException);
    inline GpException&     operator=       (GpException&&  aException);

private:
    std::u8string           iWhat;
    std::u8string_view      iMsg;
    SourceLocationT         iSourceLocation;
};

GpException::GpException (const GpException& aException):
iWhat          (aException.iWhat),
iMsg           (iWhat.data() + (aException.iMsg.data() - aException.iWhat.data()), aException.iMsg.length()),
iSourceLocation(aException.iSourceLocation)
{
}

GpException::GpException (GpException&& aException):
iWhat          (aException.iWhat),//do not std::move
iMsg           (iWhat.data() + (aException.iMsg.data() - aException.iWhat.data()), aException.iMsg.length()),
iSourceLocation(aException.iSourceLocation)//do not std::move
{
}

GpException::GpException
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation
) noexcept:
GpException
(
    GpUTF::S_As_UTF8(aMsg),
    aSourceLocation
)
{
}

GpException&    GpException::operator= (const GpException&  aException)
{
    iWhat           = aException.iWhat;
    iMsg            = iWhat.data() + (aException.iMsg.data() - aException.iWhat.data()), aException.iMsg.length();
    iSourceLocation = aException.iSourceLocation;

    return *this;
}

GpException&    GpException::operator= (GpException&& aException)
{
    iWhat           = aException.iWhat;
    iMsg            = iWhat.data() + (aException.iMsg.data() - aException.iWhat.data()), aException.iMsg.length();
    iSourceLocation = aException.iSourceLocation;

    return *this;
}

[[noreturn]] inline void    THROW_GP
(
    std::u8string_view      aMsg,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    throw GpException(aMsg, aSourceLocation);
}

[[noreturn]] inline void    THROW_GP
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    throw GpException(GpUTF::S_As_UTF8(aMsg), aSourceLocation);
}

[[noreturn]] inline void    THROW_GP_NOT_IMPLEMENTED
(
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    throw GpException(u8"Not implemented yet..."_sv, aSourceLocation);
}

inline void THROW_COND_GP
(
    const bool              aCondition,
    std::u8string_view      aMsg,
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
    const bool              aCondition,
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition) [[unlikely]]
    {
        throw GpException(GpUTF::S_As_UTF8(aMsg), aSourceLocation);
    }
}

inline void THROW_COND_GP
(
    const bool                          aCondition,
    std::function<std::u8string()>&&    aMsgGenFn,
    const SourceLocationT&              aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition) [[unlikely]]
    {
        throw GpException(aMsgGenFn(), aSourceLocation);
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
        const auto msg = aMsgGenFn();
        throw GpException(GpUTF::S_As_UTF8(msg), aSourceLocation);
    }
}

}//GPlatform

#endif//#if defined(GP_USE_EXCEPTIONS)
