#pragma once

#include "../Config/GpConfig.hpp"

#if defined(GP_USE_MEMORY_OPS)

#include "Macro/GpMacroClass.hpp"
#include "Types/Numerics/GpNumericOps.hpp"
#include <cstring>
#include <utility>
#include <sys/types.h>

namespace GPlatform {

namespace MemOpsConcepts {

template <typename T>
concept IsOneBytePtr = requires()
{
    requires
       std::is_same_v<T, char>
    || std::is_same_v<T, unsigned char>
    || std::is_same_v<T, std::byte>
    || std::is_same_v<T, u_int_8>
    || std::is_same_v<T, char8_t>;
};

}//namespace GpMemOpsConcepts

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
    //------------------------ std new/delete ----------------------------
    template<typename T, typename... Ts> static
    T*      SNew (Ts&&... aArgs)
    {
        return new T(std::forward<Ts>(aArgs)...);
    }

    template<typename T, typename... Ts> static
    T*      SEmplace (void* aPtrToPlace, Ts&&... aArgs)
    {
        return new(aPtrToPlace) T(std::forward<Ts>(aArgs)...);
    }

    template<typename T>
    static
    void    SDelete (T* aObject)
    {
        delete aObject;
    }

    //------------------------ std new/delete array ----------------------------
    template<typename T>
    static
    void    SDeleteArray (T* aObject)
    {
        delete[] aObject;
    }

    //------------------------------------------------------------------------------
    template<typename       T,
             typename...    Ts>
    static void SConstruct
    (
        T*              aElements,
        const size_t    aCount,
        Ts&&...         aArgs
    )
    {
        if constexpr (std::is_scalar<T>::value)
        {
            SConstructScalar(aElements, aCount);
        } else
        {
            SConstructNotScalar(aElements, aCount, std::forward<Ts>(aArgs)...);
        }
    }

    template<typename T>
    static void SDestruct
    (
        T*              aElements,
        const size_t    aCount
    ) noexcept
    {
        if constexpr (std::is_scalar<T>::value)
        {
            SDestructScalar(aElements, aCount);
        } else
        {
            SDestructNotScalar(aElements, aCount);
        }
    }

    template<typename T>
    static constexpr void   SConstructAndMove
    (
        T&  aElementsDst,
        T&& aElementsSrc
    )
    {
        SConstructAndMove(&aElementsDst, &aElementsSrc, 1);
    }

    template<typename T>
    static constexpr void   SConstructAndMove
    (
        T*  aElementsDst,
        T*  aElementsSrc
    )
    {
        SConstructAndMove(aElementsDst, aElementsSrc, 1);
    }

    template<typename T>
    static constexpr void   SConstructAndMove
    (
        T*              aElementsDst,
        T*              aElementsSrc,
        const size_t    aCount
    )
    {
        if constexpr (   std::is_scalar<T>::value
                      || (std::is_trivially_copyable_v<T> && std::is_trivial_v<T>))
        {
            SConstructAndMoveScalar
            (
                aElementsDst,
                aElementsSrc,
                aCount
            );
        } else
        {
            SConstructAndMoveNotScalar
            (
                aElementsDst,
                aElementsSrc,
                aCount
            );
        }
    }

    template<typename T>
    static constexpr void   SCopy
    (
        T&          aElementsDst,
        const T&    aElementsSrc
    )
    {
        SCopy(&aElementsDst, &aElementsSrc, 1);
    }

    template<typename T>
    static constexpr void   SCopy
    (
        T*          aElementsDst,
        const T*    aElementsSrc
    )
    {
        SCopy(aElementsDst, aElementsSrc, 1);
    }

    template<MemOpsConcepts::IsOneBytePtr T1,
             MemOpsConcepts::IsOneBytePtr T2>
    static constexpr void   SCopy
    (
        T1*             aElementsDst,
        const T2*       aElementsSrc,
        const size_t    aSize
    )
    {
        SCopy
        (
            reinterpret_cast<std::byte*>(aElementsDst),
            reinterpret_cast<const std::byte*>(aElementsSrc),
            aSize
        );
    }

    template<typename T>
    static constexpr void   SCopy
    (
        T*              aElementsDst,
        const T*        aElementsSrc,
        const size_t    aCount
    )
    {
        if (aCount == 0)
        {
            return;
        }

        if constexpr (   (std::is_scalar_v<T>)
                      || (std::is_trivially_copyable_v<T> && std::is_trivial_v<T>))
        {
            SCopyScalar
            (
                aElementsDst,
                aElementsSrc,
                aCount
            );
        } else
        {
            SCopyNotScalar
            (
                aElementsDst,
                aElementsSrc,
                aCount
            );
        }
    }

    template<typename T>
    [[nodiscard]] static constexpr ssize_t  SCompare
    (
        const T& aElementsA,
        const T& aElementsB
    ) noexcept
    {
        return SCompare(&aElementsA, &aElementsB, 1);
    }

    template<typename T>
    [[nodiscard]] static constexpr ssize_t  SCompare
    (
        const T* aElementsA,
        const T* aElementsB
    ) noexcept
    {
        return SCompare(aElementsA, aElementsB, 1);
    }

    template<typename T>
    [[nodiscard]] static constexpr ssize_t  SCompare
    (
        const T*        aElementsA,
        const T*        aElementsB,
        const size_t    aCount
    ) noexcept
    {
        if constexpr (   (std::is_scalar<T>::value)
                      || (std::is_trivially_copyable_v<T> && std::is_trivial_v<T>))

        {
            return SCompareScalar
            (
                aElementsA,
                aElementsB,
                aCount
            );
        } else
        {
            return SCompareNotScalar
            (
                aElementsA,
                aElementsB,
                aCount
            );
        }
    }

private:
    template<typename T>
    static void SConstructScalar
    (
        T*              aElements,
        const size_t    aCount
    )
    {
        static_assert(std::is_trivial_v<T>, "T must be TriviallyCopyable");
        const size_t s = NumOps::SMul(aCount, sizeof(T));
        std::memset(aElements, 0, s);
    }

    template<typename       T,
             typename...    Ts>
    static void SConstructNotScalar
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
            if (constructedCnt > 0)
            {
                SDestruct(aElements, constructedCnt);
            }

            throw;
        }
    }

    template<typename T>
    static void SDestructScalar
    (
        T*              aElements,
        const size_t    aCount
    ) noexcept
    {
        static_assert(std::is_trivial_v<T>, "T must be TriviallyCopyable");
        const size_t s = NumOps::SMul(aCount, sizeof(T));
        std::memset(aElements, 0, s);
    }

    template<typename T>
    static void SDestructNotScalar
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

    template<typename T>
    static constexpr void SConstructAndMoveScalar
    (
        T*              aElementsDst,
        T*              aElementsSrc,
        const size_t    aCount
    )
    {
        static_assert(std::is_trivial_v<T>, "T must be TriviallyCopyable");

        const size_t s = NumOps::SMul(aCount, sizeof(T));

        std::memcpy(aElementsDst, aElementsSrc, s);
    }

    template<typename T>
    static void SConstructAndMoveNotScalar
    (
        T*              aElementsDst,
        T*              aElementsSrc,
        const size_t    aCount
    )
    {
        T* dst = aElementsDst;
        T* src = aElementsSrc;

        //Move
        size_t movedCnt = 0;
        try
        {
            for ( ; movedCnt < aCount; ++movedCnt)
            {
                ::new (static_cast<void*>(dst)) T(std::move(*src));
                dst++;
                src++;
            }
        } catch (...)
        {
            if (movedCnt > 0)
            {
                SDestruct(aElementsDst, movedCnt);
            }

            throw;
        }
    }

    template<typename T>
    static constexpr void   SCopyScalar
    (
        T*              aElementsDst,
        const T*        aElementsSrc,
        const size_t    aCount
    )
    {
        const size_t s = NumOps::SMul(aCount, sizeof(T));
        std::memcpy(aElementsDst, aElementsSrc, s);
    }

    template<typename T>
    static void SCopyNotScalar
    (
        T*              aElementsDst,
        const T*        aElementsSrc,
        const size_t    aCount
    )
    {
        T*          dst = aElementsDst;
        const T*    src = aElementsSrc;

        //Destruct dst
        Destruct(aElementsDst, aCount);

        //Copy
        size_t copyCnt = 0;
        try
        {
            for ( ; copyCnt < aCount; ++copyCnt)
            {
                ::new (static_cast<void*>(dst)) T(*src);
                dst++;
                src++;
            }
        } catch (...)
        {
            if (copyCnt > 0)
            {
                Destruct(aElementsDst, copyCnt);
            }

            throw;
        }
    }

    template<typename T>
    [[nodiscard]] static constexpr ssize_t  SCompareScalar
    (
        const T*        aElementsA,
        const T*        aElementsB,
        const size_t    aCount
    ) noexcept
    {
        const size_t s = NumOps::SMul(aCount, sizeof(T));
        return std::memcmp(aElementsA, aElementsB, s);
    }

    template<typename T>
    [[nodiscard]]static ssize_t SCompareNotScalar
    (
        const T*        aElementsA,
        const T*        aElementsB,
        const size_t    aCount
    ) noexcept
    {
        for (size_t id = 0; id < aCount; ++id)
        {
            const T& a = *aElementsA++;
            const T& b = *aElementsB++;

            if (a > b)
            {
                return NumOps::SConvert<ssize_t>(NumOps::SInc(id));
            } else if (a < b)
            {
                return NumOps::SNegative(NumOps::SConvert<ssize_t>(NumOps::SInc(id)));
            }
        }

        return 0;
    }
};

using MemOps = GpMemOps;

}//GPlatform

#endif//#if defined(GP_USE_MEMORY_OPS)
