#pragma once

#include <GpCore2/GpUtils/Macro/GpMacroTags.hpp>
#include <GpCore2/GpUtils/GpMemOps.hpp>
#include <GpCore2/GpUtils/Exceptions/GpException.hpp>

#include <iterator>
#include <vector>
#include <span>

namespace GPlatform {

TAG_REGISTER(GpSpan)

namespace SpanConcepts {

template <typename FROM, typename TO>
concept IsConvertableFromPtr = requires()
{
   /* CAST:
    * FROM type*        -> TO byte*
    * FROM type*        -> TO char*
    * FROM type*        -> TO unsigned char*
    * FROM type*        -> TO const byte*
    * FROM type*        -> TO const char*
    * FROM type*        -> TO const unsigned char*
    * FROM type*        -> TO type*
    * FROM type*        -> TO const type*
    * FROM const type*  -> TO const byte*
    * FROM const type*  -> TO const char*
    * FROM const type*  -> TO const unsigned char*
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
            || std::is_same_v<TO, const u_int_8*>)
        || (   !std::is_const_v<std::remove_pointer_t<FROM>>//From is not const and TO is one byte type
            && (   std::is_same_v<TO, char*>                //TO is one byte type (const)
                || std::is_same_v<TO, unsigned char*>
                || std::is_same_v<TO, std::byte*>
                || std::is_same_v<TO, u_int_8*>)
           )
    );
};

template <typename FROM, typename TO>
concept IsConvertableFromSpan = requires()
{
    typename FROM::pointer;
    typename TO::pointer;

    requires
           GpHasTag_GpSpan<FROM>()
        && GpHasTag_GpSpan<TO>()
        && IsConvertableFromPtr<typename FROM::pointer, typename TO::pointer>;
};

template <typename FROM, typename TO>
concept IsConvertableFromContainer = requires(FROM f)
{
    f.data();

    requires
        ::GPlatform::Concepts::HasContiguousIter<FROM>
        && IsConvertableFromPtr<decltype(std::declval<FROM>().data()), TO>;
};

}// namespace SpanConcepts

template<typename T>
class GpSpan
{
public:
    TAG_SET(GpSpan)

    using pointer       = T*;
    using value_type    = T;
    using this_type     = GpSpan<T>;

    template<typename _T>
    using PureT = std::remove_cvref_t<std::remove_pointer_t<_T>>;

    static constexpr size_t value_size_v = sizeof(PureT<T>);

    using CompareFnT    = std::function<bool(const value_type&, const value_type&)>;

public:
    constexpr                   GpSpan      (void) noexcept;
    constexpr                   GpSpan      (const this_type& aSpan) noexcept;
    constexpr                   GpSpan      (this_type&& aSpan) noexcept;
    constexpr                   GpSpan      (pointer    aPtr,
                                             size_t     aCount);

    template<typename PtrT>
    requires SpanConcepts::IsConvertableFromPtr<PtrT, pointer>
    constexpr                   GpSpan      (PtrT           aPtr,
                                             const size_t   aCount):
                                            GpSpan
                                            (
                                                _SPtrAs<pointer>(aPtr),
                                                _SCountAs<pointer, PtrT>(aCount)
                                            ){}

    template<typename SpanT>
    requires SpanConcepts::IsConvertableFromSpan<SpanT, this_type>
    constexpr                   GpSpan      (const SpanT& aSpan) noexcept:
                                            GpSpan
                                            (
                                                _SPtrAs<pointer>(aSpan.Ptr()),
                                                _SCountAs<pointer, typename SpanT::pointer>(aSpan.Count())
                                            ){}

    template<typename ContainerT>
    requires SpanConcepts::IsConvertableFromContainer<ContainerT, pointer>
    constexpr                   GpSpan      (ContainerT&& aContainer):
                                            GpSpan
                                            (
                                                _SPtrAs<pointer>(std::data(aContainer)),
                                                _SCountAs<pointer, decltype(std::declval<ContainerT>().data())>(std::size(aContainer))
                                            ){}

    template<typename ContainerT>
    requires SpanConcepts::IsConvertableFromContainer<ContainerT, pointer>
    constexpr                   GpSpan      (ContainerT& aContainer):
                                            GpSpan
                                            (
                                                _SPtrAs<pointer>(std::data(aContainer)),
                                                _SCountAs<pointer, decltype(std::declval<ContainerT>().data())>(std::size(aContainer))
                                            ){}

    constexpr                   ~GpSpan     (void) noexcept = default;


    constexpr void              Clear       (void) noexcept;
    constexpr bool              Empty       (void) const noexcept;
    constexpr bool              IsEqual     (const this_type& aSpan) const noexcept;
    constexpr bool              IsEqual     (const this_type&   aSpan,
                                             CompareFnT         aCompareFn) const noexcept;
    constexpr bool              IsNotEqual  (const this_type& aSpan) const noexcept;
    constexpr bool              IsGreater   (const this_type& aSpan) const noexcept;
    constexpr bool              IsLess      (const this_type& aSpan) const noexcept;

    constexpr size_t            Count       (void) const noexcept;
    constexpr size_t            SizeInBytes (void) const;

    constexpr void              Set         (const this_type& aSpan) noexcept;
    constexpr void              Set         (this_type&& aSpan) noexcept;
    constexpr void              Set         (pointer aPtr, size_t aCount);

    template<typename PtrT>
    requires SpanConcepts::IsConvertableFromPtr<PtrT, pointer>
    constexpr void              Set         (PtrT   aPtr,
                                             size_t aCount)
    {
        Set
        (
            _SPtrAs<pointer>(aPtr),
            _SCountAs<pointer, PtrT>(aCount)
        );
    }

    template<typename SpanT>
    requires SpanConcepts::IsConvertableFromSpan<SpanT, this_type>
    constexpr void              Set         (const SpanT& aSpan) noexcept
    {
        Set
        (
            _SPtrAs<pointer>(aSpan.Ptr()),
            _SCountAs<pointer, typename SpanT::pointer>(aSpan.Count())
        );
    }

    template<typename ContainerT>
    requires SpanConcepts::IsConvertableFromContainer<ContainerT, pointer>
    constexpr void              Set         (ContainerT&& aContainer)
    {
        Set
        (
            _SPtrAs<pointer>(std::data(aContainer)),
            _SCountAs<pointer, decltype(std::declval<ContainerT>().data())>(std::size(aContainer))
        );
    }

    constexpr this_type&        operator=   (const this_type& aSpan) noexcept;
    constexpr this_type&        operator=   (this_type&& aSpan) noexcept;

    template<typename SpanT>
    requires SpanConcepts::IsConvertableFromSpan<SpanT, this_type>
    constexpr this_type&        operator=   (const SpanT& aSpan)
    {
        Set(aSpan);
        return *this;
    }

    template<typename ContainerT>
    requires SpanConcepts::IsConvertableFromContainer<ContainerT, pointer>
    constexpr this_type&        operator=   (ContainerT&& aContainer)
    {
        Set(std::forward<ContainerT>(aContainer));
        return *this;
    }

    constexpr value_type&       operator[]  (size_t aOffset) const;
    constexpr value_type&       operator*   (void) const;

    constexpr this_type&        operator++  (void);
    constexpr this_type         operator++  (int);

    constexpr this_type&        operator+=  (size_t aOffset);

    constexpr bool              operator==  (const this_type& aSpan) const noexcept;
    constexpr bool              operator!=  (const this_type& aSpan) const noexcept;

    constexpr bool              operator>   (const this_type& aSpan) const noexcept;
    constexpr bool              operator<   (const this_type& aSpan) const noexcept;
    constexpr bool              operator>=  (const this_type& aSpan) const noexcept;
    constexpr bool              operator<=  (const this_type& aSpan) const noexcept;

    constexpr pointer           Ptr         (void) const;
    constexpr pointer           Ptr         (size_t aOffset) const;

    template<typename PtrT>
    requires SpanConcepts::IsConvertableFromPtr<pointer, PtrT>
    constexpr PtrT              PtrAs       (void) const
    {
        return PtrAs<PtrT>(0);
    }

    template<typename PtrT>
    requires SpanConcepts::IsConvertableFromPtr<pointer, PtrT>
    constexpr PtrT              PtrAs       (size_t aOffset) const
    {
        return _SPtrAs<PtrT>(Ptr(aOffset));
    }

    constexpr value_type&       At                  (size_t aOffset) const;
    constexpr this_type&        OffsetAdd           (size_t aOffset);
    constexpr this_type         Subspan             (size_t aOffset,
                                                     size_t aCount) const;
    constexpr this_type         SubspanThenOffsetAdd(size_t aCount);
    //constexpr this_type       Subspan             (size_t aOffset) const;

    template<typename SpanT>
    requires SpanConcepts::IsConvertableFromSpan<this_type, SpanT>
    constexpr SpanT             SubspanAs           (size_t aOffset,
                                                     size_t aCount) const
    {
        const auto cnt = Count();

        THROW_COND_GP
        (
            NumOps::SAdd(aOffset, aCount) <= cnt,
            "Out of range"_sv
        );

        return SpanT
        (
            _SPtrAs<typename SpanT::pointer>(iPtr + aOffset),
            _SCountAs<typename SpanT::pointer, pointer>(aCount)
        );
    }

    template<typename SpanT>
    requires SpanConcepts::IsConvertableFromSpan<this_type, SpanT>
    constexpr SpanT             As              (void) const
    {
        return SubspanAs<SpanT>(0, Count());
    }

    template<typename SpanT>
    SpanT                       ReinterpretAs       (void) const
    {
        return SpanT
        (
            reinterpret_cast<typename SpanT::pointer>(Ptr()),
            _SCountAs<typename SpanT::pointer, pointer>(Count())
        );
    }

    constexpr std::string_view  AsStringView        (void) const;
    std::span<value_type>       AsStdSpan           (void) const;
    std::span<std::byte>        AsStdSpanByte       (void) const;
    std::span<const std::byte>  AsStdSpanConstByte  (void) const;

    std::vector<std_byte_no_init>ToByteArray        (void) const;
    std::vector<u_int_8>        ToUI8Array          (void) const;

    template<typename SpanT>
    this_type&                  CopyFrom            (const SpanT& aSpan);

    //for static_cast<std::vector<std::byte>>
    operator                                    std::vector<std::byte>() const  {return ToByteArray();}

    //for static_cast<std::vector<u_int_8>>
    operator                                    std::vector<u_int_8>() const    {return ToUI8Array();}

private:
    template<typename PtrToT, typename PtrFromT>
    static constexpr PtrToT                     _SPtrAs         (PtrFromT aFrom) noexcept;

    template<typename PtrToT, typename PtrFromT>
    static constexpr size_t                     _SCountAs       (size_t aValue) noexcept;

    void                                        _CheckPointers  (pointer    aPtr,
                                                                 size_t     aCount) const;

protected:
    pointer     iPtr    = nullptr;
    size_t      iCount  = 0;
};

template<typename T>
constexpr   GpSpan<T>::GpSpan (void) noexcept:
iPtr  {nullptr},
iCount{0}
{
}

template<typename T>
constexpr   GpSpan<T>::GpSpan (const this_type& aSpan) noexcept:
iPtr  {aSpan.iPtr},
iCount{aSpan.iCount}
{
}

template<typename T>
constexpr   GpSpan<T>::GpSpan (this_type&& aSpan) noexcept:
iPtr  {std::move(aSpan.iPtr)},
iCount{std::move(aSpan.iCount)}
{
    aSpan.Clear();
}

template<typename T>
constexpr   GpSpan<T>::GpSpan
(
    pointer         aPtr,
    const size_t    aCount
):
iPtr  {aPtr},
iCount{aCount}
{
    _CheckPointers(iPtr, iCount);
}

template<typename T>
constexpr void  GpSpan<T>::Clear (void) noexcept
{
    iPtr    = nullptr;
    iCount  = 0;
}

template<typename T>
constexpr bool  GpSpan<T>::Empty (void) const noexcept
{
    return (iPtr == nullptr) || (iCount == 0);
}

template<typename T>
constexpr bool  GpSpan<T>::IsEqual (const this_type& aSpan) const noexcept
{
    return IsEqual
    (
        aSpan,
        [](const T& a, const T& b){return a == b;}
    );
}

template<typename T>
constexpr bool  GpSpan<T>::IsEqual
(
    const this_type&    aSpan,
    CompareFnT          aCompareFn
) const noexcept
{
    const size_t sizeThis   = Count();
    const size_t sizeOther  = aSpan.Count();

    if (sizeThis != sizeOther)
    {
        return false;
    }

    const auto* thisPtr     = Ptr();
    const auto* otherPtr    = aSpan.Ptr();

    for (size_t id = 0; id < sizeThis; id++)
    {
        if (aCompareFn(*thisPtr++, *otherPtr++) == false)
        {
            return false;
        }
    }

    return true;
}

template<typename T>
constexpr bool  GpSpan<T>::IsNotEqual (const this_type& aSpan) const noexcept
{
    return !IsEqual(aSpan);
}

template<typename T>
constexpr bool  GpSpan<T>::IsGreater (const this_type& aSpan) const noexcept
{
    return MemOps::SIsGreater(Ptr(), Count(), aSpan.Ptr(), aSpan.Count());
}

template<typename T>
constexpr bool  GpSpan<T>::IsLess (const this_type& aSpan) const noexcept
{
    return MemOps::SIsLess(Ptr(), Count(), aSpan.Ptr(), aSpan.Count());
}

template<typename T>
constexpr size_t    GpSpan<T>::Count (void) const noexcept
{
    return iCount;
}

template<typename T>
constexpr size_t    GpSpan<T>::SizeInBytes (void) const
{
    return Count() * value_size_v;
}

template<typename T>
constexpr void  GpSpan<T>::Set (const this_type& aSpan) noexcept
{
    iPtr    = aSpan.iPtr;
    iCount  = aSpan.iCount;
}

template<typename T>
constexpr void  GpSpan<T>::Set (this_type&& aSpan) noexcept
{
    iPtr    = std::move(aSpan.iPtr);
    iCount  = std::move(aSpan.iCount);
}

template<typename T>
constexpr void  GpSpan<T>::Set (pointer aPtr, const size_t aCount)
{
    _CheckPointers(aPtr, aCount);

    iPtr    = aPtr;
    iCount  = aCount;
}

template<typename T>
constexpr typename GpSpan<T>::this_type&    GpSpan<T>::operator= (const this_type& aSpan) noexcept
{
    Set(aSpan);
    return *this;
}

template<typename T>
constexpr typename GpSpan<T>::this_type&    GpSpan<T>::operator= (this_type&& aSpan) noexcept
{
    Set(std::move(aSpan));
    return *this;
}

template<typename T>
constexpr typename GpSpan<T>::value_type&   GpSpan<T>::operator[] (const size_t aOffset) const
{
    return At(aOffset);
}

template<typename T>
constexpr typename GpSpan<T>::value_type&   GpSpan<T>::operator* (void) const
{
    return At(0);
}

template<typename T>
constexpr typename GpSpan<T>::this_type&    GpSpan<T>::operator++ (void)
{
    return OffsetAdd(1);
}

template<typename T>
constexpr typename GpSpan<T>::this_type GpSpan<T>::operator++ (int)
{
    this_type res(*this);
    OffsetAdd(1);
    return res;
}

template<typename T>
constexpr typename GpSpan<T>::this_type&    GpSpan<T>::operator+= (const size_t aOffset)
{
    return OffsetAdd(aOffset);
}

template<typename T>
constexpr bool  GpSpan<T>::operator== (const this_type& aSpan) const noexcept
{
    return IsEqual(aSpan);
}

template<typename T>
constexpr bool  GpSpan<T>::operator!= (const this_type& aSpan) const noexcept
{
    return IsNotEqual(aSpan);
}

template<typename T>
constexpr bool  GpSpan<T>::operator> (const this_type& aSpan) const noexcept
{
    return IsGreater(aSpan);
}

template<typename T>
constexpr bool  GpSpan<T>::operator< (const this_type& aSpan) const noexcept
{
    return IsLess(aSpan);
}

template<typename T>
bool operator==
(
    const GpSpan<T>& aSpanA,
    const GpSpan<T>& aSpanB
) noexcept
{
    return aSpanA.IsEqual(aSpanB);
}

template<typename T>
bool operator!=
(
    const GpSpan<T>& aSpanA,
    const GpSpan<T>& aSpanB
) noexcept
{
    return aSpanA.IsNotEqual(aSpanB);
}

template<typename T>
constexpr typename GpSpan<T>::pointer   GpSpan<T>::Ptr (void) const
{
    return iPtr;
}

template<typename T>
constexpr typename GpSpan<T>::pointer   GpSpan<T>::Ptr (const size_t aOffset) const
{
    THROW_COND_GP
    (
        aOffset <= Count(),
        "Out of range"_sv
    );

    return iPtr + aOffset;
}

template<typename T>
constexpr typename GpSpan<T>::value_type&   GpSpan<T>::At (const size_t aOffset) const
{
    return *Ptr(aOffset);
}

template<typename T>
constexpr typename GpSpan<T>::this_type&    GpSpan<T>::OffsetAdd (const size_t aOffset)
{
    THROW_COND_GP
    (
        aOffset <= Count(),
        "Out of range"_sv
    );

    iCount  -= aOffset;
    iPtr    += aOffset;

    return *this;
}

template<typename T>
constexpr typename GpSpan<T>::this_type GpSpan<T>::Subspan
(
    const size_t aOffset,
    const size_t aCount
) const
{
    return SubspanAs<this_type>(aOffset, aCount);
}

template<typename T>
constexpr typename GpSpan<T>::this_type GpSpan<T>::SubspanThenOffsetAdd (size_t aCount)
{
    THROW_COND_GP
    (
        aCount <= Count(),
        "Out of range"_sv
    );

    this_type subspan{iPtr, aCount};

    iCount  -= aCount;
    iPtr    += aCount;

    return subspan;
}

//template<typename T>
//constexpr typename GpSpan<T>::this_type   GpSpan<T>::Subspan (const size_t aOffset) const
//{
//  return SubspanAs<this_type>(aOffset, NumOps::SSub<size_t>(iCount, aOffset));
//}

template<typename T>
constexpr std::string_view  GpSpan<T>::AsStringView (void) const
{
    const size_t sizeInBytes = SizeInBytes();
    return std::string_view(PtrAs<const char*>(), sizeInBytes);
}

template<typename T>
std::span<typename GpSpan<T>::value_type>   GpSpan<T>::AsStdSpan (void) const
{
    return std::span<value_type>(Ptr(), Count());
}

template<typename T>
std::span<std::byte>    GpSpan<T>::AsStdSpanByte (void) const
{
    const size_t sizeInBytes = SizeInBytes();
    return std::span<std::byte>(PtrAs<std::byte*>(), sizeInBytes);
}

template<typename T>
std::span<const std::byte>  GpSpan<T>::AsStdSpanConstByte (void) const
{
    const size_t sizeInBytes = SizeInBytes();
    return std::span<const std::byte>(PtrAs<const std::byte*>(), sizeInBytes);
}

template<typename T>
std::vector<std_byte_no_init>   GpSpan<T>::ToByteArray (void) const
{
    std::vector<std_byte_no_init> res;

    const size_t sizeInBytes = SizeInBytes();

    if (sizeInBytes > 0)
    {
        res.resize(sizeInBytes);
        std::memcpy(std::data(res), PtrAs<const std::byte*>(), sizeInBytes);
    }

    return res;
}

template<typename T>
std::vector<u_int_8>        GpSpan<T>::ToUI8Array (void) const
{
    std::vector<u_int_8> res;

    const size_t sizeInBytes = SizeInBytes();

    if (sizeInBytes > 0)
    {
        res.resize(sizeInBytes);
        std::memcpy(std::data(res), PtrAs<const u_int_8*>(), sizeInBytes);
    }

    return res;
}

template<typename T>
template<typename SpanT>
typename GpSpan<T>::this_type&  GpSpan<T>::CopyFrom (const SpanT& aSpan)
{
    THROW_COND_GP
    (
        Count() >= aSpan.Count(),
        "Out of range"_sv
    );

    MemOps::SCopy
    (
        Ptr(),
        aSpan.template PtrAs<const value_type*>(),
        _SCountAs<const value_type*, typename SpanT::pointer>(aSpan.Count())
    );

    return *this;
}

template<typename T>
template<typename PtrToT, typename PtrFromT>
constexpr PtrToT    GpSpan<T>::_SPtrAs (PtrFromT aFrom) noexcept
{
    return reinterpret_cast<PtrToT>(aFrom);
}

template<typename T>
template<typename PtrToT, typename PtrFromT>
constexpr size_t    GpSpan<T>::_SCountAs (const size_t aValue) noexcept
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
            GpThrowCe<GpException>("GpSpan<T>::_SCountAs wrong types");
        }

        return NumOps::SMul(aValue, sizeOfFrom / sizeOfTo);
    } else if constexpr (sizeOfTo > sizeOfFrom)
    {
        if constexpr ((sizeOfTo % sizeOfFrom) != 0)
        {
            GpThrowCe<GpException>("GpSpan<T>::_SCountAs wrong types");
        }

        return NumOps::SDiv(aValue, sizeOfTo / sizeOfFrom);
    }

    GpThrowCe<GpException>("GpSpan<T>::_SCountAs wrong types");
}

template<typename T>
void    GpSpan<T>::_CheckPointers
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
        "(nullptr and not empty) or (not nullptr and empty)"_sv
    );
}

}// namespace GPlatform

// ---------------------------------------------- std -----------------------------------------------------
namespace std
{
    template<typename T>
    size_t size (const ::GPlatform::GpSpan<T>& aSpan)
    {
        return aSpan.Count();
    }

    template<typename T>
    const T* data (const ::GPlatform::GpSpan<T>& aSpan)
    {
        return aSpan.Ptr();
    }

    template<typename T>
    T* data (::GPlatform::GpSpan<T>& aSpan)
    {
        return aSpan.Ptr();
    }
}// namespace std
