#pragma once

#include "../Units/Other/count_t.hpp"
#include "../Units/Other/size_byte_t.hpp"
#include "../Strings/GpStringLiterals.hpp"
#include "../../Memory/GpMemOps.hpp"
#include "../../Exceptions/GpException.hpp"

namespace GPlatform {

template<typename T>
class GpRawPtrR
{
	static_assert (std::is_pointer_v<T>, "T must be pointer");

public:
	using this_type				= GpRawPtrR<T>;
	using value_type			= std::remove_cvref_t<std::remove_pointer_t<T>>;
	using pointer_type			= value_type*;
	using pointer_type_const	= const value_type*;

public:
									GpRawPtrR			(void) noexcept = delete;
	constexpr						GpRawPtrR			(const this_type& aRawPtr) noexcept;
	constexpr						GpRawPtrR			(this_type&& aRawPtr) noexcept;
	constexpr						GpRawPtrR			(T aPtr, const count_t aCount);
									~GpRawPtrR			(void) noexcept = default;

	constexpr count_t				CountTotal			(void) const noexcept {return iCount;}
	constexpr count_t				CountLeft			(void) const noexcept {return iCount - iOffset;}
	constexpr count_t				LengthTotal			(void) const noexcept {return iCount;}
	constexpr count_t				LengthLeft			(void) const noexcept {return iCount - iOffset;}
	constexpr size_byte_t			SizeTotal			(void) const {return size_byte_t::SMake((CountTotal() * count_t::SMake(sizeof(value_type))).Value());}
	constexpr size_byte_t			SizeLeft			(void) const {return size_byte_t::SMake((CountLeft() * count_t::SMake(sizeof(value_type))).Value());}

	constexpr void					OffsetAdd			(const count_t aOffset);
	constexpr void					OffsetSub			(const count_t aOffset);

	constexpr pointer_type_const	Ptr					(void) const;
	constexpr pointer_type_const	Ptr					(const count_t aOffset) const;
	constexpr const value_type&		At					(const count_t aOffset) const;
	constexpr const value_type&		operator*			(void) const;
	constexpr const value_type&		operator[]			(const count_t aOffset) const;

	constexpr this_type&			operator++			(void);
	constexpr this_type				operator++			(int);

	constexpr this_type&			operator--			(void);
	constexpr this_type				operator--			(int);

	constexpr this_type&			operator+=			(const count_t aOffset);
	constexpr this_type&			operator-=			(const count_t aOffset);

	constexpr bool					IsEqualByMinLen		(const this_type& aRawPtr) const noexcept;
	constexpr bool					IsEqualByThisLen	(const this_type& aRawPtr) const noexcept;
	constexpr bool					IsEqualByArgLen		(const this_type& aRawPtr) const noexcept;

	constexpr std::string_view		AsStringView		(void) const noexcept {return std::string_view();}

protected:
	T				iPtr	= nullptr;
	const count_t	iCount	= 0_cnt;
	count_t			iOffset	= 0_cnt;
};

template<typename T> constexpr
GpRawPtrR<T>::GpRawPtrR (const this_type& aRawPtr) noexcept:
iPtr(aRawPtr.iPtr),
iCount(aRawPtr.iCount),
iOffset(aRawPtr.iOffset)
{
}

template<typename T> constexpr
GpRawPtrR<T>::GpRawPtrR (this_type&& aRawPtr) noexcept:
iPtr(std::move(aRawPtr.iPtr)),
iCount(std::move(aRawPtr.iCount)),
iOffset(std::move(aRawPtr.iOffset))
{
}

template<typename T> constexpr
GpRawPtrR<T>::GpRawPtrR (T aPtr, const count_t aCount):
iPtr(aPtr),
iCount(aCount),
iOffset(0_cnt)
{
	THROW_GPE_COND_CHECK_M(iPtr != nullptr, "nullptr"_sv);
	THROW_GPE_COND_CHECK_M(iCount >= 1_cnt, "Count is less than 1"_sv);
}

template<typename T> constexpr
void	GpRawPtrR<T>::OffsetAdd (const count_t aOffset)
{
	const count_t countLeft = CountLeft();
	THROW_GPE_COND_CHECK_M(aOffset <= countLeft, "Out of range"_sv);

	iOffset	+= aOffset;
	iPtr	+= aOffset.ValueAs<size_t>();
}

template<typename T> constexpr
void	GpRawPtrR<T>::OffsetSub (const count_t aOffset)
{
	THROW_GPE_COND_CHECK_M(aOffset <= iOffset, "Out of range"_sv);

	iOffset	-= aOffset;
	iPtr	-= aOffset.ValueAs<size_t>();
}

template<typename T> constexpr
typename GpRawPtrR<T>::pointer_type_const  GpRawPtrR<T>::Ptr (void) const
{
	return Ptr(0_cnt);
}

template<typename T> constexpr
typename GpRawPtrR<T>::pointer_type_const  GpRawPtrR<T>::Ptr (const count_t aOffset) const
{
	const count_t countLeft = CountLeft();
	THROW_GPE_COND_CHECK_M(aOffset < countLeft, "Out of range"_sv);
	return iPtr + aOffset.ValueAs<size_t>();
}

template<typename T> constexpr
const typename GpRawPtrR<T>::value_type&	GpRawPtrR<T>::At (const count_t aOffset) const
{
	return *Ptr(aOffset);
}

template<typename T> constexpr
const typename GpRawPtrR<T>::value_type&	GpRawPtrR<T>::operator* (void) const
{
	return *Ptr();
}

template<typename T> constexpr
const typename GpRawPtrR<T>::value_type&	GpRawPtrR<T>::operator[] (const count_t aOffset) const
{
	return At(aOffset);
}

template<typename T> constexpr
typename GpRawPtrR<T>::this_type&	GpRawPtrR<T>::operator++ (void)
{
	OffsetAdd(1_cnt);
	return *this;
}

template<typename T> constexpr
typename GpRawPtrR<T>::this_type	GpRawPtrR<T>::operator++ (int)
{
	this_type res(*this);
	OffsetAdd(1_cnt);
	return res;
}

template<typename T> constexpr
typename GpRawPtrR<T>::this_type&	GpRawPtrR<T>::operator-- (void)
{
	OffsetSub(1_cnt);
	return *this;
}

template<typename T> constexpr
typename GpRawPtrR<T>::this_type	GpRawPtrR<T>::operator-- (int)
{
	this_type res(*this);
	OffsetSub(1_cnt);
	return res;
}

template<typename T> constexpr
typename GpRawPtrR<T>::this_type&	GpRawPtrR<T>::operator+= (const count_t aOffset)
{
	OffsetAdd(aOffset);
	return *this;
}

template<typename T> constexpr
typename GpRawPtrR<T>::this_type&	GpRawPtrR<T>::operator-= (const count_t aOffset)
{
	OffsetSub(aOffset);
	return *this;
}

template<typename T> constexpr
bool	GpRawPtrR<T>::IsEqualByMinLen (const this_type& aRawPtr) const noexcept
{
	const count_t count = std::min(CountLeft(), aRawPtr.CountLeft());
	return MemOps::SCompare(Ptr(), aRawPtr.Ptr(), count) == 0;
}

template<typename T> constexpr
bool	GpRawPtrR<T>::IsEqualByThisLen (const this_type& aRawPtr) const noexcept
{
	const count_t thisCount = CountLeft();

	return    (aRawPtr.CountLeft() >= thisCount)
		   && (MemOps::SCompare(Ptr(), aRawPtr.Ptr(), thisCount) == 0);
}

template<typename T> constexpr
bool	GpRawPtrR<T>::IsEqualByArgLen (const this_type& aRawPtr) const noexcept
{
	const count_t argCount = aRawPtr.CountLeft();

	return    (CountLeft() >= argCount)
		   && (MemOps::SCompare(Ptr(), aRawPtr.Ptr(), argCount) == 0);
}

}//GPlatform
