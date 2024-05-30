#pragma once

#include <type_traits>
#include <utility>

namespace GPlatform {

template<typename T>
class GpMethodAccess
{
public:
    explicit    GpMethodAccess  (const T*) {}
                GpMethodAccess  (GpMethodAccess&&) {}

private:
                GpMethodAccess  (void) {}
};

template<typename... Ts>
class GpMethodAccessGuard;

template<>
class GpMethodAccessGuard<>
{
};

template<typename First, typename... Rest>
class GpMethodAccessGuard<First, Rest...>: public GpMethodAccessGuard<Rest...>
{
public:
    using GpMethodAccessGuard<Rest...>::GpMethodAccessGuard;

    GpMethodAccessGuard (const GpMethodAccessGuard<First, Rest...>&) {}
    GpMethodAccessGuard (GpMethodAccessGuard<First, Rest...>&&) {}
    GpMethodAccessGuard (GpMethodAccess<First>) {}

protected:
    GpMethodAccessGuard (void) = default;
};

}// namespace GPlatform
