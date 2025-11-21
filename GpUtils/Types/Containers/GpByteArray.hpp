#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_CONTAINERS)

#include <GpCore2/GpUtils/Types/Pointers/GpSpan.hpp>
#include <functional>

namespace GPlatform {

using GpSpanCharR   = GpSpan<const char>;
using GpSpanCharRW  = GpSpan<char>;
using GpSpanByteRW  = GpSpan<std::byte>;
using GpSpanByteR   = GpSpan<const std::byte>;

using GpByteArray   = std::vector<std_byte_no_init>;

class GpArrayUtils
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpArrayUtils)

public:
    // --------------------------------- SMake ---------------------------------

    template<typename TO,
             typename FROM>
    requires
           (Concepts::HasContiguousIter<FROM> || GpHasTag_GpSpan<FROM>())
        &&  Concepts::HasContiguousIter<TO>
        && (sizeof(typename FROM::value_type) == sizeof(typename TO::value_type))
        && (alignof(typename FROM::value_type) == alignof(typename TO::value_type))
        && (std::is_trivially_copyable_v<typename FROM::value_type>)
        && (std::is_trivially_copyable_v<typename TO::value_type>)
    static TO SMake (const FROM& aContainer)
    {
        TO res;

        const size_t size = std::size(aContainer);
        res.resize(size);

        MemOps::SCopy
        (
            std::data(res),
            reinterpret_cast<const typename TO::value_type*>(std::data(aContainer)),
            size
        );

        return res;
    }

    // --------------------------------- SAppend ---------------------------------

    template<typename TO, typename FROM>
    requires
           (Concepts::HasContiguousIter<FROM> || GpHasTag_GpSpan<FROM>())
        &&  Concepts::HasContiguousIter<TO>
        && (sizeof(typename FROM::value_type) == sizeof(typename TO::value_type))
        && (alignof(typename FROM::value_type) == alignof(typename TO::value_type))
        && (std::is_trivially_copyable_v<typename FROM::value_type>)
        && (std::is_trivially_copyable_v<typename TO::value_type>)
    static TO&  SAppend
    (
        TO&         aDst,
        const FROM& aSrc
    )
    {
        const size_t oldSize = std::size(aDst);
        const size_t srcSize = std::size(aSrc);
        const size_t newSize = NumOps::SAdd(oldSize, srcSize);

        aDst.resize(newSize);

        MemOps::SCopy
        (
            reinterpret_cast<std::byte*>(std::data(aDst)) + oldSize,
            reinterpret_cast<const std::byte*>(std::data(aSrc)),
            srcSize
        );

        return aDst;
    }

    // --------------------------------- SMakeStdArrayFromC ---------------------------------

    template <typename T,
              std::size_t N>
    requires std::is_trivially_copyable_v<T>
    static std::array<T, N> SMakeStdArrayFromC (const T (&cArray)[N])
    {
        std::array<T, N> stdArray;

        MemOps::SCopy
        (
            std::data(stdArray),
            cArray,
            N
        );

        return stdArray;
    }

    // --------------------------------- SMakeStdArrayFromC ---------------------------------

    template <typename TO_T,
              typename FROM_T,
              std::size_t N>
    requires
           Concepts::IsArithmetic<FROM_T>
        && Concepts::IsArithmetic<TO_T>
        && (!std::is_same_v<FROM_T, TO_T>)
    static std::array<TO_T, N> SMakeStdArrayFromC (const FROM_T (&cArray)[N])
    {
        std::array<TO_T, N> stdArray;

        const FROM_T*   fromPtr = cArray;
        TO_T*           toPtr   = std::data(stdArray);

        for (size_t id = 0; id < N; id++)
        {
            *toPtr++ = NumOps::SConvert<TO_T>(*fromPtr++);
        }

        return stdArray;
    }

    // --------------------------------- SEraseFirstFast ---------------------------------

    template<typename                       V,
             Concepts::HasContiguousIter    C>
    static bool SEraseFirstFast
    (
        C&  aContainer,
        V&& aValue
    )
    {
        auto it = std::find(std::begin(aContainer), std::end(aContainer), aValue);

        if (it == std::end(aContainer))
        {
            return false;
        }

        *it = std::move(aContainer.back());
        aContainer.pop_back();

        return true;
    }

    // --------------------------------- SEraseFirstIfFast ---------------------------------

    template<Concepts::HasContiguousIter C>
    static bool SEraseFirstIfFast
    (
        C&                                                          aContainer,
        const std::function<bool(const typename C::value_type&)>&   aPredicate
    )
    {
        auto it = std::find_if(std::begin(aContainer), std::end(aContainer), aPredicate);

        if (it == std::end(aContainer))
        {
            return false;
        }

        if (std::size(aContainer) > 1) [[likely]]
        {
            *it = std::move(aContainer.back());
            aContainer.pop_back();
        } else// std::size(aContainer) == 1
        {
            aContainer.clear();
        }

        return true;
    }

    // --------------------------------- SFillZero ---------------------------------

    static GpSpanByteRW SFillZero (GpSpanByteRW aData)
    {
        if (!aData.Empty())
        {
            std::memset
            (
                aData.Ptr(),
                0,
                aData.Count()
            );
        }

        return aData;
    }
};

}// namespace GPlatform

// --------------------- std -------------------------
namespace std {

inline bool operator<(const ::GPlatform::GpByteArray& a, const ::GPlatform::GpByteArray& b)
{
    const size_t sizeA = ::std::size(a);
    const size_t sizeB = ::std::size(b);

    if (sizeA < sizeB)
    {
        return true;
    }

    if (sizeA > sizeB)
    {
        return false;
    }

    if (sizeA == 0)
    {
        return false;
    }

    return std::memcmp(std::data(a), std::data(b), sizeA) < 0;
}

}// namespace std

#endif// #if defined(GP_USE_CONTAINERS)
