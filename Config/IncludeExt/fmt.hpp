#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroWarnings.hpp>

#if defined(GP_COMPILER_GCC) || defined(GP_COMPILER_CLANG)

#   if __has_include(<format>)
#       include <format>
#       define FMT_NAMESPASE std
        namespace fmt = std;
#       define GP_IMPLEMENT_FMT_JOIN
#   else
        GP_WARNING_PUSH()
        GP_WARNING_DISABLE(switch-default)
        GP_WARNING_DISABLE(switch-enum)
#       include <fmt/core.h>
#       include <fmt/format.h>
#       include <fmt/ostream.h>
#       include <fmt/ranges.h>
        GP_WARNING_POP()
#       define FMT_NAMESPASE fmt
#   endif// if __has_include(<format>)
#elif defined(GP_COMPILER_MSVC)
#   include <format>
#   define FMT_NAMESPASE std
    namespace fmt = std;
#   define GP_IMPLEMENT_FMT_JOIN
#endif

#if defined(GP_IMPLEMENT_FMT_JOIN)
#   include <GpCore2/GpUtils/Concepts/GpConcepts.hpp>
    namespace FMT_NAMESPASE {
        // -------------------- container elements join --------------------
        template <::GPlatform::Concepts::HasContiguousIter T>
        std::string join(const T& aContainer, std::string_view aDelimiter)
        {
            std::string result;
            bool first = true;
            for (const auto& elem: aContainer)
            {
                if (!first) [[likely]]
                {
                    result += aDelimiter;
                }

                result += ::fmt::format("{}", elem);
                first = false;
            }

            return result;
        }
    }// namespace FMT_NAMESPASE
#endif//
