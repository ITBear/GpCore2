#pragma once

#include <type_traits>

namespace GPlatform {

template<typename T> struct GpConstexprFalse : std::false_type {};

}//GPlatform