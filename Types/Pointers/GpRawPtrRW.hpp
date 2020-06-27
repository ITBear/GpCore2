#pragma once

#include "GpRawPtrR.hpp"

namespace GPlatform {

template<typename T>
class GpRawPtrRW: public GpRawPtrR<T>
{
    static_assert (std::is_pointer_v<T>, "T must be pointer");

public:
    using this_type     = GpRawPtrRW<T>;
    using value_type    = typename GpRawPtrR<T>::value_type;

public:
                                    GpRawPtrRW          (void) noexcept = delete;
    constexpr                       GpRawPtrRW          (this_type& aRawPtr) noexcept;
    constexpr                       GpRawPtrRW          (this_type&& aRawPtr) noexcept;
    constexpr                       GpRawPtrRW          (T aPtr, const count_t aCount);
    constexpr                       GpRawPtrRW          (T aPtr, const size_byte_t aSize);
                                    ~GpRawPtrRW         (void) noexcept = default;

    template<typename D>
    constexpr void                  CopyFrom            (const D& aRawPtr);
    constexpr void                  CopyFrom            (const value_type* aPtr, const count_t aCount);

    constexpr const value_type*     Ptr                 (void) const;
    constexpr const value_type*     Ptr                 (const count_t aOffset) const;
    constexpr value_type*           Ptr                 (void);
    constexpr value_type*           Ptr                 (const count_t aOffset);

    template<typename R>
    constexpr R                     PtrAs               (void);

    template<typename R>
    constexpr R                     PtrAs               (const count_t aOffset);

    constexpr value_type&           At                  (const count_t aOffset);
    constexpr value_type&           operator*           (void);
    constexpr value_type&           operator[]          (const count_t aOffset);

    constexpr this_type&            operator++          (void);
    constexpr this_type             operator++          (int);

    constexpr this_type&            operator--          (void);
    constexpr this_type             operator--          (int);

    constexpr this_type&            operator+=          (const count_t aOffset);
    constexpr this_type&            operator-=          (const count_t aOffset);
};

template<typename T> constexpr
GpRawPtrRW<T>::GpRawPtrRW (this_type& aRawPtr) noexcept:
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
GpRawPtrRW<T>::GpRawPtrRW (T aPtr, const size_byte_t aSize):
GpRawPtrR<T>(aPtr, aSize)
{
}

template<typename T>
template<typename D>
constexpr
void    GpRawPtrRW<T>::CopyFrom (const D& aRawPtr)
{
    const count_t countLeftThis     = GpRawPtrR<T>::CountLeft();
    const count_t countLeftOther    = aRawPtr.CountLeft();
    THROW_GPE_COND_CHECK_M(countLeftThis >= countLeftOther, "Out of range"_sv);
    MemOps::SCopy(Ptr(), aRawPtr.Ptr(), countLeftOther);
}

template<typename T> constexpr
void    GpRawPtrRW<T>::CopyFrom (const value_type* aPtr, const count_t aCount)
{
    const count_t countLeftThis = GpRawPtrR<T>::CountLeft();
    THROW_GPE_COND_CHECK_M(countLeftThis >= aCount, "Out of range"_sv);
    MemOps::SCopy(Ptr(), aPtr, aCount);
}

template<typename T> constexpr
const typename GpRawPtrRW<T>::value_type*  GpRawPtrRW<T>::Ptr (void) const
{
    return GpRawPtrR<T>::Ptr();
}

template<typename T> constexpr
const typename GpRawPtrRW<T>::value_type*  GpRawPtrRW<T>::Ptr (const count_t aOffset) const
{
    return GpRawPtrR<T>::Ptr(aOffset);
}

template<typename T> constexpr
typename GpRawPtrRW<T>::value_type*  GpRawPtrRW<T>::Ptr (void)
{
    return Ptr(0_cnt);
}

template<typename T> constexpr
typename GpRawPtrRW<T>::value_type*  GpRawPtrRW<T>::Ptr (const count_t aOffset)
{
    auto r = std::as_const(*this).GpRawPtrR<T>::Ptr(aOffset);
    return const_cast<value_type*>(r);
}

template<typename T>
template<typename R>
constexpr
R   GpRawPtrRW<T>::PtrAs (void)
{
    return PtrAs<R>(0_cnt);
}

template<typename T>
template<typename R>
constexpr
R   GpRawPtrRW<T>::PtrAs (const count_t aOffset)
{
    using r_value_type = std::remove_cvref_t<std::remove_pointer_t<R>>;
    static_assert(sizeof(value_type) == sizeof(r_value_type));
    static_assert(alignof(value_type) == alignof(r_value_type));

    return reinterpret_cast<R>(Ptr(aOffset));
}

template<typename T> constexpr
typename GpRawPtrRW<T>::value_type& GpRawPtrRW<T>::At (const count_t aOffset)
{
    return *Ptr(aOffset);
}

template<typename T> constexpr
typename GpRawPtrRW<T>::value_type& GpRawPtrRW<T>::operator* (void)
{
    return *Ptr();
}

template<typename T> constexpr
typename GpRawPtrRW<T>::value_type& GpRawPtrRW<T>::operator[] (const count_t aOffset)
{
    return At(aOffset);
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type&  GpRawPtrRW<T>::operator++ (void)
{
    GpRawPtrR<T>::OffsetAdd(1_cnt);
    return *this;
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type   GpRawPtrRW<T>::operator++ (int)
{
    this_type res(*this);
    GpRawPtrR<T>::OffsetAdd(1_cnt);
    return res;
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type&  GpRawPtrRW<T>::operator-- (void)
{
    GpRawPtrR<T>::OffsetSub(1_cnt);
    return *this;
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type   GpRawPtrRW<T>::operator-- (int)
{
    this_type res(*this);
    GpRawPtrR<T>::OffsetSub(1_cnt);
    return res;
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type&  GpRawPtrRW<T>::operator+= (const count_t aOffset)
{
    GpRawPtrR<T>::OffsetAdd(aOffset);
    return *this;
}

template<typename T> constexpr
typename GpRawPtrRW<T>::this_type&  GpRawPtrRW<T>::operator-= (const count_t aOffset)
{
    GpRawPtrR<T>::OffsetSub(aOffset);
    return *this;
}

}//GPlatform
