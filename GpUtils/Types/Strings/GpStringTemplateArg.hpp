#pragma once

#include "../../GpMacro.hpp"

#include <utility>
#include <string_view>
#include <array>

namespace GPlatform {

GP_WARNING_PUSH()
GP_WARNING_DISABLE(gnu-string-literal-operator-template)

template <char... chars> using GpStringTemplateArg = std::integer_sequence<char, chars...>;
template <typename T, T... chars> constexpr GpStringTemplateArg<chars...> operator"" _template_str() noexcept {return { };}

GP_WARNING_POP()

template <typename> struct GpStringTemplateArgHolder;

template <char... chars>
struct GpStringTemplateArgHolder<GpStringTemplateArg<chars...>>
{
    static constexpr std::string_view SAsStringView (void) noexcept
    {
        constexpr std::array<char, sizeof...(chars) + 1> str = {chars..., '\0'};
        return std::string_view(str.data(), str.size() - 1);
    }
};

}//GPlatform

