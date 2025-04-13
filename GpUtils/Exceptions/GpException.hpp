#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_EXCEPTIONS)

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/Config/IncludeExt/boost_small_vector.hpp>
#include <GpCore2/GpUtils/Debugging/GpSourceLocation.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringLiterals.hpp>

#include <exception>
#include <functional>

namespace GPlatform {

class GP_UTILS_API GpException: public std::exception
{
public:
    struct C {
        using Opt       = std::optional<GpException>;

        template <size_t N>
        using SmallVec  = boost::container::small_vector<GpException, N>;
    };

public:
                            GpException     (void) noexcept = default;
                            GpException     (const GpException& aException);
                            GpException     (GpException&& aException) noexcept;
                            GpException     (std::string_view       aMsg,
                                             const SourceLocationT& aSourceLocation = SourceLocationT::current()) noexcept;
    virtual                 ~GpException    (void) noexcept override;

    virtual const char*     what            (void) const noexcept override {return reinterpret_cast<const char*>(std::data(iWhat));}
    std::string_view        Message         (void) const noexcept {return iMsg;}
    const SourceLocationT&  SourceLocation  (void) const noexcept {return iSourceLocation;}

    GpException&            operator=       (const GpException& aException);
    GpException&            operator=       (GpException&&      aException) noexcept;

private:
    std::string             iWhat;
    std::string             iMsg;
    SourceLocationT         iSourceLocation;
};

[[noreturn]] inline void    THROW
(
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    throw GpException{aMsg, aSourceLocation};
}

[[noreturn]] inline void    THROW_NOT_IMPLEMENTED
(
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    throw GpException{"Not implemented yet..."_sv, aSourceLocation};
}

inline void VERIFY
(
    const bool              aCondition,
    std::string_view        aMsg,
    const SourceLocationT&  aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition) [[unlikely]]
    {
        throw GpException{aMsg, aSourceLocation};
    }
}

inline void VERIFY
(
    const bool                      aCondition,
    std::function<std::string()>&&  aMsgGenFn,
    const SourceLocationT&          aSourceLocation = SourceLocationT::current()
)
{
    if (!aCondition) [[unlikely]]
    {
        throw GpException{aMsgGenFn(), aSourceLocation};
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
