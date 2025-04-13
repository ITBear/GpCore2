#pragma once

#include <GpCore2/GpUtils/TypeTraits/GpTypeTraits.hpp>

namespace GPlatform {

template<typename T>
struct GpMethodAccessTypeShell
{
};

template<typename T>
class GpMethodAccess
{
public:
                GpMethodAccess  (void) = delete;
    explicit    GpMethodAccess  (const T*) {}
    explicit    GpMethodAccess  (GpMethodAccessTypeShell<T>) {}
};

template<typename... Ts>
class GpMethodAccessGuard
{
public:
    GpMethodAccessGuard (void) = delete;

    template<typename T>
    requires is_base_of_any_v<std::remove_cvref_t<T>, Ts...>
    GpMethodAccessGuard (GpMethodAccess<T>) {}
};

}// namespace GPlatform
