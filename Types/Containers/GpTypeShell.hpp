#pragma once

#include "../../GpCore_global.hpp"
#include <utility>

namespace GPlatform {

template<typename T, typename SHELL>
class GpTypeShell
{
public:
	using value_type	= T;
	using shell_type	= SHELL;
	using this_type		= GpTypeShell<T, SHELL>;

public:
							GpTypeShell		(void) noexcept = default;
							GpTypeShell		(const GpTypeShell& aShell);
							GpTypeShell		(GpTypeShell&& aShell) noexcept;
							GpTypeShell		(const T& aValue);
							GpTypeShell		(T&& aValue) noexcept;
							~GpTypeShell	(void) noexcept = default;

	T*						operator ->		(void) noexcept {return &iValue;}
	const T*				operator ->		(void) const noexcept {return &iValue;}

	T&						Val				(void) noexcept {return iValue;}
	const T&				Val				(void) const noexcept {return iValue;}

	T&&						ValueMove		(void) noexcept {return std::move(iValue);}

	this_type&				operator=		(const this_type& aShell);
	this_type&				operator=		(this_type&& aShell) noexcept;

private:
	T						iValue	= T();
};

template<typename T, typename SHELL>
GpTypeShell<T, SHELL>::GpTypeShell (const GpTypeShell& aShell):
iValue(aShell.iValue)
{
}

template<typename T, typename SHELL>
GpTypeShell<T, SHELL>::GpTypeShell (GpTypeShell&& aShell) noexcept:
iValue(std::move(aShell.iValue))
{
}

template<typename T, typename SHELL>
GpTypeShell<T, SHELL>::GpTypeShell (const T& aValue):
iValue(aValue)
{
}

template<typename T, typename SHELL>
GpTypeShell<T, SHELL>::GpTypeShell (T&& aValue) noexcept:
iValue(std::move(aValue))
{
}

template<typename T, typename SHELL>
typename GpTypeShell<T, SHELL>::this_type&	GpTypeShell<T, SHELL>::operator= (const this_type& aShell)
{
	iValue = aShell.iValue;

	return *this;
}

template<typename T, typename SHELL>
typename GpTypeShell<T, SHELL>::this_type&	GpTypeShell<T, SHELL>::operator= (this_type&& aShell) noexcept
{
	iValue = std::move(aShell.iValue);

	return *this;
}

}//GPlatform
