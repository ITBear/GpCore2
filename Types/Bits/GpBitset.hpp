#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_BITSET)

#include "GpBitOps.hpp"
#include <type_traits>
#include <initializer_list>

namespace GPlatform{

template<typename MaskT,//enum or integral
		 typename ValueT>
class GpBitset
{
public:
	static_assert(std::is_enum<MaskT>() || std::is_integral<MaskT>(), "MaskT must be enum or integral");
	static_assert(std::is_integral<ValueT>(), "ValueT must be integral");

	using mask_type				= MaskT;
	using value_type			= ValueT;
	using value_type_no_volatile= typename std::remove_volatile<ValueT>::type;
	using this_type				= GpBitset<MaskT, ValueT>;
	using this_type_no_volatile	= GpBitset<MaskT, value_type_no_volatile>;
	using init_mask_type		= std::initializer_list<MaskT>;

private:
	constexpr							GpBitset			(const value_type_no_volatile aValue) noexcept;

public:
	constexpr							GpBitset			(void) noexcept;
	constexpr							GpBitset			(const this_type& aBitset) noexcept;
	constexpr							GpBitset			(this_type&& aBitset) noexcept;
	constexpr							GpBitset			(const init_mask_type aMask) noexcept;
	constexpr							GpBitset			(const GpVector<MaskT>& aMask) noexcept;

	constexpr value_type_no_volatile	Value				(void) const noexcept;
	constexpr this_type					MakeCopy			(void) const noexcept;
	constexpr this_type_no_volatile		MakeCopyNoVolatile	(void) const noexcept;
	constexpr this_type&				Clear				(void) noexcept;
	constexpr this_type&				SetValue			(const value_type_no_volatile aValue) noexcept;
	constexpr this_type&				Set					(const this_type& aBitset) noexcept;
	constexpr this_type&				SetNV				(const this_type_no_volatile& aBitset) noexcept;

	constexpr this_type&				operator=			(const this_type& aBitset) noexcept;
	constexpr this_type&				operator=			(this_type&& aBitset) noexcept;
	constexpr bool						operator==			(const this_type& aBitset) const noexcept;
	constexpr bool						operator!=			(const this_type& aBitset) const noexcept;

	constexpr this_type&				And					(const init_mask_type aMask) noexcept;
	constexpr this_type&				Up					(const init_mask_type aMask) noexcept;
	constexpr this_type&				Up					(const value_type_no_volatile aMask) noexcept;
	constexpr this_type&				Down				(const init_mask_type aMask) noexcept;
	constexpr this_type&				Down				(const value_type_no_volatile aMask) noexcept;
	constexpr this_type&				UpAndDown			(const init_mask_type aMaskUp,
															 const init_mask_type aMaskDown) noexcept;
	constexpr this_type&				UpAndDown			(const value_type_no_volatile aMaskUp,
															 const value_type_no_volatile aMaskDown) noexcept;
	constexpr this_type&				Invert				(const init_mask_type aMask) noexcept;
	constexpr this_type&				SHL					(const value_type_no_volatile aShift) noexcept;
	constexpr this_type&				SHR					(const value_type_no_volatile aShift) noexcept;

	constexpr bool						TestAny				(const init_mask_type aMask) const noexcept;
	constexpr bool						TestAll				(const init_mask_type aMask) const noexcept;
	constexpr bool						Match				(const value_type_no_volatile	aValue,
															 const init_mask_type			aMask) const noexcept;

	constexpr size_t					LeadingZeroCount	(void) const noexcept;
	constexpr size_t					TrailingZeroCount	(void) const noexcept;
	constexpr size_t					LeastSignificantBit	(void) const noexcept;

	static constexpr
	value_type_no_volatile				MakeValueFromList	(const init_mask_type aMask) noexcept;

	static
	value_type_no_volatile				MakeValueFromList	(const GpVector<MaskT>& aMask) noexcept;

	static constexpr
	this_type							MakeFromValue		(const value_type_no_volatile aValue) noexcept;

	static this_type*					GetByAddr			(const std::ptrdiff_t aADDR) noexcept;
	static this_type_no_volatile		CopyFromAddr		(const std::ptrdiff_t aADDR) noexcept;
	static void							WriteToAddr			(const std::ptrdiff_t			aADDR,
															 const value_type_no_volatile	aValue) noexcept;
	static void							WriteToAddr			(const std::ptrdiff_t			aADDR,
															 const this_type_no_volatile&	aBitset) noexcept;

	template<typename		REGTYPE,
			 std::ptrdiff_t	ADDR>
	static void							Up					(init_mask_type	aMask)
	{
		this_type reg(REGTYPE::CopyFromAddr(ADDR));
		reg.Up(aMask);
		REGTYPE::WriteToAddr(ADDR, reg);
	}

	template<typename		REGTYPE,
			 std::ptrdiff_t	ADDR>
	static void							Down				(init_mask_type	aMask)
	{
		this_type reg(REGTYPE::CopyFromAddr(ADDR));
		reg.Down(aMask);
		REGTYPE::WriteToAddr(ADDR, reg);
	}

private:
	value_type							iValue;
};

template<typename MaskT,
		 typename ValueT> constexpr
GpBitset<MaskT, ValueT>::GpBitset (const value_type_no_volatile aValue) noexcept:
iValue(aValue)
{
	//NOP
}

template<typename MaskT,
		 typename ValueT> constexpr
GpBitset<MaskT, ValueT>::GpBitset (void) noexcept:
iValue(value_type_no_volatile(0))
{
	//NOP
}

template<typename MaskT,
		 typename ValueT> constexpr
GpBitset<MaskT, ValueT>::GpBitset (const this_type& aBitset) noexcept:
iValue(aBitset.Value())
{
	//NOP
}

template<typename MaskT,
		 typename ValueT> constexpr
GpBitset<MaskT, ValueT>::GpBitset (this_type&& aBitset) noexcept:
iValue(aBitset.Value())
{
	//NOP
}

template<typename MaskT,
		 typename ValueT> constexpr
GpBitset<MaskT, ValueT>::GpBitset (const init_mask_type aMask) noexcept:
GpBitset(MakeValueFromList(aMask))
{
	//NOP
}

template<typename MaskT,
		 typename ValueT> constexpr
GpBitset<MaskT, ValueT>::GpBitset (const GpVector<MaskT>& aMask) noexcept:
GpBitset(MakeValueFromList(aMask))
{
	//NOP
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::Value (void) const noexcept -> value_type_no_volatile
{
	return iValue;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::MakeCopy (void) const noexcept -> this_type
{
	return this_type::MakeFromValue(Value());
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::MakeCopyNoVolatile (void) const noexcept -> this_type_no_volatile
{
	return this_type_no_volatile::MakeFromValue(Value());
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::Clear (void) noexcept -> this_type&
{
	iValue = value_type_no_volatile(0);

	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::SetValue (const value_type_no_volatile aValue) noexcept -> this_type&
{
	iValue = aValue;

	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::Set (const this_type& aBitset) noexcept -> this_type&
{
	return SetValue(aBitset.Value());
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::SetNV (const this_type_no_volatile& aBitset) noexcept -> this_type&
{
	return SetValue(aBitset.Value());
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::operator=	(const this_type& aBitset) noexcept -> this_type&
{
	iValue = aBitset.Value();
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::operator=	(this_type&& aBitset) noexcept -> this_type&
{
	iValue = aBitset.Value();
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
bool	GpBitset<MaskT, ValueT>::operator!=	(const this_type& aBitset) const noexcept
{
	return iValue != aBitset.Value();
}


template<typename MaskT,
		 typename ValueT> constexpr
bool	GpBitset<MaskT, ValueT>::operator==	(const this_type& aBitset) const noexcept
{
	return iValue == aBitset.Value();
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::And (const init_mask_type aMask) noexcept -> this_type&
{
	iValue = BitOps::And(Value(), MakeValueFromList(aMask));
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::Up (const init_mask_type aMask) noexcept -> this_type&
{
	iValue = BitOps::Up(Value(), MakeValueFromList(aMask));
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::Up (const value_type_no_volatile aMask) noexcept -> this_type&
{
	iValue = BitOps::Up(Value(), aMask);
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::Down (const init_mask_type aMask) noexcept -> this_type&
{
	iValue = BitOps::Down(Value(), MakeValueFromList(aMask));
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::Down (const value_type_no_volatile aMask) noexcept -> this_type&
{
	iValue = BitOps::Down(Value(), aMask);
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::UpAndDown (const init_mask_type aMaskUp,
											const init_mask_type aMaskDown) noexcept -> this_type&
{
	iValue = BitOps::UpAndDown(Value(), MakeValueFromList(aMaskUp), MakeValueFromList(aMaskDown));
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::UpAndDown (const value_type_no_volatile aMaskUp,
											const value_type_no_volatile aMaskDown) noexcept -> this_type&
{
	iValue = BitOps::UpAndDown(Value(), aMaskUp, aMaskDown);
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::Invert (const init_mask_type aMask) noexcept -> this_type&
{
	iValue = BitOps::Invert(Value(), MakeValueFromList(aMask));
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::SHL (const value_type_no_volatile aShift) noexcept -> this_type&
{
	iValue = BitOps::SHL(Value(), aShift);
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::SHR (const value_type_no_volatile aShift) noexcept -> this_type&
{
	iValue = BitOps::SHR(Value(), aShift);
	return *this;
}

template<typename MaskT,
		 typename ValueT> constexpr
bool	GpBitset<MaskT, ValueT>::TestAny (const init_mask_type aMask) const noexcept
{
	return BitOps::TestAny(Value(), MakeValueFromList(aMask));
}

template<typename MaskT,
		 typename ValueT> constexpr
bool	GpBitset<MaskT, ValueT>::TestAll (const init_mask_type aMask) const noexcept
{
	return BitOps::TestAll(Value(), MakeValueFromList(aMask));
}

template<typename MaskT,
		 typename ValueT> constexpr
bool	GpBitset<MaskT, ValueT>::Match (const value_type_no_volatile	aValue,
										const init_mask_type			aMask) const noexcept
{
	return BitOps::Match(Value(), aValue, MakeValueFromList(aMask));
}

template<typename MaskT,
		 typename ValueT> constexpr
size_t	GpBitset<MaskT, ValueT>::LeadingZeroCount (void) const noexcept
{
	return BitOps::LeadingZeroCount(Value());
}

template<typename MaskT,
		 typename ValueT> constexpr
size_t	GpBitset<MaskT, ValueT>::TrailingZeroCount (void) const noexcept
{
	return BitOps::TrailingZeroCount(Value());
}

template<typename MaskT,
		 typename ValueT> constexpr
size_t	GpBitset<MaskT, ValueT>::LeastSignificantBit (void) const noexcept
{
	return BitOps::LeastSignificantBit(Value());
}

template<typename MaskT,
		 typename ValueT> constexpr
auto GpBitset<MaskT, ValueT>::MakeValueFromList (const init_mask_type aMask) noexcept -> value_type_no_volatile
{
	size_t	bitset = size_t(0);

	for (MaskT bitId: aMask)
	{
		bitset |= BitOps::SHL(size_t(1), bitId);
	}

	return value_type_no_volatile(bitset);
}

template<typename MaskT,
		 typename ValueT>
auto GpBitset<MaskT, ValueT>::MakeValueFromList (const GpVector<MaskT>& aMask) noexcept -> value_type_no_volatile
{
	size_t	bitset = size_t(0);

	for (MaskT bitId: aMask)
	{
		bitset |= BitOps::SHL(size_t(1), bitId);
	}

	return value_type_no_volatile(bitset);
}

template<typename MaskT,
		 typename ValueT> constexpr
auto	GpBitset<MaskT, ValueT>::MakeFromValue (const value_type_no_volatile aValue) noexcept -> this_type
{
	return this_type(aValue);
}

template<typename MaskT,
		 typename ValueT>
auto	GpBitset<MaskT, ValueT>::GetByAddr (const std::ptrdiff_t aADDR) noexcept -> this_type*
{
	return reinterpret_cast<this_type*>(aADDR);
}

template<typename MaskT,
		 typename ValueT>
auto	GpBitset<MaskT, ValueT>::CopyFromAddr (const std::ptrdiff_t aADDR) noexcept -> this_type_no_volatile
{
	return GetByAddr(aADDR)->MakeCopyNoVolatile();
}

template<typename MaskT,
		 typename ValueT>
void	GpBitset<MaskT, ValueT>::WriteToAddr (const std::ptrdiff_t			aADDR,
											  const value_type_no_volatile	aValue) noexcept
{
	GetByAddr(aADDR)->SetValue(aValue);
}

template<typename MaskT,
		 typename ValueT>
void	GpBitset<MaskT, ValueT>::WriteToAddr (const std::ptrdiff_t			aADDR,
											  const this_type_no_volatile&	aBitset) noexcept
{
	GetByAddr(aADDR)->SetNV(aBitset);
}

}//GPlatform

#endif//#if defined(GP_USE_BITSET)
