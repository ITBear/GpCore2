#pragma once

#include <string_view>
#include <array>

namespace GPlatform {

//GP_WARNING_PUSH()
//GP_WARNING_DISABLE(gnu-string-literal-operator-template)
//
//template <char... chars> using GpStringTemplateArg = std::integer_sequence<char, chars...>;
//template <typename T, T... chars> constexpr GpStringTemplateArg<chars...> operator"" _template_str() noexcept {return { };}
//
//GP_WARNING_POP()
//
//template <typename> struct GpStringTemplateArgHolder;
//
//template <char... chars>
//struct GpStringTemplateArgHolder<GpStringTemplateArg<chars...>>
//{
//  static constexpr std::string_view SAsStringView (void) noexcept
//  {
//      constexpr std::array<char, sizeof...(chars) + 1> str = {chars..., '\0'};
//      return std::string_view(std::data(str), std::size(str) - 1);
//  }
//};

// TODO: reimplement with constexpr std::string_view

template<char... chars>
struct GpStringTemplateArg
{
    static constexpr char sString[] = {chars..., '\0'};

    static constexpr std::string_view SAsStringView (void) noexcept
    {
        return std::string_view(sString, sizeof...(chars));
    }
};

}// namespace GPlatform
