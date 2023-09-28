#pragma once

#include "../../../Config/GpConfig.hpp"
#include "../../Macro/GpMacroClass.hpp"
#include "../../Concepts/GpConcepts.hpp"
#include "../Numerics/GpNumericTypes.hpp"
#include "../Pointers/GpSpanPtr.hpp"

#include <bit>
#include <tuple>

#if defined(GP_CPU_USE_BMI2)
#   include <immintrin.h>
#endif

//TODO: implement with https://en.cppreference.com/w/cpp/header/bit

namespace GPlatform {

#if defined(GP_COMPILER_GCC) || defined(GP_COMPILER_CLANG)
#define BSWAP16(x)  __builtin_bswap16(x)
#define BSWAP32(x)  __builtin_bswap32(x)
#define BSWAP64(x)  __builtin_bswap64(x)
#define BSWAP128(x) __builtin_bswap128(x)
#elif defined(GP_COMPILER_MSVC)
#define BSWAP16(x)  _byteswap_ushort(x)
#define BSWAP32(x)  _byteswap_ulong(x)
#define BSWAP64(x)  _byteswap_uint64(x)
#define BSWAP128(x) _byteswap_uint128(x)
#else
#   error Unknown compiler
#endif

static_assert
(
       (std::endian::native == std::endian::big)
    || (std::endian::native == std::endian::little)
    , "Mixed endian"
);

static_assert
(
       (sizeof(unsigned int) == sizeof(u_int_32))
    && (sizeof(unsigned long) == sizeof(u_int_64))
    && (sizeof(unsigned long long) == sizeof(u_int_64))
    , ""
);

class GpBitOperations
{
public:
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBitOperations)


public:
    //------------------------------------ Network/Host byte order (N2H/H2N) --------------------------------
    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static
    T                               BSwap (const T aValue)
    {
        typename std::make_unsigned<T>::type tmpVal;
        tmpVal = std::bit_cast<decltype(tmpVal)>(aValue);

        if constexpr(sizeof(T) == 2)        tmpVal = BSWAP16(tmpVal);
        else if constexpr(sizeof(T) == 4)   tmpVal = BSWAP32(tmpVal);
        else if constexpr(sizeof(T) == 8)   tmpVal = BSWAP64(tmpVal);
        else if constexpr(sizeof(T) == 16)  tmpVal = BSWAP128(tmpVal);

        return std::bit_cast<T>(tmpVal);
    }

    [[nodiscard]] static double     BSwap (const double aValue)
    {
        static_assert(sizeof(double) == sizeof(u_int_64), "sizeof(double) != sizeof(u_int_64)");

        const u_int_64 tmpVal = std::bit_cast<u_int_64>(aValue);
        return std::bit_cast<double>(BSWAP64(tmpVal));
    }

    [[nodiscard]] static float      BSwap (float aValue)
    {
        static_assert(sizeof(float) == sizeof(u_int_32), "sizeof(float) != sizeof(u_int_32)");

        const u_int_32 tmpVal = std::bit_cast<u_int_32>(aValue);
        return std::bit_cast<float>(BSWAP32(tmpVal));
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static
    T                               Native2BigEndian (T aValue)
    {
        if constexpr (std::endian::native == std::endian::big)
        {
            return aValue;
        } else if constexpr (std::endian::native == std::endian::little)
        {
            return BSwap(aValue);
        }
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static
    T                               Native2LittleEndian (T aValue)
    {
        if constexpr (std::endian::native == std::endian::big)
        {
            return BSwap(aValue);
        } else if constexpr (std::endian::native == std::endian::little)
        {
            return aValue;
        }
    }

    template<typename T>
    requires   Concepts::IsIntegralUpTo128<T>
            || std::is_same_v<T, double>
            || std::is_same_v<T, float>
    [[nodiscard]] static T          N2H (T aValue)
    {
        if constexpr (std::endian::native == std::endian::big)
        {
            return aValue;
        } else if constexpr (std::endian::native == std::endian::little)
        {
            return BSwap(aValue);
        }
    }

    template<typename T>
    requires   Concepts::IsIntegralUpTo128<T>
            || std::is_same_v<T, double>
            || std::is_same_v<T, float>
    [[nodiscard]] static T          H2N (T aValue)
    {
        if constexpr (std::endian::native == std::endian::big)
        {
            return aValue;
        } else if constexpr (std::endian::native == std::endian::little)
        {
            return BSwap(aValue);
        }
    }

    template<typename T>
    requires   Concepts::IsIntegralUpTo128<T>
            || std::is_same_v<T, double>
            || std::is_same_v<T, float>
    static void                     H2N_Span (GpSpanPtr<T*> aSpan)
    {
        if constexpr (std::endian::native == std::endian::big)
        {
            return;
        } else if constexpr (std::endian::native == std::endian::little)
        {
            const size_t    count   = aSpan.Count();
            T*              ptr     = aSpan.Ptr();
            for (size_t id = 0; id < count; id++)
            {
                *ptr = BSwap<T>(*ptr);
                ptr++;
            }
        }
    }

    template<typename T>
    requires   Concepts::IsIntegralUpTo128<T>
            || std::is_same_v<T, double>
            || std::is_same_v<T, float>
    static void                     N2H_Span (GpSpanPtr<T*> aSpan)
    {
        H2N_Span<T>(aSpan);
    }

    //------------------------------------ Interleave16_16(Morton Codes) ------------------------------------
    template<u_int_16 Left, u_int_16 Right> [[nodiscard]] static constexpr
    u_int_32                        Interleave16_16 (void) noexcept
    {
        return Interleave16_16_std(Left, Right);
    }

    [[nodiscard]] static
    inline u_int_32                 Interleave16_16     (const u_int_16 aLeft, const u_int_16 aRight) noexcept;

    [[nodiscard]] static constexpr
    inline u_int_32                 Interleave16_16_std (const u_int_16 aLeft, const u_int_16 aRight) noexcept;

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]] static
    inline  u_int_32                Interleave16_16_bmi (const u_int_16 aLeft, const u_int_16 aRight) noexcept;
#endif//GP_CPU_USE_BMI2

    //------------------------------------ Interleave32_32(Morton Codes) ------------------------------------
    template<u_int_32 Left, u_int_32 Right> [[nodiscard]] static constexpr
    u_int_64                        Interleave32_32 (void) noexcept
    {
        return Interleave32_32_std(Left, Right);
    }

    [[nodiscard]] static
    inline u_int_64             Interleave32_32     (const u_int_32 aLeft, const u_int_32 aRight) noexcept;

    [[nodiscard]] static constexpr
    inline u_int_64             Interleave32_32_std (const u_int_32 aLeft, const u_int_32 aRight) noexcept;

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]] static
    inline u_int_64             Interleave32_32_bmi (const u_int_32 aLeft, const u_int_32 aRight) noexcept;
#endif//GP_CPU_USE_BMI2

    //------------------------------------ Deinterleave16_16(Morton Codes) ------------------------------------
    template<u_int_32 Value> [[nodiscard]] static constexpr
    std::tuple<u_int_16/*left*/,
               u_int_16/*right*/>   Deinterleave16_16       (void) noexcept
    {
        return Deinterleave16_16_std(Value);
    }

    [[nodiscard]] static inline
    std::tuple<u_int_16/*left*/,
               u_int_16/*right*/>   Deinterleave16_16       (const u_int_32 aValue) noexcept;

    [[nodiscard]] static constexpr inline
    std::tuple<u_int_16/*left*/,
               u_int_16/*right*/>   Deinterleave16_16_std   (const u_int_32 aValue) noexcept;

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]] static inline
    std::tuple<u_int_16/*left*/,
               u_int_16/*right*/>   Deinterleave16_16_bmi   (const u_int_32 aValue) noexcept;
#endif//GP_CPU_USE_BMI2

    //------------------------------------ Deinterleave32_32(Morton Codes) ------------------------------------
    template<u_int_64 Value> [[nodiscard]] static constexpr
    std::tuple<u_int_32/*left*/,
               u_int_32/*right*/>   Deinterleave32_32       (void) noexcept
    {
        return Deinterleave32_32_std(Value);
    }

    [[nodiscard]] static inline
    std::tuple<u_int_32/*left*/,
               u_int_32/*right*/>   Deinterleave32_32       (const u_int_64 aValue) noexcept;

    [[nodiscard]] static constexpr inline
    std::tuple<u_int_32/*left*/,
               u_int_32/*right*/>   Deinterleave32_32_std   (const u_int_64 aValue) noexcept;

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]] static inline
    std::tuple<u_int_32/*left*/,
               u_int_32/*right*/>   Deinterleave32_32_bmi   (const u_int_64 aValue) noexcept;
#endif//GP_CPU_USE_BMI2

    //----------------------------- Determine if a word has a byte = 0 -------------------------
    [[nodiscard]] static constexpr inline
    bool                            Has0ByteIn32            (const u_int_32 aValue) noexcept;

    [[nodiscard]] static constexpr inline
    bool                            Has0ByteIn64            (const u_int_64 aValue) noexcept;

    //----------------------------- Determine if a word has a byte = V -------------------------
    [[nodiscard]] static constexpr inline
    bool                            HasSpecificByteIn32     (const u_int_32 aValue,
                                                             const u_int_8  aSpecific) noexcept;

    [[nodiscard]] static constexpr inline
    bool                            HasSpecificByteIn64     (const u_int_64 aValue,
                                                             const u_int_8  aSpecific) noexcept;

    //-------------------------------------------------------------------------------------------
    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    T                               SetByMask           (const T aValueDst,
                                                         const T aValueSrc,
                                                         const T aMask) noexcept
    {
        return T((aValueDst & ~(aMask)) | (aValueSrc & (aMask)));
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    T                               And_by_mask         (const T aValue, const T aBitMask) noexcept
    {
        return T(aValue & aBitMask);
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    T                               And_by_id           (const T aValue, const size_t aBitId) noexcept
    {
        return And_by_mask<T>(aValue, T(1) << aBitId);
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    T                               Up_by_mask          (const T aValue, const T aBitMask) noexcept
    {
        return T(aValue | aBitMask);
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    T                               Up_by_id            (const T aValue, const size_t aBitId) noexcept
    {
        return Up_by_mask<T>(aValue, T(1) << aBitId);
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    T                               Down_by_mask        (const T aValue, const T aBitMask) noexcept
    {
        return T(aValue & ~(aBitMask));
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    T                               Down_by_id          (const T aValue, const size_t aBitId) noexcept
    {
        return Down_by_mask<T>(aValue, T(1) << aBitId);
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    T                               UpAndDown_by_mask   (const T aValue, const T aUpBitMask, const T aDownBitMask) noexcept
    {
        return Up(Down(aValue, aDownBitMask), aUpBitMask);
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    T                               Invert_by_mask      (const T aValue, const T aBitMask) noexcept
    {
        return T(aValue ^ aBitMask);
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    T                               Invert_by_id            (const T aValue, const size_t aBitId) noexcept
    {
        return Invert_by_mask<T>(aValue, T(1) << aBitId);
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    bool                            TestAny_by_mask     (const T aValue, const T aBitMask) noexcept
    {
        return T(aValue & aBitMask);
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    T                               Test_by_id          (const T aValue, const size_t aBitId) noexcept
    {
        return TestAny_by_mask<T>(aValue, T(1) << aBitId);
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    bool                            TestAll_by_mask     (const T aValue, const T aBitMask) noexcept
    {
        return T((aValue & aBitMask) == aBitMask);
    }

    template<Concepts::IsIntegralUpTo128 T> [[nodiscard]] static constexpr
    bool                            Match_by_mask       (const T aValueA, const T aValueB, const T aBitMask) noexcept
    {
        return (aValueA & aBitMask) == (aValueB & aBitMask);
    }

    template<Concepts::IsIntegralUpTo128 TValue> [[nodiscard]] static constexpr
    TValue                          SHL                 (const TValue aValue, const size_t aShift) noexcept
    {
        return TValue(aValue << aShift);
    }

    template<Concepts::IsIntegralUpTo128 TValue> [[nodiscard]] static constexpr
    TValue                          SHR                 (const TValue aValue, const size_t aShift) noexcept
    {
        return TValue(aValue >> aShift);
    }

    template<Concepts::IsIntegralUpTo128 TValue> [[nodiscard]] static constexpr
    TValue                          SHL_C               (const TValue aValue, const size_t aShift) noexcept
    {
        if (aShift == 0) return aValue;
        return (aValue << aShift) | (aValue >> ((sizeof(aValue) * size_t(8)) - aShift));
    }

    template<Concepts::IsIntegralUpTo128 TValue> [[nodiscard]] static constexpr
    TValue                          SHR_C               (const TValue aValue, const size_t aShift) noexcept
    {
        if (aShift == 0) return aValue;
        return (aValue >> aShift) | (aValue << ((sizeof(aValue) * size_t(8)) - aShift));
    }

    template<Concepts::IsIntegralUpTo64 T> [[nodiscard]] static constexpr
    size_t                          Leading0bitCnt  (T aValue) noexcept
    {
        using UT = typename std::make_unsigned<T>::type;

        const UT v = std::bit_cast<UT>(aValue);

        if (v == UT(0))
        {
            return static_cast<size_t>(sizeof(UT) * 8);
        }

        if constexpr (std::is_same<UT, u_int_8>::value)
        {
            return static_cast<size_t>(__builtin_clz(static_cast<unsigned int>(v))) - (sizeof(unsigned int) - sizeof(u_int_8))*8;
        } else if constexpr (std::is_same<UT, u_int_16>::value)
        {
            return static_cast<size_t>(__builtin_clz(static_cast<unsigned int>(v))) - (sizeof(unsigned int) - sizeof(u_int_16))*8;
        } else if constexpr (std::is_same<UT, u_int_32>::value)
        {
            return static_cast<size_t>(__builtin_clz(static_cast<unsigned int>(v))) - (sizeof(unsigned int) - sizeof(u_int_32))*8;
        } else if constexpr (std::is_same<UT, u_int_64>::value)
        {
            return static_cast<size_t>(__builtin_clzl(static_cast<unsigned long>(v))) - (sizeof(unsigned long) - sizeof(u_int_64))*8;
        }
    }

    template<Concepts::IsIntegralUpTo64 T> [[nodiscard]] static constexpr
    size_t                          Leading1bitCnt  (T aValue) noexcept
    {
        using UT = typename std::make_unsigned<T>::type;
        const UT v  = std::bit_cast<UT>(aValue);
        const UT iv = ~v;
        return Leading0bitCnt(iv);
    }

    template<Concepts::IsIntegralUpTo64 T> [[nodiscard]] static constexpr
    size_t                      Trailing0bitCnt (T aValue) noexcept
    {
        using UT = typename std::make_unsigned<T>::type;

        const UT v = std::bit_cast<UT>(aValue);

        if (v == UT(0))
        {
            return static_cast<size_t>(sizeof(UT)*8);
        }

        if constexpr (std::is_same<UT, u_int_8>::value)
        {
            return static_cast<size_t>(__builtin_ctz(static_cast<unsigned int>(v)));
        } else if constexpr (std::is_same<UT, u_int_16>::value)
        {
            return static_cast<size_t>(__builtin_ctz(static_cast<unsigned int>(v)));
        } else if constexpr (std::is_same<UT, u_int_32>::value)
        {
            return static_cast<size_t>(__builtin_ctz(static_cast<unsigned int>(v)));
        } else if constexpr (std::is_same<UT, u_int_64>::value)
        {
            return static_cast<size_t>(__builtin_ctzl(static_cast<unsigned long>(v)));
        }
    }

    template<Concepts::IsIntegralUpTo64 T> [[nodiscard]] static constexpr
    size_t                      PopCount    (T aValue) noexcept
    {
        using UT = typename std::make_unsigned<T>::type;

        const UT v = std::bit_cast<UT>(aValue);

        if constexpr (std::is_same<UT, u_int_8>::value)
        {
            return static_cast<size_t>(__builtin_popcount(static_cast<unsigned int>(v)));
        } else if constexpr (std::is_same<UT, u_int_16>::value)
        {
            return static_cast<size_t>(__builtin_popcount(static_cast<unsigned int>(v)));
        } else if constexpr (std::is_same<UT, u_int_32>::value)
        {
            return static_cast<size_t>(__builtin_popcount(static_cast<unsigned int>(v)));
        } else if constexpr (std::is_same<UT, u_int_64>::value)
        {
            return static_cast<size_t>(__builtin_popcountl(static_cast<unsigned long>(v)));
        }
    }

    /**
     * Returns one plus the index of the least significant 1-bit of aValue, or if aValue is zero, returns zero.
     */
    template<Concepts::IsIntegralUpTo64 T> [[nodiscard]] static constexpr
    size_t                      LeastSignificantBit (T aValue) noexcept
    {
        using UT = typename std::make_unsigned<T>::type;

        const UT v = std::bit_cast<UT>(aValue);

        if constexpr (std::is_same<UT, u_int_8>::value)
        {
            return static_cast<size_t>(__builtin_ffs(std::bit_cast<int>(v)));
        } else if constexpr (std::is_same<UT, u_int_16>::value)
        {
            return static_cast<size_t>(__builtin_ffs(std::bit_cast<int>(v)));
        } else if constexpr (std::is_same<UT, u_int_32>::value)
        {
            return static_cast<size_t>(__builtin_ffs(std::bit_cast<int>(v)));
        } else if constexpr (std::is_same<UT, u_int_64>::value)
        {
            return static_cast<size_t>(__builtin_ffsl(std::bit_cast<long>(v)));
        }
    }

    /**
     * Returns one plus the index of the most significant 1-bit of aValue, or if aValue is zero, returns zero.
     */
    template<Concepts::IsIntegralUpTo64 T> [[nodiscard]] static constexpr
    size_t                      MostSignificantBit  (T aValue) noexcept
    {
        return static_cast<size_t>(sizeof(T)*8) - Leading0bitCnt(aValue);
    }

    template<Concepts::IsIntegralUpTo64 T> [[nodiscard]] static constexpr
    T               MakeMaskHI          (void);

    template<Concepts::IsIntegralUpTo64 T> [[nodiscard]] static constexpr
    T               MakeMaskLO          (void);
};

[[nodiscard]] u_int_32  GpBitOperations::Interleave16_16 (const u_int_16 aLeft, const u_int_16 aRight) noexcept
{
#if defined(GP_CPU_USE_BMI2)
    return Interleave16_16_bmi(aLeft, aRight);
#else
    return Interleave16_16_std(aLeft, aRight);
#endif
}

[[nodiscard]] constexpr
u_int_32    GpBitOperations::Interleave16_16_std (const u_int_16 aLeft, const u_int_16 aRight) noexcept
{
#if defined(GP_ENVIRONMENT_32)
    u_int_32 l = u_int_32(aLeft);
    u_int_32 r = u_int_32(aRight);

    l = (l | (l << 8)) & u_int_32(0x00FF00FF);
    l = (l | (l << 4)) & u_int_32(0x0F0F0F0F);
    l = (l | (l << 2)) & u_int_32(0x33333333);
    l = (l | (l << 1)) & u_int_32(0x55555555);

    r = (r | (r << 8)) & u_int_32(0x00FF00FF);
    r = (r | (r << 4)) & u_int_32(0x0F0F0F0F);
    r = (r | (r << 2)) & u_int_32(0x33333333);
    r = (r | (r << 1)) & u_int_32(0x55555555);

    return r | (l << 1);
#elif defined(GP_ENVIRONMENT_64)
    u_int_64 lr = ((u_int_64(aLeft) << 32) | u_int_64(aRight));
    lr = (lr | (lr << 8)) & u_int_64(0x00FF00FF00FF00FF);
    lr = (lr | (lr << 4)) & u_int_64(0x0F0F0F0F0F0F0F0F);
    lr = (lr | (lr << 2)) & u_int_64(0x3333333333333333);
    lr = (lr | (lr << 1)) & u_int_64(0x5555555555555555);

    return u_int_32(lr | (lr >> 31));
#else
#   error Unknown environment bits capacity
#endif
}

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]]
    u_int_32    GpBitOperations::Interleave16_16_bmi (const u_int_16 aLeft, const u_int_16 aRight) noexcept
    {
#   if defined(GP_ENVIRONMENT_32)
        return _pdep_u32(u_int_32(aLeft),  u_int_32(0xAAAAAAAA)) |
               _pdep_u32(u_int_32(aRight), u_int_32(0x55555555));
#   elif defined(GP_ENVIRONMENT_64)
        return u_int_32(_pdep_u64(u_int_64(aLeft),  u_int_64(0x00000000AAAAAAAA)) |
                        _pdep_u64(u_int_64(aRight), u_int_64(0x0000000055555555)));
#   else
#       error Unknown environment bits capacity
#   endif
    }
#endif//GP_CPU_USE_BMI2

[[nodiscard]]
u_int_64    GpBitOperations::Interleave32_32 (const u_int_32 aLeft, const u_int_32 aRight) noexcept
{
#if defined(GP_CPU_USE_BMI2)
    return Interleave32_32_bmi(aLeft, aRight);
#else
    return Interleave32_32_std(aLeft, aRight);
#endif
}

[[nodiscard]] constexpr
u_int_64    GpBitOperations::Interleave32_32_std (const u_int_32 aLeft, const u_int_32 aRight) noexcept
{
    const u_int_16 leftLo   = u_int_16((aLeft >> 0) & 0xFFFF);
    const u_int_16 leftHi   = u_int_16((aLeft >> 16));
    const u_int_16 rightLo  = u_int_16((aRight >> 0) & 0xFFFF);
    const u_int_16 rightHi  = u_int_16((aRight >> 16));

    const u_int_64 resLo    = u_int_64(Interleave16_16_std(leftLo, rightLo));
    const u_int_64 resHi    = u_int_64(Interleave16_16_std(leftHi, rightHi));

    return (resHi << 32) | resLo;
}

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]]
    u_int_64    GpBitOperations::Interleave32_32_bmi (const u_int_32 aLeft, const u_int_32 aRight) noexcept
    {
        return u_int_64(_pdep_u64(u_int_64(aLeft),  u_int_64(0xAAAAAAAAAAAAAAAA)) |
                        _pdep_u64(u_int_64(aRight), u_int_64(0x5555555555555555)));
    }
#endif//GP_CPU_USE_BMI2

[[nodiscard]]
std::tuple<u_int_16/*left*/,
           u_int_16/*right*/>   GpBitOperations::Deinterleave16_16 (const u_int_32 aValue) noexcept
{
#if defined(GP_CPU_USE_BMI2)
    return Deinterleave16_16_bmi(aValue);
#else
    return Deinterleave16_16_std(aValue);
#endif
}

[[nodiscard]] constexpr
std::tuple<u_int_16/*left*/,
           u_int_16/*right*/>   GpBitOperations::Deinterleave16_16_std  (const u_int_32 aValue) noexcept
{
#if defined(GP_ENVIRONMENT_32)
    u_int_32 r = aValue;

    r = r & 0x55555555;
    r = (r | (r >> 1)) & u_int_32(0x33333333);
    r = (r | (r >> 2)) & u_int_32(0x0F0F0F0F);
    r = (r | (r >> 4)) & u_int_32(0x00FF00FF);
    r = (r | (r >> 8)) & u_int_32(0x0000FFFF);

    u_int_32 l = aValue >> 1;

    l = l & 0x55555555;
    l = (l | (l >> 1)) & u_int_32(0x33333333);
    l = (l | (l >> 2)) & u_int_32(0x0F0F0F0F);
    l = (l | (l >> 4)) & u_int_32(0x00FF00FF);
    l = (l | (l >> 8)) & u_int_32(0x0000FFFF);

    return {u_int_16(l) , u_int_16(r)};
#elif defined(GP_ENVIRONMENT_64)
    u_int_64 lr = ((u_int_64(aValue) & u_int_64(0xAAAAAAAA)) << 31) | (u_int_64(aValue) & u_int_64(0x55555555));
    lr = (lr | (lr >> 1)) & u_int_64(0x3333333333333333);
    lr = (lr | (lr >> 2)) & u_int_64(0x0F0F0F0F0F0F0F0F);
    lr = (lr | (lr >> 4)) & u_int_64(0x00FF00FF00FF00FF);
    lr = (lr | (lr >> 8)) & u_int_64(0x0000FFFF0000FFFF);

    return {u_int_16((lr >> 32) & 0xFFFF) , u_int_16(lr & 0xFFFF)};
#else
#   error Unknown environment bits capacity
#endif
}

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]]
    std::tuple<u_int_16/*left*/,
               u_int_16/*right*/>   GpBitOperations::Deinterleave16_16_bmi  (const u_int_32 aValue) noexcept
    {
#   if defined(GP_ENVIRONMENT_32)
        return {u_int_16(_pext_u32(aValue, u_int_32(0xAAAAAAAA))),
                u_int_16(_pext_u32(aValue, u_int_32(0x55555555)))};
#   elif defined(GP_ENVIRONMENT_64)
        return {u_int_16(_pext_u64(aValue, u_int_64(0x00000000AAAAAAAA))),
                u_int_16(_pext_u64(aValue, u_int_64(0x0000000055555555)))};
#   else
#       error Unknown environment bits capacity
#   endif
    }
#endif//GP_CPU_USE_BMI2

[[nodiscard]]
std::tuple<u_int_32/*left*/,
           u_int_32/*right*/>   GpBitOperations::Deinterleave32_32 (const u_int_64 aValue) noexcept
{
#if defined(GP_CPU_USE_BMI2)
    return Deinterleave32_32_bmi(aValue);
#else
    return Deinterleave32_32_std(aValue);
#endif
}

[[nodiscard]] constexpr
std::tuple<u_int_32/*left*/,
           u_int_32/*right*/>   GpBitOperations::Deinterleave32_32_std (const u_int_64 aValue) noexcept
{
    const u_int_32 lo   = u_int_32((aValue >> 0) & 0xFFFFFFFF);
    const u_int_32 hi   = u_int_32((aValue >> 32));

    const auto loPair   = Deinterleave16_16_std(lo);
    const auto hiPair   = Deinterleave16_16_std(hi);

    return {(std::get<0>(hiPair) << 16) | std::get<0>(loPair),
            (std::get<1>(hiPair) << 16) | std::get<1>(loPair)};
}

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]]
    std::tuple<u_int_32/*left*/,
               u_int_32/*right*/>   GpBitOperations::Deinterleave32_32_bmi (const u_int_64 aValue) noexcept
    {
        return {u_int_32(_pext_u64(aValue, u_int_64(0xAAAAAAAAAAAAAAAA))),
                u_int_32(_pext_u64(aValue, u_int_64(0x5555555555555555)))};
    }
#endif//GP_CPU_USE_BMI2

[[nodiscard]]
constexpr bool  GpBitOperations::Has0ByteIn32 (const u_int_32 aValue) noexcept
{
    return (aValue - u_int_32(0x01010101)) & (~aValue) & u_int_32(0x80808080);
}

[[nodiscard]]
constexpr bool  GpBitOperations::Has0ByteIn64 (const u_int_64 aValue) noexcept
{
    return (aValue - u_int_64(0x0101010101010101)) & (~aValue) & u_int_64(0x8080808080808080);
}

[[nodiscard]]
constexpr bool  GpBitOperations::HasSpecificByteIn32
(
    const u_int_32 aValue,
    const u_int_8  aSpecific
) noexcept
{
    return Has0ByteIn32(aValue ^ (~u_int_32(0)/u_int_32(255) * aSpecific));
}

[[nodiscard]]
constexpr bool  GpBitOperations::HasSpecificByteIn64
(
    const u_int_64 aValue,
    const u_int_8  aSpecific
) noexcept
{
    return Has0ByteIn64(aValue ^ (~u_int_64(0)/u_int_64(255) * aSpecific));
}

template<Concepts::IsIntegralUpTo64 T> [[nodiscard]] constexpr
T   GpBitOperations::MakeMaskHI (void)
{
    static_assert(std::is_unsigned_v<T>);

    constexpr T hi = MakeMaskLO<T>() << ((sizeof(T) * 8 / 2));
    return hi;
}

template<Concepts::IsIntegralUpTo64 T> [[nodiscard]] constexpr
T   GpBitOperations::MakeMaskLO (void)
{
    static_assert(std::is_unsigned_v<T>);

    constexpr T lo = (T(1) << ((sizeof(T) * 8 / 2))) - T(1);
    return lo;
}

using BitOps = GpBitOperations;

}//GPlatform
