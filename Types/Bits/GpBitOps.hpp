#pragma once

#include "../../Config/GpConfig.hpp"
#include "../Classes/GpClassesDefines.hpp"
#include "../Numerics/GpNumericTypes.hpp"
#include "../Containers/GpContainersT.hpp"
#include "../../Exceptions/GpCeExceptions.hpp"
#include "GpBitCast.hpp"

#include <bitset>

#if defined(GP_CPU_USE_BMI2)
#	include <immintrin.h>
#endif

namespace GPlatform {

#if defined(GP_COMPILER_GCC) || defined(GP_COMPILER_CLANG)
#define	BSWAP16(x)	__builtin_bswap16(x)
#define	BSWAP32(x)	__builtin_bswap32(x)
#define	BSWAP64(x)	__builtin_bswap64(x)
#elif defined(GP_COMPILER_MSVC)
#define	BSWAP16(x)	_byteswap_ushort(x)
#define	BSWAP32(x)	_byteswap_ulong(x)
#define	BSWAP64(x)	_byteswap_uint64(x)
#else
#	error Unknown compiler
#endif

class GpBitOperations
{
private:
    CLASS_REMOVE_CTRS(GpBitOperations);

public:
    //------------------------------------ Network/Host byte order (N2H/H2N) --------------------------------
    template<typename T> [[nodiscard]] static
    T								BSwap (const T aValue)
    {
        static_assert(std::is_integral<T>::value, "T must be integral");
        static_assert((sizeof(T) == 1) ||
                      (sizeof(T) == 2) ||
                      (sizeof(T) == 4) ||
                      (sizeof(T) == 8), "sizeof(T) must be 2 or 4 or 8");

        typename std::make_unsigned<T>::type tmpVal;
        tmpVal = std::bit_cast<decltype(tmpVal)>(aValue);

        if constexpr(sizeof(T) == 2)		tmpVal = BSWAP16(tmpVal);
        else if constexpr(sizeof(T) == 4)	tmpVal = BSWAP32(tmpVal);
        else if constexpr(sizeof(T) == 8)	tmpVal = BSWAP64(tmpVal);

        return std::bit_cast<T>(tmpVal);
    }

    [[nodiscard]] static double		BSwap (const double aValue)
    {
        static_assert(sizeof(double) == sizeof(u_int_64), "sizeof(double) != sizeof(u_int_64)");

        const u_int_64 tmpVal = std::bit_cast<u_int_64>(aValue);
        return std::bit_cast<double>(BSWAP64(tmpVal));
    }

    [[nodiscard]] static float		BSwap (float aValue)
    {
        static_assert(sizeof(float) == sizeof(u_int_32), "sizeof(float) != sizeof(u_int_32)");

        const u_int_32 tmpVal = std::bit_cast<u_int_32>(aValue);
        return std::bit_cast<float>(BSWAP32(tmpVal));
    }

    template<typename T> [[nodiscard]] static
    T								N2H (T aValue)
    {
#if defined(GP_ORDER_LITTLE_ENDIAN)
        return BSwap(aValue);
#else
        return aValue;
#endif
    }

    template<typename T> [[nodiscard]] static
    T								H2N (T aValue)
    {
        return N2H(aValue);
    }

    //------------------------------------ Interleave16_16(Morton Codes) ------------------------------------
    template<u_int_16 Left, u_int_16 Right> [[nodiscard]] static constexpr
    u_int_32						Interleave16_16 (void) noexcept
    {
        return Interleave16_16_std(Left, Right);
    }

    [[nodiscard]] static
    inline u_int_32					Interleave16_16		(const u_int_16 aLeft, const u_int_16 aRight) noexcept;

    [[nodiscard]] static constexpr
    inline u_int_32					Interleave16_16_std	(const u_int_16 aLeft, const u_int_16 aRight) noexcept;

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]] static
    inline  u_int_32				Interleave16_16_bmi	(const u_int_16 aLeft, const u_int_16 aRight) noexcept;
#endif//GP_CPU_USE_BMI2

    //------------------------------------ Interleave32_32(Morton Codes) ------------------------------------
    template<u_int_32 Left, u_int_32 Right> [[nodiscard]] static constexpr
    u_int_64						Interleave32_32 (void) noexcept
    {
        return Interleave32_32_std(Left, Right);
    }

    [[nodiscard]] static
    inline u_int_64				Interleave32_32		(const u_int_32 aLeft, const u_int_32 aRight) noexcept;

    [[nodiscard]] static constexpr
    inline u_int_64				Interleave32_32_std	(const u_int_32 aLeft, const u_int_32 aRight) noexcept;

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]] static
    inline u_int_64				Interleave32_32_bmi	(const u_int_32 aLeft, const u_int_32 aRight) noexcept;
#endif//GP_CPU_USE_BMI2

    //------------------------------------ Deinterleave16_16(Morton Codes) ------------------------------------
    template<u_int_32 Value> [[nodiscard]] static constexpr
    GpTuple<u_int_16/*left*/,
            u_int_16/*right*/>	Deinterleave16_16		(void) noexcept
    {
        return Deinterleave16_16_std(Value);
    }

    [[nodiscard]] static inline
    GpTuple<u_int_16/*left*/,
            u_int_16/*right*/>	Deinterleave16_16		(const u_int_32 aValue) noexcept;

    [[nodiscard]] static constexpr inline
    GpTuple<u_int_16/*left*/,
            u_int_16/*right*/>	Deinterleave16_16_std	(const u_int_32 aValue) noexcept;

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]] static inline
    GpTuple<u_int_16/*left*/,
            u_int_16/*right*/>	Deinterleave16_16_bmi	(const u_int_32 aValue) noexcept;
#endif//GP_CPU_USE_BMI2

    //------------------------------------ Deinterleave32_32(Morton Codes) ------------------------------------
    template<u_int_64 Value> [[nodiscard]] static constexpr
    GpTuple<u_int_32/*left*/,
            u_int_32/*right*/>	Deinterleave32_32		(void) noexcept
    {
        return Deinterleave32_32_std(Value);
    }

    [[nodiscard]] static inline
    GpTuple<u_int_32/*left*/,
            u_int_32/*right*/>	Deinterleave32_32		(const u_int_64 aValue) noexcept;

    [[nodiscard]] static constexpr inline
    GpTuple<u_int_32/*left*/,
            u_int_32/*right*/>	Deinterleave32_32_std	(const u_int_64 aValue) noexcept;

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]] static inline
    GpTuple<u_int_32/*left*/,
            u_int_32/*right*/>	Deinterleave32_32_bmi	(const u_int_64 aValue) noexcept;
#endif//GP_CPU_USE_BMI2

    //----------------------------- Determine if a word has a byte = 0 -------------------------
    [[nodiscard]] static constexpr inline
    bool							HasZeroByteIn32			(const u_int_32 aValue) noexcept;

    [[nodiscard]] static constexpr inline
    bool							HasZeroByteIn64			(const u_int_64 aValue) noexcept;

    //----------------------------- Determine if a word has a byte = V -------------------------
    [[nodiscard]] static constexpr inline
    bool							HasSpecificByteIn32		(const u_int_32 aValue,
                                                             const u_int_8  aSpecific) noexcept;

    [[nodiscard]] static constexpr inline
    bool							HasSpecificByteIn64		(const u_int_64 aValue,
                                                             const u_int_8  aSpecific) noexcept;

    //-------------------------------------------------------------------------------------------
    template<typename T> [[nodiscard]] static constexpr
    T								SetByMask			(const T aValue,
                                                         const T aValueToSet,
                                                         const T aBitMask,
                                                         const T aOffset) noexcept
    {
        return T((aValue & ~(aBitMask << aOffset)) | ((aValueToSet & aBitMask) << aOffset));
    }

    template<typename T> [[nodiscard]] static constexpr
    T								And					(const T aValue, const T aBitMask) noexcept
    {
        return T(aValue & aBitMask);
    }

    template<typename T> [[nodiscard]] static constexpr
    T								Up					(const T aValue, const T aBitMask) noexcept
    {
        return T(aValue | aBitMask);
    }

    template<typename T> [[nodiscard]] static constexpr
    T								Down				(const T aValue, const T aBitMask) noexcept
    {
        return T(aValue & ~(aBitMask));
    }

    template<typename T> [[nodiscard]] static constexpr
    T								UpAndDown			(const T aValue, const T aUpBitMask, const T aDownBitMask) noexcept
    {
        return Up(Down(aValue, aDownBitMask), aUpBitMask);
    }

    template<typename T> [[nodiscard]] static constexpr
    T								Invert				(const T aValue, const T aBitMask) noexcept
    {
        return T(aValue ^ aBitMask);
    }

    template<typename T> [[nodiscard]] static constexpr
    bool							TestAny				(const T aValue, const T aBitMask) noexcept
    {
        return T(aValue & aBitMask);
    }

    template<typename T> [[nodiscard]] static constexpr
    bool							TestAll				(const T aValue, const T aBitMask) noexcept
    {
        return T((aValue & aBitMask) == aBitMask);
    }

    template<typename T> [[nodiscard]] static constexpr
    bool							Match				(const T aValueA, const T aValueB, const T aBitMask) noexcept
    {
        return (aValueA & aBitMask) == (aValueB & aBitMask);
    }

    template<typename TValue, typename TShift> [[nodiscard]] static constexpr
    TValue							SHL					(TValue aValue, TShift aShift) noexcept
    {
        return TValue(aValue << size_t(aShift));
    }

    template<typename TValue, typename TShift> [[nodiscard]] static constexpr
    TValue							SHR					(TValue aValue, TShift aShift) noexcept
    {
        return TValue(aValue >> size_t(aShift));
    }

    template<typename T> [[nodiscard]] static constexpr
    count_t							LeadingZeroCount	(T aValue) noexcept
    {
        using UT = typename std::make_unsigned<T>::type;

        static_assert(std::is_integral<T>(), "Type must be integral");
        static_assert(sizeof(T) <= sizeof(unsigned long long), "Type size is too big");
        static_assert(sizeof(T) == sizeof(UT), "Signed and unsigned types must be the same size");

        const UT v = std::bit_cast<UT>(aValue);

        if (v == UT(0))
        {
            return count_t::SMake(sizeof(UT) * 8);
        }

        if constexpr (std::is_same<UT, u_int_8>::value)
        {
            return count_t::SMake(size_t(__builtin_clz(static_cast<unsigned int>(v))) - (sizeof(unsigned int) - sizeof(u_int_8))*8);
        } else if constexpr (std::is_same<UT, u_int_16>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_clz(static_cast<unsigned int>(v))) - (sizeof(unsigned int) - sizeof(u_int_16))*8);
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_clzl(static_cast<unsigned long>(v))) - (sizeof(unsigned long) - sizeof(u_int_16))*8);
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_clzll(static_cast<unsigned long long>(v))) - (sizeof(unsigned long long) - sizeof(u_int_16))*8);
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else if constexpr (std::is_same<UT, u_int_32>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_clz(static_cast<unsigned int>(v))) - (sizeof(unsigned int) - sizeof(u_int_32))*8);
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_clzl(static_cast<unsigned long>(v))) - (sizeof(unsigned long) - sizeof(u_int_32))*8);
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_clzll(static_cast<unsigned long long>(v))) - (sizeof(unsigned long long) - sizeof(u_int_32))*8);
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else if constexpr (std::is_same<UT, u_int_64>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_clz(static_cast<unsigned int>(v))) - (sizeof(unsigned int) - sizeof(u_int_64))*8);
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_clzl(static_cast<unsigned long>(v))) - (sizeof(unsigned long) - sizeof(u_int_64))*8);
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_clzll(static_cast<unsigned long long>(v))) - (sizeof(unsigned long long) - sizeof(u_int_64))*8);
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else
        {
            GP_TEMPLATE_THROW(T, "Unsupported type");
        }
    }

    template<typename T> [[nodiscard]] static constexpr
    count_t						TrailingZeroCount	(T aValue) noexcept
    {
        using UT = typename std::make_unsigned<T>::type;

        static_assert(std::is_integral<T>(), "Type must be integral");
        static_assert(sizeof(T) <= sizeof(unsigned long long), "Type size is too big");
        static_assert(sizeof(T) == sizeof(UT), "Signed and unsigned types must be the same size");

        const UT v = std::bit_cast<UT>(aValue);

        if (v == UT(0))
        {
            return count_t::SMake(sizeof(UT)*8);
        }

        if constexpr (std::is_same<UT, u_int_8>::value)
        {
            return count_t::SMake(size_t(__builtin_ctz(static_cast<unsigned int>(v))));
        } else if constexpr (std::is_same<UT, u_int_16>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_ctz(static_cast<unsigned int>(v))));
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_ctzl(static_cast<unsigned long>(v))));
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_ctzll(static_cast<unsigned long long>(v))));
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else if constexpr (std::is_same<UT, u_int_32>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_ctz(static_cast<unsigned int>(v))));
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_ctzl(static_cast<unsigned long>(v))));
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_ctzll(static_cast<unsigned long long>(v))));
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else if constexpr (std::is_same<UT, u_int_64>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_ctz(static_cast<unsigned int>(v))));
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_ctzl(static_cast<unsigned long>(v))));
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_ctzll(static_cast<unsigned long long>(v))));
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else
        {
            GP_TEMPLATE_THROW(T, "Unsupported type");
        }
    }

    template<typename T> [[nodiscard]] static constexpr
    count_t						PopCount	(T aValue) noexcept
    {
        using UT = typename std::make_unsigned<T>::type;

        static_assert(std::is_integral<T>(), "Type must be integral");
        static_assert(sizeof(T) <= sizeof(unsigned long long), "Type size is too big");
        static_assert(sizeof(T) == sizeof(UT), "Signed and unsigned types must be the same size");

        const UT v = std::bit_cast<UT>(aValue);

        if constexpr (std::is_same<UT, u_int_8>::value)
        {
            return count_t::SMake(size_t(__builtin_popcount(static_cast<unsigned int>(v))));
        } else if constexpr (std::is_same<UT, u_int_16>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_popcount(static_cast<unsigned int>(v))));
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_popcountl(static_cast<unsigned long>(v))));
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_popcountll(static_cast<unsigned long long>(v))));
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else if constexpr (std::is_same<UT, u_int_32>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_popcount(static_cast<unsigned int>(v))));
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_popcountl(static_cast<unsigned long>(v))));
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_popcountll(static_cast<unsigned long long>(v))));
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else if constexpr (std::is_same<UT, u_int_64>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_popcount(static_cast<unsigned int>(v))));
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_popcountl(static_cast<unsigned long>(v))));
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_popcountll(static_cast<unsigned long long>(v))));
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else
        {
            GP_TEMPLATE_THROW(T, "Unsupported type");
        }
    }

    /**
     * Returns one plus the index of the least significant 1-bit of aValue, or if aValue is zero, returns zero.
     */
    template<typename T> [[nodiscard]] static constexpr
    count_t						LeastSignificantBit	(T aValue) noexcept
    {
        using UT = typename std::make_unsigned<T>::type;

        static_assert(std::is_integral<T>(), "Type must be integral");
        static_assert(sizeof(T) <= sizeof(unsigned long long), "Type size is too big");
        static_assert(sizeof(T) == sizeof(UT), "Signed and unsigned types must be the same size");

        const UT v = std::bit_cast<UT>(aValue);

        if constexpr (std::is_same<UT, u_int_8>::value)
        {
            return count_t::SMake(size_t(__builtin_ffs(static_cast<unsigned int>(v))));
        } else if constexpr (std::is_same<UT, u_int_16>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_ffs(static_cast<unsigned int>(v))));
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_ffsl(static_cast<unsigned long>(v))));
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_16))
            {
                return count_t::SMake(size_t(__builtin_ffsll(static_cast<unsigned long long>(v))));
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else if constexpr (std::is_same<UT, u_int_32>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_ffs(static_cast<unsigned int>(v))));
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_ffsl(static_cast<unsigned long>(v))));
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_32))
            {
                return count_t::SMake(size_t(__builtin_ffsll(static_cast<unsigned long long>(v))));
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else if constexpr (std::is_same<UT, u_int_64>::value)
        {
            if constexpr (sizeof(unsigned int) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_ffs(static_cast<unsigned int>(v))));
            } else if constexpr (sizeof(unsigned long) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_ffsl(static_cast<unsigned long>(v))));
            } else if constexpr (sizeof(unsigned long long) >= sizeof(u_int_64))
            {
                return count_t::SMake(size_t(__builtin_ffsll(static_cast<unsigned long long>(v))));
            } else
            {
                GP_TEMPLATE_THROW(T, "Unsupported type");
            }
        } else
        {
            GP_TEMPLATE_THROW(T, "Unsupported type");
        }
    }

    /**
     * Returns one plus the index of the most significant 1-bit of aValue, or if aValue is zero, returns zero.
     */
    template<typename T> [[nodiscard]] static constexpr
    count_t						MostSignificantBit	(T aValue) noexcept
    {
        return count_t::SMake(sizeof(T)*8) - LeadingZeroCount(aValue);
    }
};

[[nodiscard]] u_int_32	GpBitOperations::Interleave16_16 (const u_int_16 aLeft, const u_int_16 aRight) noexcept
{
#if defined(GP_CPU_USE_BMI2)
    return Interleave16_16_bmi(aLeft, aRight);
#else
    return Interleave16_16_std(aLeft, aRight);
#endif
}

[[nodiscard]] constexpr
u_int_32	GpBitOperations::Interleave16_16_std (const u_int_16 aLeft, const u_int_16 aRight) noexcept
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
#	error Unknown environment bits capacity
#endif
}

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]]
    u_int_32	GpBitOperations::Interleave16_16_bmi (const u_int_16 aLeft, const u_int_16 aRight) noexcept
    {
#	if defined(GP_ENVIRONMENT_32)
        return _pdep_u32(u_int_32(aLeft),  u_int_32(0xAAAAAAAA)) |
               _pdep_u32(u_int_32(aRight), u_int_32(0x55555555));
#	elif defined(GP_ENVIRONMENT_64)
        return u_int_32(_pdep_u64(u_int_64(aLeft),  u_int_64(0x00000000AAAAAAAA)) |
                        _pdep_u64(u_int_64(aRight), u_int_64(0x0000000055555555)));
#	else
#		error Unknown environment bits capacity
#	endif
    }
#endif//GP_CPU_USE_BMI2

[[nodiscard]]
u_int_64	GpBitOperations::Interleave32_32 (const u_int_32 aLeft, const u_int_32 aRight) noexcept
{
#if defined(GP_CPU_USE_BMI2)
    return Interleave32_32_bmi(aLeft, aRight);
#else
    return Interleave32_32_std(aLeft, aRight);
#endif
}

[[nodiscard]] constexpr
u_int_64	GpBitOperations::Interleave32_32_std (const u_int_32 aLeft, const u_int_32 aRight) noexcept
{
    const u_int_16 leftLo	= u_int_16((aLeft >> 0) & 0xFFFF);
    const u_int_16 leftHi	= u_int_16((aLeft >> 16));
    const u_int_16 rightLo	= u_int_16((aRight >> 0) & 0xFFFF);
    const u_int_16 rightHi	= u_int_16((aRight >> 16));

    const u_int_64 resLo	= u_int_64(Interleave16_16_std(leftLo, rightLo));
    const u_int_64 resHi	= u_int_64(Interleave16_16_std(leftHi, rightHi));

    return (resHi << 32) | resLo;
}

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]]
    u_int_64	GpBitOperations::Interleave32_32_bmi (const u_int_32 aLeft, const u_int_32 aRight) noexcept
    {
        return u_int_64(_pdep_u64(u_int_64(aLeft),  u_int_64(0xAAAAAAAAAAAAAAAA)) |
                        _pdep_u64(u_int_64(aRight), u_int_64(0x5555555555555555)));
    }
#endif//GP_CPU_USE_BMI2

[[nodiscard]]
GpTuple<u_int_16/*left*/,
        u_int_16/*right*/>	GpBitOperations::Deinterleave16_16 (const u_int_32 aValue) noexcept
{
#if defined(GP_CPU_USE_BMI2)
    return Deinterleave16_16_bmi(aValue);
#else
    return Deinterleave16_16_std(aValue);
#endif
}

[[nodiscard]] constexpr
GpTuple<u_int_16/*left*/,
        u_int_16/*right*/>	GpBitOperations::Deinterleave16_16_std	(const u_int_32 aValue) noexcept
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
#	error Unknown environment bits capacity
#endif
}

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]]
    GpTuple<u_int_16/*left*/,
            u_int_16/*right*/>	GpBitOperations::Deinterleave16_16_bmi	(const u_int_32 aValue) noexcept
    {
#	if defined(GP_ENVIRONMENT_32)
        return {u_int_16(_pext_u32(aValue, u_int_32(0xAAAAAAAA))),
                u_int_16(_pext_u32(aValue, u_int_32(0x55555555)))};
#	elif defined(GP_ENVIRONMENT_64)
        return {u_int_16(_pext_u64(aValue, u_int_64(0x00000000AAAAAAAA))),
                u_int_16(_pext_u64(aValue, u_int_64(0x0000000055555555)))};
#	else
#		error Unknown environment bits capacity
#	endif
    }
#endif//GP_CPU_USE_BMI2

[[nodiscard]]
GpTuple<u_int_32/*left*/,
        u_int_32/*right*/>	GpBitOperations::Deinterleave32_32 (const u_int_64 aValue) noexcept
{
#if defined(GP_CPU_USE_BMI2)
    return Deinterleave32_32_bmi(aValue);
#else
    return Deinterleave32_32_std(aValue);
#endif
}

[[nodiscard]] constexpr
GpTuple<u_int_32/*left*/,
        u_int_32/*right*/>	GpBitOperations::Deinterleave32_32_std (const u_int_64 aValue) noexcept
{
    const u_int_32 lo	= u_int_32((aValue >> 0) & 0xFFFFFFFF);
    const u_int_32 hi	= u_int_32((aValue >> 32));

    const auto loPair	= Deinterleave16_16_std(lo);
    const auto hiPair	= Deinterleave16_16_std(hi);

    return {(std::get<0>(hiPair) << 16) | std::get<0>(loPair),
            (std::get<1>(hiPair) << 16) | std::get<1>(loPair)};
}

#if defined(GP_CPU_USE_BMI2)
    [[nodiscard]]
    GpTuple<u_int_32/*left*/,
            u_int_32/*right*/>	GpBitOperations::Deinterleave32_32_bmi (const u_int_64 aValue) noexcept
    {
        return {u_int_32(_pext_u64(aValue, u_int_64(0xAAAAAAAAAAAAAAAA))),
                u_int_32(_pext_u64(aValue, u_int_64(0x5555555555555555)))};
    }
#endif//GP_CPU_USE_BMI2

[[nodiscard]]
constexpr bool	GpBitOperations::HasZeroByteIn32 (const u_int_32 aValue) noexcept
{
    return (aValue - u_int_32(0x01010101)) & (~aValue) & u_int_32(0x80808080);
}

[[nodiscard]]
constexpr bool	GpBitOperations::HasZeroByteIn64 (const u_int_64 aValue) noexcept
{
    return (aValue - u_int_64(0x0101010101010101)) & (~aValue) & u_int_64(0x8080808080808080);
}

[[nodiscard]]
constexpr bool	GpBitOperations::HasSpecificByteIn32 (const u_int_32 aValue,
                                                      const u_int_8  aSpecific) noexcept
{
    return HasZeroByteIn32(aValue ^ (~u_int_32(0)/u_int_32(255) * aSpecific));
}

[[nodiscard]]
constexpr bool	GpBitOperations::HasSpecificByteIn64 (const u_int_64 aValue,
                                                      const u_int_8  aSpecific) noexcept
{
    return HasZeroByteIn64(aValue ^ (~u_int_64(0)/u_int_64(255) * aSpecific));
}

using BitOps = GpBitOperations;

}//GPlatform
