#pragma once

#include <utility>
#include <functional>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>

#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>

namespace GPlatform {

TAG_REGISTER(GpTypeShell)

namespace Concepts::TypeShell {

template <typename T>
concept IsTypeShell = requires()
{
    requires GpHasTag_GpTypeShell<T>();
};

}// namespace Concepts::TypeShell

template<typename T, typename SHELL>
class GpTypeShell
{
public:
    using value_type    = T;
    using shell_type    = SHELL;
    using this_type     = GpTypeShell<T, SHELL>;

    TAG_SET(GpTypeShell)

public:
    constexpr               GpTypeShell     (void) noexcept;
    constexpr               GpTypeShell     (const GpTypeShell& aShell);
    constexpr               GpTypeShell     (GpTypeShell&& aShell) noexcept;
    constexpr               GpTypeShell     (const T& aValue);
    constexpr               GpTypeShell     (T&& aValue) noexcept;
    constexpr               ~GpTypeShell    (void) noexcept = default;

    constexpr T*            operator ->     (void) noexcept {return &iValue;}
    constexpr const T*      operator ->     (void) const noexcept {return &iValue;}

    constexpr T&            Value           (void) noexcept {return iValue;}
    constexpr const T&      Value           (void) const noexcept {return iValue;}

    constexpr T&&           ValueMove       (void) noexcept {return std::move(iValue);}

    constexpr this_type&    operator=       (const this_type& aShell);
    constexpr this_type&    operator=       (this_type&& aShell) noexcept;

    constexpr bool          operator==      (const this_type& aShell) const noexcept;
    constexpr bool          operator!=      (const this_type& aShell) const noexcept;
    constexpr bool          operator<       (const this_type& aShell) const noexcept;
    constexpr bool          operator<=      (const this_type& aShell) const noexcept;
    constexpr bool          operator>       (const this_type& aShell) const noexcept;
    constexpr bool          operator>=      (const this_type& aShell) const noexcept;

private:
    T                       iValue  = {};
};

template<typename T, typename SHELL>
constexpr GpTypeShell<T, SHELL>::GpTypeShell (void) noexcept
{
}

template<typename T, typename SHELL>
constexpr GpTypeShell<T, SHELL>::GpTypeShell (const GpTypeShell& aShell):
iValue{aShell.iValue}
{
}

template<typename T, typename SHELL>
constexpr GpTypeShell<T, SHELL>::GpTypeShell (GpTypeShell&& aShell) noexcept:
iValue{std::move(aShell.iValue)}
{
}

template<typename T, typename SHELL>
constexpr GpTypeShell<T, SHELL>::GpTypeShell (const T& aValue):
iValue{aValue}
{
}

template<typename T, typename SHELL>
constexpr GpTypeShell<T, SHELL>::GpTypeShell (T&& aValue) noexcept:
iValue{std::move(aValue)}
{
}

template<typename T, typename SHELL>
constexpr typename GpTypeShell<T, SHELL>::this_type&    GpTypeShell<T, SHELL>::operator= (const this_type& aShell)
{
    iValue = aShell.iValue;

    return *this;
}

template<typename T, typename SHELL>
constexpr typename GpTypeShell<T, SHELL>::this_type&    GpTypeShell<T, SHELL>::operator= (this_type&& aShell) noexcept
{
    iValue = std::move(aShell.iValue);

    return *this;
}

template<typename T, typename SHELL>
constexpr bool  GpTypeShell<T, SHELL>::operator== (const this_type& aShell) const noexcept
{
    return Value() == aShell.Value();
}

template<typename T, typename SHELL>
constexpr bool  GpTypeShell<T, SHELL>::operator!= (const this_type& aShell) const noexcept
{
    return Value() != aShell.Value();
}

template<typename T, typename SHELL>
constexpr bool  GpTypeShell<T, SHELL>::operator< (const this_type& aShell) const noexcept
{
    return Value() < aShell.Value();
}

template<typename T, typename SHELL>
constexpr bool  GpTypeShell<T, SHELL>::operator<= (const this_type& aShell) const noexcept
{
    return Value() <= aShell.Value();
}

template<typename T, typename SHELL>
constexpr bool  GpTypeShell<T, SHELL>::operator> (const this_type& aShell) const noexcept
{
    return Value() > aShell.Value();
}

template<typename T, typename SHELL>
constexpr bool  GpTypeShell<T, SHELL>::operator>= (const this_type& aShell) const noexcept
{
    return Value() >= aShell.Value();
}

}// namespace GPlatform

namespace std {

template<::GPlatform::Concepts::TypeShell::IsTypeShell T>
struct hash<T>
{
    constexpr std::size_t operator()(T const& aValue) const noexcept
    {
        return std::hash<typename T::value_type>()(aValue.Value());
    }
};

template<::GPlatform::Concepts::TypeShell::IsTypeShell T>
::std::string   to_string (const T& aValue)
{
    return ::std::to_string(aValue.Value());
}

}// namespace std
