#pragma once

#include "../Units/Other/count_t.hpp"
#include "../Units/Other/size_byte_t.hpp"
#include "../Strings/GpStringLiterals.hpp"
#include "../../Memory/GpMemOps.hpp"
#include "../../Exceptions/GpException.hpp"

#include <iterator>
#include <concepts>
#include <vector>

namespace GPlatform {

template<typename T>
class GpRawPtr
{
public:
    CLASS_TAG(GpRawPtr)
    CLASS_TAG_DETECTOR(GpRawPtr)

    template<typename P>
    using value_type_t              = std::remove_cvref_t<std::remove_pointer_t<P>>;

    template<typename P>
    static consteval bool is_ptr_v  =     std::is_pointer_v<P>
                                      && !std::is_pointer_v<std::remove_pointer_t<P>>;

    static_assert(is_ptr_v<T>, "T must be pointer to value");

    using pointer_type  = T;
    using this_type     = GpRawPtr<T>;   
    using value_type    = value_type_t<T>;

    static constexpr size_byte_t value_size_v = size_byte_t::SMake(sizeof(value_type));

    /* CAST:
     * FROM type*       -> TO byte*
     * FROM type*       -> TO char*
     * FROM type*       -> TO unsigned char*
     * FROM type*       -> TO const byte*
     * FROM type*       -> TO const char*
     * FROM type*       -> TO const unsigned char*
     * FROM type*       -> TO type*
     * FROM type*       -> TO const type*
     * FROM const type* -> TO const byte*
     * FROM const type* -> TO const char*
     * FROM const type* -> TO const unsigned char*
     * FROM const type* -> TO const type*
    */

    /* REINTERPRET:
     * FROM byte*                   -> TO type*
     * FROM char*                   -> TO type*
     * FROM unsigned char*          -> TO type*
     * FROM byte*                   -> TO const type*
     * FROM char*                   -> TO const type*
     * FROM unsigned char*          -> TO const type*
     * FROM type*                   -> TO type*
     * FROM type*                   -> TO const type*
     * FROM const byte*             -> TO const type*
     * FROM const char*             -> TO const type*
     * FROM const unsigned char*    -> TO const type*
     * FROM const type*             -> TO const type*
    */

    template<typename P>
    static consteval bool is_const_v                =   std::is_const_v<std::remove_pointer_t<P>>;

    template<typename FROM, typename TO>
    static consteval bool is_same_const_v           =    std::is_same_v<TO, const char*>
                                                      || std::is_same_v<TO, const unsigned char*>
                                                      || std::is_same_v<TO, const std::byte*>
                                                      || std::is_same_v<TO, const value_type_t<FROM>*>;

    template<typename FROM, typename TO>
    static consteval bool is_same_v                 =    std::is_same_v<TO, char*>
                                                      || std::is_same_v<TO, unsigned char*>
                                                      || std::is_same_v<TO, std::byte*>
                                                      || std::is_same_v<TO, value_type_t<FROM>*>;

    template<typename FROM, typename TO>
    static consteval bool is_convertable_ptr_v      =    is_ptr_v<FROM>
                                                      && is_ptr_v<TO>
                                                      && (   (   is_const_v<FROM>
                                                              && is_const_v<TO>
                                                              && is_same_const_v<FROM, TO>
                                                             )
                                                          || (  !is_const_v<FROM>
                                                              && (   is_same_const_v<FROM, TO>
                                                                  || is_same_v<FROM, TO>
                                                                 )
                                                             )
                                                         );

    template<typename FROM, typename TO>
    static consteval bool is_reinterpretable_ptr_v  =    is_ptr_v<FROM>
                                                      && is_ptr_v<TO>
                                                      && (   (   is_const_v<FROM>
                                                              && is_const_v<TO>
                                                              && is_same_const_v<TO, FROM>
                                                             )
                                                          || (  !is_const_v<FROM>
                                                              && (   is_same_const_v<TO, FROM>
                                                                  || is_same_v<TO, FROM>
                                                                 )
                                                             )
                                                         );

    template<typename FROM, typename TO>
    static consteval bool is_countable_ptr_v        =    is_ptr_v<FROM>
                                                      && is_ptr_v<TO>
                                                      && (   is_same_const_v<FROM, TO>
                                                          || is_same_v<FROM, TO>
                                                         );

    template<class C>
    using has_pointer_type_v                        = typename C::pointer_type;

    template<typename C>
    using pointer_type_t                            = std::experimental::detected_or_t<void, has_pointer_type_v, C>;

    template<typename FROM, typename TO>
    static consteval bool is_convertable_raw_v      =    SHasTag_GpRawPtr<FROM>
                                                      && SHasTag_GpRawPtr<TO>
                                                      && is_convertable_ptr_v<pointer_type_t<FROM>, pointer_type_t<TO>>;

    template<typename FROM, typename TO>
    static consteval bool is_reinterpretable_raw_v  =    SHasTag_GpRawPtr<FROM>
                                                      && SHasTag_GpRawPtr<TO>
                                                      && is_reinterpretable_ptr_v<pointer_type_t<FROM>, pointer_type_t<TO>>;

    template<class C>
    using has_iterator_v                            = typename C::iterator;

    template<typename C>
    using iter_t                                    = std::experimental::detected_or_t<void, has_iterator_v, C>;

    template <typename C>
    static constexpr bool is_ra_container_v         = std::random_access_iterator<iter_t<C>>;

public:
    constexpr                                   GpRawPtr        (void) noexcept: iPtr(nullptr),
                                                                                 iCount(0_cnt),
                                                                                 iOffset(0_cnt){}
    constexpr                                   GpRawPtr        (const this_type& aRawPtr) noexcept: iPtr(aRawPtr.iPtr),
                                                                                                     iCount(aRawPtr.iCount),
                                                                                                     iOffset(aRawPtr.iOffset){}

    constexpr                                   GpRawPtr        (this_type&& aRawPtr) noexcept: iPtr(std::move(aRawPtr.iPtr)),
                                                                                                iCount(std::move(aRawPtr.iCount)),
                                                                                                iOffset(std::move(aRawPtr.iOffset)){}

    constexpr                                   GpRawPtr        (pointer_type aPtr, const count_t aCount): iPtr(aPtr),
                                                                                                           iCount(aCount),
                                                                                                           iOffset(0_cnt)
    {
        THROW_GPE_COND_CHECK_M((iPtr != nullptr) || (iCount == 0_cnt), "nullptr"_sv);
        //THROW_GPE_COND_CHECK_M(iCount >= 1_cnt, "Count is less than 1"_sv);
    }

    constexpr                                   GpRawPtr        (pointer_type aPtr, const count_t aCount, const count_t aOffset): iPtr(aPtr),
                                                                                                                                  iCount(aCount),
                                                                                                                                  iOffset(aOffset)
    {
        THROW_GPE_COND_CHECK_M((iPtr != nullptr) || (iCount == 0_cnt), "nullptr"_sv);
        //THROW_GPE_COND_CHECK_M(iCount >= 1_cnt, "Count is less than 1"_sv);
        THROW_GPE_COND_CHECK_M(iCount >= iOffset, "Count < Offset"_sv);
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<P, pointer_type>, P>>
                                                GpRawPtr        (P aPtr, const size_t aSize): GpRawPtr(SPtrAs<P, pointer_type>(aPtr),
                                                                                                       SCountAs<P, pointer_type>(count_t::SMake(aSize))){}

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<P, pointer_type>, P>>
                                                GpRawPtr        (P aPtr, const count_t aSize): GpRawPtr(SPtrAs<P, pointer_type>(aPtr),
                                                                                                        SCountAs<P, pointer_type>(aSize)){}

    template<typename R, typename = std::enable_if_t<is_convertable_raw_v<R, this_type>, R>>
                                                GpRawPtr        (const R& aRawPtr): GpRawPtr(aRawPtr.template _PtrBeginAs<pointer_type>(),
                                                                                             aRawPtr.template CountTotalAs<const value_type*>(),
                                                                                             aRawPtr.template OffsetAs<const value_type*>()){}

    template<typename C, std::enable_if_t<    is_ra_container_v<C>
                                          && !is_const_v<T>
                                          &&  is_convertable_ptr_v<typename C::value_type*, pointer_type>, int> = 0>
                                                GpRawPtr        (C& aContainer): GpRawPtr(SPtrAs<typename C::value_type*, pointer_type>(aContainer.data()),
                                                                                          SCountAs<typename C::value_type*, pointer_type>(count_t::SMake(aContainer.size()))){}

    template<typename C, std::enable_if_t<   is_ra_container_v<C>
                                          && is_const_v<T>
                                          && is_convertable_ptr_v<const typename C::value_type*, pointer_type>, int> = 0>
                                                GpRawPtr        (const C& aContainer): GpRawPtr(SPtrAs<const typename C::value_type*, pointer_type>(aContainer.data()),
                                                                                                SCountAs<const typename C::value_type*, pointer_type>(count_t::SMake(aContainer.size()))){}

                                                ~GpRawPtr       (void) noexcept = default;

    constexpr void                              Set             (const this_type& aRawPtr) noexcept
    {
        iPtr    = aRawPtr.iPtr;
        iCount  = aRawPtr.iCount;
        iOffset = aRawPtr.iOffset;
    }

    constexpr void                              Set             (this_type&& aRawPtr) noexcept
    {
        iPtr    = std::move(aRawPtr.iPtr);
        iCount  = std::move(aRawPtr.iCount);
        iOffset = std::move(aRawPtr.iOffset);
    }

    constexpr void                              Set             (pointer_type aPtr, const count_t aCount)
    {
        THROW_GPE_COND_CHECK_M((iPtr != nullptr) || (iCount == 0_cnt), "nullptr"_sv);
        //THROW_GPE_COND_CHECK_M(aCount >= 1_cnt, "Count is less than 1"_sv);

        iPtr    = aPtr;
        iCount  = aCount;
        iOffset = 0_cnt;
    }

    constexpr void                              Set             (pointer_type aPtr, const count_t aCount, const count_t aOffset)
    {
        THROW_GPE_COND_CHECK_M((iPtr != nullptr) || (iCount == 0_cnt), "nullptr"_sv);
        //THROW_GPE_COND_CHECK_M(aCount >= 1_cnt, "Count is less than 1"_sv);
        THROW_GPE_COND_CHECK_M(aCount >= aOffset, "Count < Offset"_sv);

        iPtr    = aPtr + aOffset.ValueAs<size_t>();
        iCount  = aCount;
        iOffset = aOffset;
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<P, pointer_type>, P>>
    constexpr   void                            Set             (P aPtr, const size_t aSize)
    {
        Set(SPtrAs<P, pointer_type>(aPtr), SCountAs<P, pointer_type>(count_t::SMake(aSize)));
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<P, pointer_type>, P>>
    constexpr   void                            Set             (P aPtr, const count_t aSize)
    {
        Set(SPtrAs<P, pointer_type>(aPtr), SCountAs<P, pointer_type>(aSize));
    }

    template<typename R, typename = std::enable_if_t<is_convertable_raw_v<R, this_type>, R>>
    constexpr   void                            Set             (const R& aRawPtr)
    {
        Set(aRawPtr.template _PtrBeginAs<pointer_type>(),
            aRawPtr.template CountTotalAs<const value_type*>(),
            aRawPtr.template OffsetAs<const value_type*>());
    }

    constexpr this_type&                        operator=       (const this_type& aRawPtr) noexcept
    {
        Set(aRawPtr);
        return *this;
    }

    constexpr this_type&                        operator=       (this_type&& aRawPtr) noexcept
    {
        Set(std::move(aRawPtr));
        return *this;
    }

    template<typename R, typename = std::enable_if_t<is_convertable_raw_v<R, this_type>, R>>
    constexpr this_type&                        operator=       (const R& aRawPtr)
    {
        Set<R>(aRawPtr);
        return *this;
    }

    constexpr count_t                           CountTotal      (void) const noexcept
    {
        return iCount;
    }

    template<typename V>
    constexpr V                                 CountTotalV     (void) const
    {
        return CountTotal().template ValueAs<V>();
    }

    template<typename P, typename = std::enable_if_t<is_countable_ptr_v<pointer_type, P>, P>>
    constexpr count_t                           CountTotalAs    (void) const
    {
        return SCountAs<pointer_type, P>(CountTotal());
    }

    template<typename P, typename V, typename = std::enable_if_t<is_countable_ptr_v<pointer_type, P>, P>>
    constexpr V                                 CountTotalAsV   (void) const
    {
        return CountTotalAs<P>().template ValueAs<V>();
    }

    constexpr count_t                           CountLeft       (void) const noexcept
    {
        return count_t::SMake(iCount.Value() - iOffset.Value());
    }

    template<typename V>
    constexpr V                                 CountLeftV      (void) const
    {
        return CountLeft().template ValueAs<V>();
    }

    template<typename P, typename = std::enable_if_t<is_countable_ptr_v<pointer_type, P>, P>>
    constexpr count_t                           CountLeftAs     (void) const
    {
        return SCountAs<pointer_type, P>(CountLeft());
    }

    template<typename P, typename V, typename = std::enable_if_t<is_countable_ptr_v<pointer_type, P>, P>>
    constexpr V                                 CountLeftAsV    (void) const
    {
        return CountLeftAs<P>().template ValueAs<V>();
    }

    constexpr count_t                           Offset          (void) const noexcept
    {
        return iOffset;
    }

    template<typename V>
    constexpr V                                 OffsetV         (void) const
    {
        return Offset().template ValueAs<V>();
    }

    template<typename P, typename = std::enable_if_t<is_countable_ptr_v<pointer_type, P>, P>>
    constexpr count_t                           OffsetAs        (void) const
    {
        return SCountAs<pointer_type, P>(Offset());
    }

    template<typename P, typename V, typename = std::enable_if_t<is_countable_ptr_v<pointer_type, P>, P>>
    constexpr V                                 OffsetAsV       (void) const
    {
        return OffsetAs<P>().template ValueAs<V>();
    }

    constexpr size_byte_t                       SizeTotal       (void) const
    {
        return (CountTotal() * count_t::SMake(sizeof(value_type))).template ValueAs<size_byte_t>();
    }

    template<typename V>
    constexpr V                                 SizeTotalV      (void) const
    {
        return SizeTotal().template ValueAs<V>();
    }

    constexpr size_byte_t                       SizeLeft        (void) const
    {
        return (CountLeft()  * count_t::SMake(sizeof(value_type))).template ValueAs<size_byte_t>();
    }

    template<typename V>
    constexpr V                                 SizeLeftV       (void) const
    {
        return SizeLeft().template ValueAs<V>();
    }

    constexpr void                              OffsetAdd       (const count_t aOffset)
    {
        const count_t countLeft = CountLeft();
        THROW_GPE_COND_CHECK_M(aOffset <= countLeft, "Out of range"_sv);

        iOffset += aOffset;
        iPtr    += aOffset.ValueAs<size_t>();
    }

    constexpr void                              OffsetSub       (const count_t aOffset)
    {
        THROW_GPE_COND_CHECK_M(aOffset <= iOffset, "Out of range"_sv);

        iOffset -= aOffset;
        iPtr    -= aOffset.ValueAs<size_t>();
    }

    constexpr void                              OffsetZero      (void) noexcept
    {
        iOffset = 0_cnt;
    }

    constexpr void                              CopyFrom        (const this_type& aRawPtr)
    {
        const count_t countLeftThis     = CountLeft();
        const count_t countLeftOther    = aRawPtr.CountLeft();
        THROW_GPE_COND_CHECK_M(countLeftThis >= countLeftOther, "Out of range"_sv);
        MemOps::SCopy(Ptr(), aRawPtr.Ptr(), countLeftOther);
    }

    constexpr void                              CopyFrom        (const value_type* aPtr, const count_t aCount)
    {
        const count_t countLeftThis = CountLeft();
        THROW_GPE_COND_CHECK_M(countLeftThis >= aCount, "Out of range"_sv);
        MemOps::SCopy(Ptr(), aPtr, aCount);
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<P, const value_type*>, P>>
    constexpr   void                            CopyFrom        (P aPtr, const size_t aSize)
    {
        CopyFrom(SPtrAs<P, const value_type*>(aPtr),
                 SCountAs<P, const value_type*>(count_t::SMake(aSize)));
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<P, const value_type*>, P>>
    constexpr   void                            CopyFrom        (P aPtr, const count_t aSize)
    {
        CopyFrom(SPtrAs<P, const value_type*>(aPtr),
                 SCountAs<P, const value_type*>(aSize));
    }

    template<typename R, typename = std::enable_if_t<is_convertable_raw_v<R, GpRawPtr<const value_type*>>, R>>
    constexpr   void                            CopyFrom        (const R& aRawPtr)
    {
        CopyFrom(aRawPtr.template _PtrAs<const value_type*>(),
                 aRawPtr.template CountLeftAs<const value_type*>());
    }

    constexpr this_type                         Subrange            (const count_t aOffset, const count_t aCount) const
    {
        return SubrangeAs<this_type>(aOffset, aCount);
    }

    template<typename R, typename = std::enable_if_t<is_convertable_raw_v<this_type, R>, R>>
    constexpr R                                 SubrangeAs          (const count_t aOffset, const count_t aCount) const
    {
        const count_t countLeft = CountLeft();
        THROW_GPE_COND_CHECK_M((aOffset + aCount) <= countLeft, "Out of range"_sv);
        return R(_PtrAs<typename R::pointer_type>(aOffset),
                 SCountAs<pointer_type, typename R::pointer_type>(aCount));
    }

    constexpr this_type                         SubrangeBeginOffset (const count_t aOffset) const
    {
        return SubrangeAs<this_type>(aOffset, CountLeft() - aOffset);
    }

    template<typename R, typename = std::enable_if_t<is_convertable_raw_v<this_type, R>, R>>
    constexpr R                                 SubrangeBeginOffsetAs (const count_t aOffset) const
    {
        return SubrangeAs<R>(aOffset, CountLeft() - aOffset);
    }

    template<typename R, typename = std::enable_if_t<is_convertable_raw_v<this_type, R>, R>>
    constexpr R                                 As                  (void) const
    {
        return SubrangeAs<R>(0_cnt, CountLeft());
    }

    template<typename R, typename = std::enable_if_t<is_reinterpretable_raw_v<this_type, R>, R>>
    constexpr R                                 ReinterpretAs       (void) const
    {
        const value_type*   ptr     = _Ptr();
        const size_t        addr    = std::bit_cast<size_t>(ptr);

        THROW_GPE_COND_CHECK_M((addr % alignof(typename R::value_type)) == 0, "Wrong memory align"_sv);

        const size_t countLeft = CountLeft().template ValueAs<size_t>();

        THROW_GPE_COND_CHECK_M((countLeft % sizeof(typename R::value_type)) == 0, "Wrong elements count"_sv);

        return R(reinterpret_cast<typename R::pointer_type>(_Ptr()),
                 countLeft / sizeof(typename R::value_type));
    }

    constexpr const value_type*                 Ptr                 (void) const
    {
        return _Ptr();
    }

    constexpr const value_type*                 Ptr                 (const count_t aOffset) const
    {
        return _Ptr(aOffset);
    }

    template <typename _D = void, typename = std::enable_if_t<!is_const_v<T>, _D>>
    constexpr value_type*                       Ptr                 (void)
    {
        return const_cast<value_type*>(std::as_const(*this).Ptr());
    }

    template <typename _D = void, typename = std::enable_if_t<!is_const_v<T>, _D>>
    constexpr value_type*                       Ptr                 (const count_t aOffset)
    {
        return const_cast<value_type*>(std::as_const(*this).Ptr(aOffset));
    }

    constexpr const value_type*                 PtrBegin            (void) const noexcept
    {
        return _PtrBegin();
    }

    constexpr const value_type*                 PtrBegin            (const count_t aOffset) const
    {
        return _PtrBegin(aOffset);
    }

    template <typename _D = void, typename = std::enable_if_t<!is_const_v<T>, _D>>
    constexpr value_type*                       PtrBegin            (void) noexcept
    {
        return const_cast<value_type*>(std::as_const(*this).PtrBegin());
    }

    template <typename _D = void, typename = std::enable_if_t<!is_const_v<T>, _D>>
    constexpr value_type*                       PtrBegin            (const count_t aOffset)
    {
        return const_cast<value_type*>(std::as_const(*this).PtrBegin(aOffset));
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 PtrAs               (void) const
    {
        return _PtrAs<P>();
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 PtrAs               (const count_t aOffset) const
    {
        return _PtrAs<P>(aOffset);
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 PtrAs               (void)
    {
        using P_const_ptr_t = const std::remove_cvref_t<std::remove_pointer_t<P>>*;
        return const_cast<P>(std::as_const(*this).template PtrAs<P_const_ptr_t>());
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 PtrAs               (const count_t aOffset)
    {
        using P_const_ptr_t = const std::remove_cvref_t<std::remove_pointer_t<P>>*;
        return const_cast<P>(std::as_const(*this).template PtrAs<P_const_ptr_t>(aOffset));
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 PtrBeginAs          (void) const noexcept
    {
        return _PtrBeginAs<P>();
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 PtrBeginAs          (const count_t aOffset) const noexcept
    {
        return _PtrBeginAs<P>(aOffset);
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 PtrBeginAs          (void) noexcept
    {
        using P_const_ptr_t = const std::remove_cvref_t<std::remove_pointer_t<P>>*;
        return const_cast<P>(std::as_const(*this).template PtrBeginAs<P_const_ptr_t>());
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 PtrBeginAs          (const count_t aOffset) noexcept
    {
        using P_const_ptr_t = const std::remove_cvref_t<std::remove_pointer_t<P>>*;
        return const_cast<P>(std::as_const(*this).template PtrBeginAs<P_const_ptr_t>(aOffset));
    }

    constexpr pointer_type                      _Ptr            (void) const
    {
        return _Ptr(0_cnt);
    }

    constexpr pointer_type                      _Ptr            (const count_t aOffset) const
    {
        const count_t countLeft = CountLeft();
        THROW_GPE_COND_CHECK_M(aOffset < countLeft, "Out of range"_sv);
        return _PtrBegin() + aOffset.ValueAs<size_t>();
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 _PtrAs          (void) const
    {
        return SPtrAs<pointer_type, P>(_Ptr());
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 _PtrAs          (const count_t aOffset) const
    {
        return SPtrAs<pointer_type, P>(_Ptr(aOffset));
    }

    constexpr pointer_type                      _PtrBegin       (void) const noexcept
    {
        return iPtr;
    }

    constexpr pointer_type                      _PtrBegin       (const count_t aOffset) const
    {
        const count_t countTotal = CountTotal();
        THROW_GPE_COND_CHECK_M(aOffset < countTotal, "Out of range"_sv);
        return _PtrBegin() + aOffset.ValueAs<size_t>();
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 _PtrBeginAs     (void) const noexcept
    {
        return SPtrAs<pointer_type, P>(_PtrBegin());
    }

    template<typename P, typename = std::enable_if_t<is_convertable_ptr_v<pointer_type, P>, P>>
    constexpr P                                 _PtrBeginAs     (const count_t aOffset) const
    {
        return SPtrAs<pointer_type, P>(_PtrBegin(aOffset));
    }

    template<typename FROM, typename TO, typename = std::enable_if_t<is_convertable_ptr_v<FROM, TO>, FROM>>
    static constexpr TO                         SPtrAs          (FROM aFrom) noexcept
    {
        return reinterpret_cast<TO>(aFrom);
    }

    template<typename FROM, typename TO, typename = std::enable_if_t<is_countable_ptr_v<FROM, TO>, FROM>>
    static constexpr count_t                    SCountAs        (const count_t aValue)
    {
        if constexpr (sizeof(value_type_t<TO>) == 1)
        {
            return aValue * count_t::SMake(sizeof(value_type_t<FROM>));
        } else
        {
            return aValue;
        }
    }

    constexpr const value_type&     At                  (const count_t aOffset) const
    {
        return *Ptr(aOffset);
    }

    constexpr value_type&           At                  (const count_t aOffset)
    {
        return *const_cast<value_type*>(std::as_const(*this).template Ptr(aOffset));
    }

    constexpr const value_type&     operator[]          (const count_t aOffset) const
    {
        return *Ptr(aOffset);
    }

    constexpr value_type&           operator[]          (const count_t aOffset)
    {
        return *const_cast<value_type*>(std::as_const(*this).template Ptr(aOffset));
    }

    constexpr const value_type&     operator*           (void) const
    {
        return *Ptr();
    }

    constexpr value_type&           operator*           (void)
    {
        return *const_cast<value_type*>(std::as_const(*this).template Ptr());
    }

    constexpr this_type&            operator++          (void)
    {
        OffsetAdd(1_cnt);
        return *this;
    }

    constexpr this_type             operator++          (int)
    {
        this_type res(*this);
        OffsetAdd(1_cnt);
        return res;
    }

    constexpr this_type&            operator--          (void)
    {
        OffsetSub(1_cnt);
        return *this;
    }

    constexpr this_type             operator--          (int)
    {
        this_type res(*this);
        OffsetSub(1_cnt);
        return res;
    }

    constexpr this_type&            operator+=          (const count_t aOffset)
    {
        OffsetAdd(aOffset);
        return *this;
    }

    constexpr this_type&            operator-=          (const count_t aOffset)
    {
        OffsetSub(aOffset);
        return *this;
    }

    constexpr bool                  operator==          (const this_type& aRawPtr) const noexcept
    {
        return IsEqual(aRawPtr);
    }

    constexpr bool                  operator!=          (const this_type& aRawPtr) const noexcept
    {
        return !IsEqual(aRawPtr);
    }

    constexpr bool                  IsEqual             (const this_type& aRawPtr) const noexcept
    {
        return    (CountLeft() == aRawPtr.CountLeft())
               && (MemOps::SCompare(Ptr(), aRawPtr.Ptr(), CountLeft()) == 0);
    }

    constexpr bool                  IsEqualByMinLen     (const this_type& aRawPtr) const noexcept
    {
        const count_t count = std::min(CountLeft(), aRawPtr.CountLeft());
        return MemOps::SCompare(Ptr(), aRawPtr.Ptr(), count) == 0;
    }

    constexpr bool                  IsEqualByThisLen    (const this_type& aRawPtr) const noexcept
    {
        const count_t thisCount = CountLeft();

        return    (aRawPtr.CountLeft() >= thisCount)
               && (MemOps::SCompare(Ptr(), aRawPtr.Ptr(), thisCount) == 0);
    }

    constexpr bool                  IsEqualByArgLen     (const this_type& aRawPtr) const noexcept
    {
        const count_t argCount = aRawPtr.CountLeft();

        return    (CountLeft() >= argCount)
               && (MemOps::SCompare(Ptr(), aRawPtr.Ptr(), argCount) == 0);
    }

    constexpr bool                  IsEmpty             (void) const noexcept
    {
        return (iPtr == nullptr) || (CountLeft() == 0_cnt);
    }

    constexpr std::string_view      AsStringView        (void) const
    {
        return std::string_view(PtrAs<const char*>(),
                                CountLeftV<size_t>());
    }

    std::vector<std::byte>          ToByteArray     (void) const
    {
        std::vector<std::byte> res;

        const size_t size = CountLeftV<size_t>();
        if (size > 0)
        {
            res.resize(CountLeftV<size_t>());

            GpRawPtr<std::byte*> resPtr(res);
            resPtr.CopyFrom(*this);
        }

        return res;
    }

protected:
    pointer_type    iPtr    = nullptr;
    count_t         iCount  = 0_cnt;
    count_t         iOffset = 0_cnt;
};

}//GPlatform
