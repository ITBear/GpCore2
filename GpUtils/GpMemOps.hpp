#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_MEMORY_OPS)

#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Numerics/GpNumericOps.hpp>

#include <cstring>
#include <utility>
#include <sys/types.h>

namespace GPlatform {

namespace MemOpsConcepts {

template <typename T>
concept IsRawCopyable = requires()
{
    requires
       (std::is_scalar_v<T>)
    || (std::is_trivially_copyable_v<T> && std::is_trivial_v<T>)
    || (std::is_same_v<T, std_byte_no_init>);
};

template <typename T>
concept IsNotRawCopyable = requires()
{
    requires
       !(std::is_scalar_v<T>)
    && !(std::is_trivially_copyable_v<T> && std::is_trivial_v<T>)
    && !(std::is_same_v<T, std_byte_no_init>);
};

}// namespace GpMemOpsConcepts

template <typename T>
ssize_t Compare(const T&, const T&);

class GpMemOps
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpMemOps)

public:
    struct VoidDeleter
    {
        template<typename T>
        static void SDelete (T*)
        {
            //NOP
        }
    };

    struct NewDeleter
    {
        template<typename T>
        static void SDelete (T* aPtr)
        {
            GpMemOps::SDelete<T>(aPtr);
        }
    };

    struct EmplaceDeleter
    {
        template<typename T>
        static void SDelete (T* aPtr)
        {
            GpMemOps::SDestruct<T>(aPtr, 1);
        }
    };

public:
    // ------------------------ std new/delete ----------------------------
    template<typename T, typename... Ts>
    static T*                   SNew            (Ts&&... aArgs);

    template<typename T, typename... Ts>
    static T*                   SEmplace        (void*      aPtrToPlace,
                                                 Ts&&...    aArgs);

    template<typename T>
    static void                 SDelete         (T* aObject);

    // ------------------------ std new/delete array ----------------------------
    template<typename T>
    static
    void                        SDeleteArray    (T* aObject);

    // ---------------------------------- SConstruct --------------------------------------------
    template<MemOpsConcepts::IsRawCopyable T>
    static void                 SConstruct      (T*     aElements,
                                                 size_t aCount);
    template<MemOpsConcepts::IsNotRawCopyable   T,
             typename...                        Ts>
    static void                 SConstruct      (T*         aElements,
                                                 size_t     aCount,
                                                 Ts&&...    aArgs);

    template<MemOpsConcepts::IsRawCopyable T>
    static void                 SDestruct       (T*     aElements,
                                                 size_t aCount) noexcept;
    template<MemOpsConcepts::IsNotRawCopyable T>
    static void                 SDestruct       (T*     aElements,
                                                 size_t aCount) noexcept;

    template<MemOpsConcepts::IsRawCopyable T>
    static constexpr T          SCopyBitCast    (const void* aElementsSrc);

    template<MemOpsConcepts::IsRawCopyable T>
    static constexpr void       SCopy           (T&         aElementsDst,
                                                 const T&   aElementsSrc);
    template<MemOpsConcepts::IsRawCopyable T>
    static constexpr void       SCopy           (T*         aElementsDst,
                                                 const T*   aElementsSrc);
    template<MemOpsConcepts::IsRawCopyable T>
    static constexpr void       SCopy           (T*         aElementsDst,
                                                 const T*   aElementsSrc,
                                                 size_t     aCount);

    template<MemOpsConcepts::IsNotRawCopyable T>
    static constexpr void       SCopy           (T&         aElementsDst,
                                                 const T&   aElementsSrc);
    template<MemOpsConcepts::IsNotRawCopyable T>
    static constexpr void       SCopy           (T*         aElementsDst,
                                                 const T*   aElementsSrc);
    template<MemOpsConcepts::IsNotRawCopyable T>
    static constexpr void       SCopy           (T*         aElementsDst,
                                                 const T*   aElementsSrc,
                                                 size_t     aCount);

    template<typename T>
    static constexpr bool       SIsEqual        (const T*       aElementsA,
                                                 const size_t   aCountA,
                                                 const T*       aElementsB,
                                                 const size_t   aCountB) noexcept;
    template<typename T>
    static constexpr bool       SIsGreater      (const T*       aElementsA,
                                                 const size_t   aCountA,
                                                 const T*       aElementsB,
                                                 const size_t   aCountB) noexcept;
    template<typename T>
    static constexpr bool       SIsLess         (const T*       aElementsA,
                                                 const size_t   aCountA,
                                                 const T*       aElementsB,
                                                 const size_t   aCountB) noexcept;
    template<typename T>
    static constexpr ssize_t    SCompare        (const T& aElementsA,
                                                 const T& aElementsB) noexcept;
    template<typename T>
    static constexpr ssize_t    SCompare        (const T* aElementsA,
                                                 const T* aElementsB) noexcept;
    template<MemOpsConcepts::IsRawCopyable T>
    static constexpr ssize_t    SCompare        (const T*       aElementsA,
                                                 const T*       aElementsB,
                                                 const size_t   aCount) noexcept;
    template<MemOpsConcepts::IsNotRawCopyable T>
    static constexpr ssize_t    SCompare        (const T*       aElementsA,
                                                 const T*       aElementsB,
                                                 const size_t   aCount) noexcept;
};

template<typename T, typename... Ts>
T*  GpMemOps::SNew (Ts&&... aArgs)
{
    return ::new T(std::forward<Ts>(aArgs)...);
}

template<typename T, typename... Ts>
T*  GpMemOps::SEmplace
(
    void*   aPtrToPlace,
    Ts&&... aArgs
)
{
    return ::new(aPtrToPlace) T(std::forward<Ts>(aArgs)...);
}

template<typename T>
void    GpMemOps::SDelete (T* aObject)
{
    ::delete aObject;
}

template<typename T>
void    GpMemOps::SDeleteArray (T* aObject)
{
    ::delete[] aObject;
}

template<MemOpsConcepts::IsRawCopyable T>
void    GpMemOps::SConstruct
(
    T*              aElements,
    const size_t    aCount
)
{
    const size_t s = NumOps::SMul(aCount, sizeof(T));
    std::memset(aElements, 0, s);
}

template<MemOpsConcepts::IsNotRawCopyable   T,
         typename...                        Ts>
void    GpMemOps::SConstruct
(
    T*              aElements,
    const size_t    aCount,
    Ts&&...         aArgs
)
{
    T* e = aElements;

    size_t constructedCnt = 0;
    try
    {
        for ( ; constructedCnt < aCount; ++constructedCnt)
        {
            ::new (static_cast<void*>(e)) T(std::forward<Ts>(aArgs)...);
            e++;
        }
    } catch (...)
    {
        std::exception_ptr currentException = std::current_exception();
        {
            if (constructedCnt > 0)
            {
                SDestruct(aElements, constructedCnt);
            }
        }
        std::rethrow_exception(currentException);
    }
}

template<MemOpsConcepts::IsRawCopyable T>
void    GpMemOps::SDestruct
(
    T*              aElements,
    const size_t    aCount
) noexcept
{
    static_assert(std::is_trivial_v<T>, "T must be TriviallyCopyable");
    const size_t s = NumOps::SMul(aCount, sizeof(T));
    std::memset(aElements, 0, s);
}

template<MemOpsConcepts::IsNotRawCopyable T>
void    GpMemOps::SDestruct
(
    T*              aElements,
    const size_t    aCount
) noexcept
{
    for (size_t id = 0; id < aCount; ++id)
    {
        aElements->~T();
        aElements++;
    }
}

template<MemOpsConcepts::IsRawCopyable T>
constexpr T GpMemOps::SCopyBitCast (const void* aElementsSrc)
{
    T value;
    std::memcpy(&value, aElementsSrc, sizeof(T));
    return value;
}

template<MemOpsConcepts::IsRawCopyable T>
constexpr void  GpMemOps::SCopy
(
    T&          aElementsDst,
    const T&    aElementsSrc
)
{
    GpMemOps::SCopy<T>(&aElementsDst, &aElementsSrc, 1);
}

template<MemOpsConcepts::IsRawCopyable T>
constexpr void  GpMemOps::SCopy
(
    T*          aElementsDst,
    const T*    aElementsSrc
)
{
    GpMemOps::SCopy<T>(aElementsDst, aElementsSrc, 1);
}

template<MemOpsConcepts::IsRawCopyable T>
constexpr void  GpMemOps::SCopy
(
    T*          aElementsDst,
    const T*    aElementsSrc,
    size_t      aCount
)
{
    if (aCount == 0) [[unlikely]]
    {
        return;
    }

    const size_t s = NumOps::SMul(aCount, sizeof(T));
    std::memcpy(aElementsDst, aElementsSrc, s);
}

template<MemOpsConcepts::IsNotRawCopyable T>
constexpr void  GpMemOps::SCopy
(
    T&          aElementsDst,
    const T&    aElementsSrc
)
{
    GpMemOps::SCopy<T>(&aElementsDst, &aElementsSrc, 1);
}

template<MemOpsConcepts::IsNotRawCopyable T>
constexpr void  GpMemOps::SCopy
(
    T*          aElementsDst,
    const T*    aElementsSrc
)
{
    GpMemOps::SCopy<T>(aElementsDst, aElementsSrc, 1);
}

template<MemOpsConcepts::IsNotRawCopyable T>
constexpr void  GpMemOps::SCopy
(
    T*          aElementsDst,
    const T*    aElementsSrc,
    size_t      aCount
)
{
    if (aCount == 0) [[unlikely]]
    {
        return;
    }

    T*          dst = aElementsDst;
    const T*    src = aElementsSrc;

    // Copy
    for (size_t id = 0; id < aCount; ++id)
    {
        *dst++ = *src++;
    }
}

template<typename T>
constexpr bool  GpMemOps::SIsEqual
(
    const T*        aElementsA,
    const size_t    aCountA,
    const T*        aElementsB,
    const size_t    aCountB
) noexcept
{
    if (aCountA != aCountB)
    {
        return false;
    }

    return SCompare(aElementsA, aElementsB, aCountA) == 0;
}

template<typename T>
constexpr bool  GpMemOps::SIsGreater
(
    const T*        aElementsA,
    const size_t    aCountA,
    const T*        aElementsB,
    const size_t    aCountB
) noexcept
{
    if (aCountA > aCountB)
    {
        return true;
    }

    if (aCountA < aCountB)
    {
        return false;
    }

    return SCompare(aElementsA, aElementsB, aCountA) > 0;
}

template<typename T>
constexpr bool  GpMemOps::SIsLess
(
    const T*        aElementsA,
    const size_t    aCountA,
    const T*        aElementsB,
    const size_t    aCountB
) noexcept
{
    if (aCountA < aCountB)
    {
        return true;
    }

    if (aCountA > aCountB)
    {
        return false;
    }

    return SCompare(aElementsA, aElementsB, aCountA) < 0;
}

template<typename T>
constexpr ssize_t   GpMemOps::SCompare
(
    const T& aElementsA,
    const T& aElementsB
) noexcept
{
    return SCompare(&aElementsA, &aElementsB, 1);
}

template<typename T>
constexpr ssize_t   GpMemOps::SCompare
(
    const T* aElementsA,
    const T* aElementsB
) noexcept
{
    return SCompare(aElementsA, aElementsB, 1);
}

template<MemOpsConcepts::IsRawCopyable T>
constexpr ssize_t   GpMemOps::SCompare
(
    const T*        aElementsA,
    const T*        aElementsB,
    const size_t    aCount
) noexcept
{
    if (aCount == 0) [[unlikely]]
    {
        return 0;
    }

    const size_t s = NumOps::SMul(aCount, sizeof(T));
    return std::memcmp(aElementsA, aElementsB, s);
}

template<MemOpsConcepts::IsNotRawCopyable T>
constexpr ssize_t   GpMemOps::SCompare
(
    const T*        aElementsA,
    const T*        aElementsB,
    const size_t    aCount
) noexcept
{
    if (aCount == 0) [[unlikely]]
    {
        return 0;
    }

    T*          a = aElementsA;
    const T*    b = aElementsB;

    // Copy
    for (size_t id = 0; id < aCount; ++id)
    {
        ::GPlatform::Compare<T>(*a++, *b++);
    }
}

using MemOps = GpMemOps;

}// namespace GPlatform

#endif// #if defined(GP_USE_MEMORY_OPS)
