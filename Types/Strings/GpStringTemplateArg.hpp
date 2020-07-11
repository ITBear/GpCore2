#pragma once

#include "../../Config/GpCompilerFeatures.hpp"

#include <utility>
#include <string_view>
#include <array>

namespace GPlatform {

template <char... chars> using GpStringTemplateArg = std::integer_sequence<char, chars...>;
template <typename T, T... chars> constexpr GpStringTemplateArg<chars...> operator"" _template_str() noexcept {return { };}

template <typename> struct GpStringTemplateArgHolder;

template <char... chars>
struct GpStringTemplateArgHolder<GpStringTemplateArg<chars...>>
{
    static std::string_view SAsStringView (void) noexcept
    {
        static constexpr char str[sizeof...(chars) + 1] = {chars..., '\0'};
        return std::string_view(str, sizeof...(chars));
    }
};

}//GPlatform

