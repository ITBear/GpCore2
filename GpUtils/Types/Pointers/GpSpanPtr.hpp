#pragma once

#include "../../Macro/GpMacroTags.hpp"
#include "../../Exceptions/GpException.hpp"
#include "../../GpMemOps.hpp"
#include "../Units/Other/size_byte_t.hpp"
#include "../Numerics/GpNumericTypes.hpp"
#include "../Strings/GpStringLiterals.hpp"

#include <iterator>
#include <vector>
#include <span>

namespace GPlatform {

TAG_REGISTER(GpSpanPtr)

namespace SpanPtrConcepts {

template <typename FROM, typename TO>
concept IsConvertableFromPtr = requires()
{
   /* CAST:
    * FROM type*        -> TO byte*
    * FROM type*        -> TO char*
    * FROM type*        -> TO unsigned char*
    * FROM type*        -> TO char8_t*
    * FROM type*        -> TO const byte*
    * FROM type*        -> TO const char*
    * FROM type*        -> TO const unsigned char*
    * FROM type*        -> TO const char8_t*
    * FROM type*        -> TO type*
    * FROM type*        -> TO const type*
    * FROM const type*  -> TO const byte*
    * FROM const type*  -> TO const char*
    * FROM const type*  -> TO const unsigned char*
    * FROM const type*  -> TO const char8_t*
    * FROM const type*  -> TO const type*
   */

    requires
       std::is_pointer_v<FROM>
    && (
           (std::is_same_v<FROM, TO>)//Same types
        || (   std::is_const_v<std::remove_pointer_t<TO>>   //Same types if remove cv, but TO is const
            && std::is_same_v<std::remove_cvref_t<std::remove_pointer_t<FROM>>, std::remove_cvref_t<std::remove_pointer_t<TO>>>)
        || (   std::is_same_v<TO, const char*>              //TO is one byte type (const)
            || std::is_same_v<TO, const unsigned char*>
            || std::is_same_v<TO, const std::byte*>
            || std::is_same_v<TO, const u_int_8*>
            || std::is_same_v<TO, const char8_t*>)
        || (   !std::is_const_v<std::remove_pointer_t<FROM>>//From is not const and TO is one byte type
            && (   std::is_same_v<TO, char*>                //TO is one byte type (const)
                || std::is_same_v<TO, unsigned char*>
                || std::is_same_v<TO, std::byte*>
                || std::is_same_v<TO, u_int_8*>
                || std::is_same_v<TO, char8_t*>)
           )
    );
};

template <typename FROM, typename TO>
concept IsConvertableFromSpan = requires()
{
    typename FROM::pointer;
    typename TO::pointer;

    requires
           IsConvertableFromPtr<typename FROM::pointer, typename TO::pointer>
        && GpHasTag_GpSpanPtr<FROM>()
        && GpHasTag_GpSpanPtr<TO>();
};

template <typename FROM, typename TO>
concept IsConvertableFromContainer = requires(FROM f)
{
    f.data();

    requires
        ::GPlatform::Concepts::HasRandomAccessIter<FROM>
        && IsConvertableFromPtr<decltype(std::declval<FROM>().data()), TO>;
};

}//namespace SpanPtrConcepts

template<Concepts::IsPointer T>
class GpSpanPtr
{
public:
    TAG_SET(GpSpanPtr)

    static_assert(std::is_pointer_v<T>, "T must be pointer to value");

    using pointer       = T;
    using value_type    = std::remove_pointer_t<pointer>;
    using this_type     = GpSpanPtr<T>;

    template<typename _T>
    using PureT = std::remove_cvref_t<std::remove_pointer_t<_T>>;

    static constexpr size_byte_t value_size_v = size_byte_t::SMake(sizeof(PureT<T>));

public:
    constexpr                                   GpSpanPtr       (void) noexcept = delete;
    constexpr                                   GpSpanPtr       (const this_type& aSpan) noexcept;
    constexpr                                   GpSpanPtr       (this_type&& aSpan) noexcept;
    constexpr                                   GpSpanPtr       (pointer        aPtr,
                                                                 const size_t   aCount);

    template<typename PtrT>
    requires SpanPtrConcepts::IsConvertableFromPtr<PtrT, pointer>
    constexpr                                   GpSpanPtr       (PtrT           aPtr,
                                                                 const size_t   aCount):
                                                                GpSpanPtr
                                                                (
                                                                    _SPtrAs<pointer>(aPtr),
                                                                    _SCountAs<pointer, PtrT>(aCount)
                                                                ){}

    template<typename SpanT>
    requires SpanPtrConcepts::IsConvertableFromSpan<SpanT, this_type>
    constexpr                                   GpSpanPtr       (const SpanT& aSpan) noexcept:
                                                                GpSpanPtr
                                                                (
                                                                    _SPtrAs<pointer>(aSpan.Ptr()),
                                                                    _SCountAs<pointer, typename SpanT::pointer>(aSpan.Count())
                                                                ){}

    template<typename ContainerT>
    requires SpanPtrConcepts::IsConvertableFromContainer<ContainerT, pointer>
    constexpr                                   GpSpanPtr       (ContainerT&& aContainer):
                                                                GpSpanPtr
                                                                (
                                                                    _SPtrAs<pointer>(aContainer.data()),
                                                                    _SCountAs<pointer, decltype(std::declval<ContainerT>().data())>(aContainer.size())
                                                                ){}

    template<typename ContainerT>
    requires SpanPtrConcepts::IsConvertableFromContainer<ContainerT, pointer>
    constexpr                                   GpSpanPtr       (ContainerT& aContainer):
                                                                GpSpanPtr
                                                                (
                                                                    _SPtrAs<pointer>(aContainer.data()),
                                                                    _SCountAs<pointer, decltype(std::declval<ContainerT>().data())>(aContainer.size())
                                                                ){}

    constexpr                                   ~GpSpanPtr      (void) noexcept = default;


    constexpr void                              Clear           (void) noexcept;
    constexpr bool                              IsEmpty         (void) const noexcept;
    constexpr bool                              IsEqual         (const this_type& aSpan) const noexcept;
    constexpr bool                              IsNotEqual      (const this_type& aSpan) const noexcept;
    constexpr size_t                            Compare         (const this_type& aSpan) const noexcept;

    constexpr size_t                            Count           (void) const noexcept;
    constexpr size_byte_t                       Size            (void) const;

    constexpr void                              Set             (const this_type& aSpan) noexcept;
    constexpr void                              Set             (this_type&& aSpan) noexcept;
    constexpr void                              Set             (pointer aPtr, const size_t aCount);

    template<typename PtrT>
    requires SpanPtrConcepts::IsConvertableFromPtr<PtrT, pointer>
    constexpr void                              Set             (PtrT           aPtr,
                                                                 const size_t   aCount)
    {
        Set
        (
            _SPtrAs<pointer>(aPtr),
            _SCountAs<pointer, PtrT>(aCount)
        );
    }

    template<typename SpanT>
    requires SpanPtrConcepts::IsConvertableFromSpan<SpanT, this_type>
    constexpr void                              Set             (const SpanT& aSpan) noexcept
    {
        Set
        (
            _SPtrAs<pointer>(aSpan.Ptr()),
            _SCountAs<pointer, typename SpanT::pointer>(aSpan.Count())
        );
    }

    template<typename ContainerT>
    requires SpanPtrConcepts::IsConvertableFromContainer<ContainerT, pointer>
    constexpr void                              Set             (ContainerT&& aContainer)
    {
        Set
        (
            _SPtrAs<pointer>(aContainer.data()),
            _SCountAs<pointer, decltype(std::declval<ContainerT>().data())>(aContainer.size())
        );
    }

    constexpr this_type&                        operator=       (const this_type& aSpan) noexcept;
    constexpr this_type&                        operator=       (this_type&& aSpan) noexcept;

    template<typename SpanT>
    requires SpanPtrConcepts::IsConvertableFromSpan<SpanT, this_type>
    constexpr this_type&                        operator=       (const SpanT& aSpan)
    {
        Set(aSpan);
        return *this;
    }

    template<typename ContainerT>
    requires SpanPtrConcepts::IsConvertableFromContainer<ContainerT, pointer>
    constexpr this_type&                        operator=       (ContainerT&& aContainer)
    {
        Set(std::forward(aContainer));
        return *this;
    }

    constexpr value_type&                       operator[]      (const size_t aOffset) const;
    constexpr value_type&                       operator*       (void) const;

    constexpr this_type&                        operator++      (void);
    constexpr this_type                         operator++      (int);

    constexpr this_type&                        operator+=      (const size_t aOffset);

    constexpr bool                              operator==      (const this_type& aSpan) const noexcept;
    constexpr bool                              operator!=      (const this_type& aSpan) const noexcept;

    constexpr pointer                           Ptr             (void) const;
    constexpr pointer                           Ptr             (const size_t aOffset) const;

    template<typename PtrT>
    requires SpanPtrConcepts::IsConvertableFromPtr<pointer, PtrT>
    constexpr PtrT                              PtrAs           (void) const
    {
        return PtrAs<PtrT>(0);
    }

    template<typename PtrT>
    requires SpanPtrConcepts::IsConvertableFromPtr<pointer, PtrT>
    constexpr PtrT                              PtrAs           (const size_t aOffset) const
    {
        return _SPtrAs<PtrT>(Ptr(aOffset));
    }

    constexpr value_type&                       At              (const size_t aOffset) const;
    constexpr this_type&                        OffsetAdd       (const size_t aOffset);
    constexpr this_type                         SubspanBegin    (const size_t aOffset,
                                                                 const size_t aCount) const;

    template<typename SpanT>
    requires SpanPtrConcepts::IsConvertableFromSpan<this_type, SpanT>
    constexpr SpanT                             SubspanBeginAs  (const size_t aOffset,
                                                                 const size_t aCount) const
    {
        const auto cnt = Count();

        THROW_COND_GP
        (
            NumOps::SAdd(aOffset, aCount) <= cnt,
            u8"Out of range"_sv
        );

        return SpanT
        (
            _SPtrAs<typename SpanT::pointer>(iPtr + aOffset),
            _SCountAs<typename SpanT::pointer, pointer>(aCount)
        );
    }

    constexpr this_type                         SubspanEnd      (const size_t aOffset,
                                                                 const size_t aCount) const;

    template<typename SpanT>
    requires SpanPtrConcepts::IsConvertableFromSpan<this_type, SpanT>
    constexpr SpanT                             SubspanEndAs    (const size_t aOffset,
                                                                 const size_t aCount) const
    {
        const auto cnt = Count();

        THROW_COND_GP
        (
            NumOps::SAdd(aOffset, aCount) <= cnt,
            u8"Out of range"_sv
        );

        return SpanT
        (
            _SPtrAs<typename SpanT::pointer>(iPtr + (cnt - aOffset - 1)),
            _SCountAs<typename SpanT::pointer, pointer>(aCount)
        );
    }   

    template<typename SpanT>
    requires SpanPtrConcepts::IsConvertableFromSpan<this_type, SpanT>
    constexpr SpanT                             As              (void) const
    {
        return SubspanBeginAs<SpanT>(0, Count());
    }

    template<typename SpanT>
    SpanT                                       ReinterpretAs       (void) const
    {
        return SpanT
        (
            reinterpret_cast<typename SpanT::pointer>(Ptr()),
            _SCountAs<typename SpanT::pointer, pointer>(Count())
        );
    }

    constexpr std::u8string_view                AsStringViewU8      (void) const;
    std::span<value_type>                       AsStdSpan           (void) const;
    std::span<u_int_8>                          AsStdSpanByte       (void) const;
    std::span<const u_int_8>                    AsStdSpanConstByte  (void) const;

    std::vector<u_int_8>                        ToByteArray         (void) const;

    template<typename SpanT>
    this_type&                                  CopyFrom            (const SpanT& aSpan);

    //for static_cast<std::vector<u_int_8>>
    operator                                    std::vector<u_int_8>() const    {return ToByteArray();}

private:
    template<typename PtrToT, typename PtrFromT>
    static constexpr PtrToT                     _SPtrAs         (PtrFromT aFrom) noexcept;

    template<typename PtrToT, typename PtrFromT>
    static constexpr size_t                     _SCountAs       (const size_t aValue) noexcept;

    void                                        _CheckPointers  (pointer        aPtr,
                                                                 const size_t   aCount) const;

protected:
    pointer     iPtr    = nullptr;
    size_t      iCount  = 0;
};

template<Concepts::IsPointer T>
constexpr   GpSpanPtr<T>::GpSpanPtr (const this_type& aSpan) noexcept:
iPtr(aSpan.iPtr),
iCount(aSpan.iCount)
{
}

template<Concepts::IsPointer T>
constexpr   GpSpanPtr<T>::GpSpanPtr (this_type&& aSpan) noexcept:
iPtr(std::move(aSpan.iPtr)),
iCount(std::move(aSpan.iCount))
{
}

template<Concepts::IsPointer T>
constexpr   GpSpanPtr<T>::GpSpanPtr
(
    pointer         aPtr,
    const size_t    aCount
):
iPtr(aPtr),
iCount(aCount)
{
    _CheckPointers(iPtr, iCount);
}

template<Concepts::IsPointer T>
constexpr void  GpSpanPtr<T>::Clear (void) noexcept
{
    iPtr    = nullptr;
    iCount  = 0;
}

template<Concepts::IsPointer T>
constexpr bool  GpSpanPtr<T>::IsEmpty (void) const noexcept
{
    return (iPtr == nullptr) || (iCount == 0);
}

template<Concepts::IsPointer T>
constexpr bool  GpSpanPtr<T>::IsEqual (const this_type& aSpan) const noexcept
{
    return
           (Count() == aSpan.Count())
        && (MemOps::SCompare(Ptr(), aSpan.Ptr(), Count()) == 0);
}

template<Concepts::IsPointer T>
constexpr bool  GpSpanPtr<T>::IsNotEqual (const this_type& aSpan) const noexcept
{
    return
           (Count() != aSpan.Count())
        || (MemOps::SCompare(Ptr(), aSpan.Ptr(), Count()) != 0);
}

template<Concepts::IsPointer T>
constexpr size_t    GpSpanPtr<T>::Compare (const this_type& aSpan) const noexcept
{
    THROW_COND_GP
    (
        Count() == aSpan.Count(),
        u8"Span elements count is not equal"_sv
    );

    return MemOps::SCompare(Ptr(), aSpan.Ptr(), Count());
}

template<Concepts::IsPointer T>
constexpr size_t    GpSpanPtr<T>::Count (void) const noexcept
{
    return iCount;
}

template<Concepts::IsPointer T>
constexpr size_byte_t   GpSpanPtr<T>::Size (void) const
{
    return size_byte_t::SMake(Count()) * value_size_v;
}

template<Concepts::IsPointer T>
constexpr void  GpSpanPtr<T>::Set (const this_type& aSpan) noexcept
{
    iPtr    = aSpan.iPtr;
    iCount  = aSpan.iCount;
}

template<Concepts::IsPointer T>
constexpr void  GpSpanPtr<T>::Set (this_type&& aSpan) noexcept
{
    iPtr    = std::move(aSpan.iPtr);
    iCount  = std::move(aSpan.iCount);
}

template<Concepts::IsPointer T>
constexpr void  GpSpanPtr<T>::Set (pointer aPtr, const size_t aCount)
{
    _CheckPointers(aPtr, aCount);

    iPtr    = aPtr;
    iCount  = aCount;
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::this_type& GpSpanPtr<T>::operator= (const this_type& aSpan) noexcept
{
    Set(aSpan);
    return *this;
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::this_type& GpSpanPtr<T>::operator= (this_type&& aSpan) noexcept
{
    Set(std::move(aSpan));
    return *this;
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::value_type&    GpSpanPtr<T>::operator[] (const size_t aOffset) const
{
    return At(aOffset);
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::value_type&    GpSpanPtr<T>::operator* (void) const
{
    return At(0);
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::this_type& GpSpanPtr<T>::operator++ (void)
{
    return OffsetAdd(1);
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::this_type  GpSpanPtr<T>::operator++ (int)
{
    this_type res(*this);
    OffsetAdd(1);
    return res;
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::this_type& GpSpanPtr<T>::operator+= (const size_t aOffset)
{
    return OffsetAdd(aOffset);
}

template<Concepts::IsPointer T>
constexpr bool  GpSpanPtr<T>::operator== (const this_type& aSpan) const noexcept
{
    return IsEqual(aSpan);
}

template<Concepts::IsPointer T>
constexpr bool  GpSpanPtr<T>::operator!= (const this_type& aSpan) const noexcept
{
    return IsNotEqual(aSpan);
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::pointer    GpSpanPtr<T>::Ptr (void) const
{
    return iPtr;
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::pointer    GpSpanPtr<T>::Ptr (const size_t aOffset) const
{
    THROW_COND_GP
    (
        aOffset <= Count(),
        u8"Out of range"_sv
    );

    return iPtr + aOffset;
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::value_type&    GpSpanPtr<T>::At (const size_t aOffset) const
{
    return *Ptr(aOffset);
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::this_type& GpSpanPtr<T>::OffsetAdd (const size_t aOffset)
{
    THROW_COND_GP
    (
        aOffset <= Count(),
        u8"Out of range"_sv
    );

    iCount  -= aOffset;
    iPtr    += aOffset;

    return *this;
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::this_type  GpSpanPtr<T>::SubspanBegin
(
    const size_t aOffset,
    const size_t aCount
) const
{
    return SubspanBeginAs<this_type>(aOffset, aCount);
}

template<Concepts::IsPointer T>
constexpr typename GpSpanPtr<T>::this_type  GpSpanPtr<T>::SubspanEnd
(
    const size_t aOffset,
    const size_t aCount
) const
{
    return SubspanEndAs<this_type>(aOffset, aCount);
}

template<Concepts::IsPointer T>
constexpr std::u8string_view    GpSpanPtr<T>::AsStringViewU8 (void) const
{
    const size_byte_t size = Size();
    return std::u8string_view(PtrAs<const char8_t*>(), size.As<size_t>());
}

#if (__cplusplus >= CPP_VERSION_20)

template<Concepts::IsPointer T>
std::span<typename GpSpanPtr<T>::value_type>    GpSpanPtr<T>::AsStdSpan (void) const
{
    return std::span<value_type>(Ptr(), Count());
}

template<Concepts::IsPointer T>
std::span<u_int_8>  GpSpanPtr<T>::AsStdSpanByte (void) const
{
    const size_byte_t size = Size();
    return std::span<u_int_8>(PtrAs<u_int_8*>(), size.As<size_t>());
}

template<Concepts::IsPointer T>
std::span<const u_int_8>    GpSpanPtr<T>::AsStdSpanConstByte (void) const
{
    const size_byte_t size = Size();
    return std::span<const u_int_8>(PtrAs<const u_int_8*>(), size.As<size_t>());
}

#endif//#if (__cplusplus >= CPP_VERSION_20)

template<Concepts::IsPointer T>
std::vector<u_int_8>        GpSpanPtr<T>::ToByteArray (void) const
{
    std::vector<u_int_8> res;

    const size_byte_t size = Size();

    if (size > 0_byte)
    {
        res.resize(size.As<size_t>());
        std::memcpy(res.data(), PtrAs<const u_int_8*>(), size.As<size_t>());
    }

    return res;
}

template<Concepts::IsPointer T>
template<typename SpanT>
typename GpSpanPtr<T>::this_type&   GpSpanPtr<T>::CopyFrom (const SpanT& aSpan)
{
    THROW_COND_GP
    (
        Count() >= aSpan.Count(),
        u8"Out of range"_sv
    );

    MemOps::SCopy
    (
        Ptr(),
        aSpan.template PtrAs<const value_type*>(),
        _SCountAs<const value_type*, typename SpanT::pointer>(aSpan.Count())
    );

    return *this;
}

template<Concepts::IsPointer T>
template<typename PtrToT, typename PtrFromT>
constexpr PtrToT    GpSpanPtr<T>::_SPtrAs (PtrFromT aFrom) noexcept
{
    return reinterpret_cast<PtrToT>(aFrom);
}

template<Concepts::IsPointer T>
template<typename PtrToT, typename PtrFromT>
constexpr size_t    GpSpanPtr<T>::_SCountAs (const size_t aValue) noexcept
{
    constexpr const size_t sizeOfTo     = sizeof(PureT<PtrToT>);
    constexpr const size_t sizeOfFrom   = sizeof(PureT<PtrFromT>);

    if constexpr (sizeOfTo == 1)
    {
        return NumOps::SMul(aValue, sizeOfFrom);
    } else if constexpr (sizeOfFrom == sizeOfTo)
    {
        return aValue;
    } else if constexpr (sizeOfTo < sizeOfFrom)
    {
        if constexpr ((sizeOfFrom % sizeOfTo) != 0)
        {
            GpThrowCe<GpException>(u8"GpSpanPtr<T>::_SCountAs wrong types");
        }

        return NumOps::SMul(aValue, sizeOfFrom / sizeOfTo);
    } else if constexpr (sizeOfTo > sizeOfFrom)
    {
        if constexpr ((sizeOfTo % sizeOfFrom) != 0)
        {
            GpThrowCe<GpException>(u8"GpSpanPtr<T>::_SCountAs wrong types");
        }

        return NumOps::SDiv(aValue, sizeOfTo / sizeOfFrom);
    }

    GpThrowCe<GpException>(u8"GpSpanPtr<T>::_SCountAs wrong types");
}

template<Concepts::IsPointer T>
void    GpSpanPtr<T>::_CheckPointers
(
    pointer         aPtr,
    const size_t    aCount
) const
{
    THROW_COND_GP
    (
           (aPtr != nullptr)
        || (
             (aPtr == nullptr) && (aCount == 0)
           ),
        u8"(nullptr and not empty) or (not nullptr and empty)"_sv
    );
}

}//GPlatform
