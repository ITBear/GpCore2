#pragma once

#include "../../Macro/GpMacroWarnings.hpp"

#include <string_view>
#include <array>

namespace GPlatform {

GP_WARNING_PUSH()
GP_WARNING_DISABLE(gnu-string-literal-operator-template)

template <char8_t... chars> using GpStringTemplateArg = std::integer_sequence<char8_t, chars...>;
template <typename T, T... chars> constexpr GpStringTemplateArg<chars...> operator"" _template_str() noexcept {return { };}

GP_WARNING_POP()

template <typename> struct GpStringTemplateArgHolder;

template <char8_t... chars>
struct GpStringTemplateArgHolder<GpStringTemplateArg<chars...>>
{
    static constexpr std::u8string_view SAsStringView (void) noexcept
    {
        constexpr std::array<char8_t, sizeof...(chars) + 1> str = {chars..., '\0'};
        return std::u8string_view(str.data(), str.size() - 1);
    }
};

}//GPlatform
