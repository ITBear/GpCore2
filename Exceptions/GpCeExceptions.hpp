#pragma once

#include "../Constexpr/GpConstexprFalse.hpp"
#include <stdexcept>

namespace GPlatform {

constexpr auto GpThrowByCondition (bool aCondition) -> void
{
	aCondition ? throw std::runtime_error("") : 0;
}

template<typename Exception, typename... Args>
void GpThrowCe([[maybe_unused]] Args... aArgs)
{
	if (std::is_constant_evaluated())
	{
		GpThrowByCondition(true);
	} else
	{
		throw Exception(aArgs...);
	}
}

#define GP_TEMPLATE_THROW(T, MSG) static_assert(GpConstexprFalse<T>::value, MSG)

}//Gplatform