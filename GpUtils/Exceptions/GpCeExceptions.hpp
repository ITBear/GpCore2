#pragma once

#include "../TypeTraits/GpTypeTraitsFalse.hpp"
#include <stdexcept>
#include <type_traits>

namespace GPlatform {

template<bool ConditionV>
constexpr auto GpThrowByCondition (void) -> void
{
    ConditionV ? throw std::runtime_error("") : 0;
}

template<typename Exception, typename... Args>
void GpThrowCe([[maybe_unused]] Args... aArgs)
{
#if (__cplusplus >= 202002L)
    if (std::is_constant_evaluated())
    {
        GpThrowByCondition<true>();
    } else
    {
        throw Exception(aArgs...);
    }
#else
    GpThrowByCondition<true>();
#endif
}

#define GP_TEMPLATE_THROW(T, MSG) static_assert(GpConstexprFalse<T>::value, MSG)

}//Gplatform
