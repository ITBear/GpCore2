#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_EXCEPTIONS)

#include <stdexcept>
#include <type_traits>

namespace GPlatform {

template<bool ConditionV>
constexpr auto GpThrowByCondition (void) -> void
{
    ConditionV ? throw std::runtime_error("") : 0;
}

template<typename Exception, typename... Args>
constexpr void GpThrowCe([[maybe_unused]] Args... aArgs)
{   
    if (std::is_constant_evaluated())
    {
        GpThrowByCondition<true>();
    } else
    {
        throw Exception(aArgs...);
    }
}

}// namespace GPlatform

#endif// #if defined(GP_USE_EXCEPTIONS)
