#pragma once

#include "../../Config/GpCompilerFeatures.hpp"

#if defined(GP_COMPILER_GCC) || defined(GP_COMPILER_CLANG)
#   if (__cplusplus >= CPP_VERSION_20) && __has_include(<source_location>) && __has_builtin(__builtin_source_location)
#      include <source_location>
        namespace GPlatform {
            using SourceLocationT = std::source_location;
        }
#   elif  (__cplusplus >= CPP_VERSION_20) && __has_include(<experimental/source_location>)
#      include <experimental/source_location>
        namespace GPlatform {
            using SourceLocationT = std::experimental::source_location;
        }
#   else
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

        namespace GPlatform {
            using SourceLocationT = source_location;
        }
#   endif
#else
#   include <source_location>

    namespace GPlatform {
        using SourceLocationT = std::source_location;
    }
#endif

//----------------------------------------------------------------------------------
#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpUTF.hpp>
#include <optional>

#include <GpCore2/Config/IncludeExt/fmt.hpp>

namespace fmt {

using namespace GPlatform;

template<>
struct formatter<SourceLocationT>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& aCtx)
    {
        return std::begin(aCtx);
    }

    template<typename FormatContext>
    auto format(const SourceLocationT& aLocation, FormatContext& aCtx) const
    {
        return ::fmt::format_to
        (
            aCtx.out(),
            "Function: '{}' in '{}:{}'",
            aLocation.function_name(),
            aLocation.file_name(),
            aLocation.line()
        );
    }
};

template<>
struct formatter<std::optional<SourceLocationT>>
{
    template<typename ParseContext>
    constexpr auto parse(ParseContext& aCtx)
    {
        return std::begin(aCtx);
    }

    template<typename FormatContext>
    auto format(const std::optional<SourceLocationT>& aLocation, FormatContext& aCtx) const
    {
        if (aLocation.has_value()) [[likely]]
        {
            return ::fmt::formatter<SourceLocationT>().format(aLocation.value(), aCtx);
        } else
        {
            return ::fmt::format_to(aCtx.out(), "");
        }
    }
};

}// namespace std
