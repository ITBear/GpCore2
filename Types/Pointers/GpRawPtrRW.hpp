#pragma once

#include "GpRawPtrR.hpp"

namespace GPlatform {

template<typename T>
class GpRawPtrRW: public GpRawPtrR<T>
{
    static_assert (std::is_pointer_v<T>, "T must be pointer");

public:
    using this_type				= GpRawPtrRW<T>;
    using value_type			= std::remove_cvref_t<std::remove_pointer_t<T>>;
    using pointer_type			= value_type*;
    using pointer_type_const	= const value_type*;

public:
                                    GpRawPtrRW			(void) noexcept = delete;
    constexpr						GpRawPtrRW			(const this_type& aRawPtr) noexcept;
    constexpr						GpRawPtrRW			(this_type&& aRawPtr) noexcept;
    constexpr						GpRawPtrRW			(T aPtr, const count_t aCount);
                                    ~GpRawPtrRW			(void) noexcept = default;

    constexpr pointer_type			Ptr					(void);
    constexpr pointer_type			Ptr					(const count_t aOffset);
    constexpr value_type&			At					(const count_t aOffset);
    constexpr value_type&			operator*			(void);
    constexpr value_type&			operator[]			(const count_t aOffset);

    constexpr this_type&			operator++			(void);
    constexpr this_type				operator++			(int);

    constexpr this_type&			operator--			(void);
    constexpr this_type				operator--			(int);

    constexpr this_type&			operator+=			(const count_t aOffset);
    constexpr this_type&			operator-=			(const count_t aOffset);
};

template<typename T> constexpr
GpRawPtrRW<T>::GpRawPtrRW (const this_type& aRawPtr) noexcept:
GpRawPtrR<T>(aRawPtr)
{
}

template<typename T> constexpr
GpRawPtrRW<T>::GpRawPtrRW (this_type&& aRawPtr) noexcept:
GpRawPtrR<T>(std::move(aRawPtr))
{
}

template<typename T> constexpr
GpRawPtrRW<T>::GpRawPtrRW (T aPtr, const count_t aCount):
GpRawPtrR<T>(aPtr, aCount)
{
}

template<typename T> constexpr
typename GpRawPtrRW<T>::pointer_type  GpRawPtrRW<T>::Ptr (void)
{
    return Ptr(0_cnt);
}

template<typename T> constexpr
typename GpRawPtrRW<T>::pointer_type  GpRawPtrRW<T>::Ptr (const count_t aOffset)
{
    const count_t countLeft = GpRawPtrR<T>::CountLeft();
    THROW_GPE_COND_CHECK_M(aOffset < countLeft, "Out of range"_sv);
    return GpRawPtrR<T>::iPtr + aOffset.ValueAs<size_t>();
}

template<typename T> constexpr
typename GpRawPtrRW<T>::value_type&	GpRawPtrRW<T>::At (const count_t aOffset)
{
    return *Ptr(aOffset);
}

template<typename T> constexpr
typename GpRawPtrRW<T>::value_type&	GpRawPtrRW<T>::operator* (void)
{
    return *Ptr();
}

template<typename T> constexpr
typename GpRawPtrRW<T>::value_type&	GpRawPtrRW<T>::operator[] (const count_t aOffset)
{
    return At(aOffset);
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type&	GpRawPtrRW<T>::operator++ (void)
{
    GpRawPtrR<T>::OffsetAdd(1_cnt);
    return *this;
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type	GpRawPtrRW<T>::operator++ (int)
{
    this_type res(*this);
    GpRawPtrR<T>::OffsetAdd(1_cnt);
    return res;
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type&	GpRawPtrRW<T>::operator-- (void)
{
    GpRawPtrR<T>::OffsetSub(1_cnt);
    return *this;
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type	GpRawPtrRW<T>::operator-- (int)
{
    this_type res(*this);
    GpRawPtrR<T>::OffsetSub(1_cnt);
    return res;
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type&	GpRawPtrRW<T>::operator+= (const count_t aOffset)
{
    GpRawPtrR<T>::OffsetAdd(aOffset);
    return *this;
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type&	GpRawPtrRW<T>::operator-= (const count_t aOffset)
{
    GpRawPtrR<T>::OffsetSub(aOffset);
    return *this;
}

}//GPlatform
