#pragma once

#include "../../GpMacro.hpp"
#include "GpNumericTypes.hpp"
#include "../../Exceptions/GpCeExceptions.hpp"

#include <cmath>
#include <limits>
#include <stdexcept>

namespace GPlatform{

class GpNumericOps;

using NumOps = GpNumericOps;

class GpNumericOps
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpNumericOps)

    static_assert (sizeof(int) == sizeof(s_int_32), "sizeof(int) != sizeof (s_int_32)");
    static_assert (sizeof(long long int) == sizeof(s_int_64), "sizeof(long long int) == sizeof (s_int_64)");

public:
    [[nodiscard]] inline static
    size_t                          SDecDigsCountUI64 (const u_int_64 aValue) noexcept;

    [[nodiscard]] inline static
    size_t                          SDecDigsCountSI64 (const s_int_64 aValue) noexcept;

    template<typename T>
    [[nodiscard]] static size_t     SDecDigsCount (const T aValue) noexcept
    {
        static_assert(std::numeric_limits<T>::is_integer, "Value must be integer");

        size_t  res     = 1;
        T       value   = aValue;

        if ((std::is_signed<T>::value) && (value < 0))
        {
            for (;;)
            {
                if (value > T(-10)) return res;
                if (value > T(-100)) return res + 1;
                if (value > T(-1000)) return res + 2;
                if (value > T(-10000)) return res + 3;
                if (value > T(-100000)) return res + 4;

                value /= T(100000);
                res += 5;
            }
        } else
        {
            for (;;)
            {
                if (value < T(10)) return res;
                if (value < T(100)) return res + 1;
                if (value < T(1000)) return res + 2;
                if (value < T(10000)) return res + 3;
                if (value < T(100000)) return res + 4;

                value /= T(100000);
                res += 5;
            }
        }
    }

    template<typename T>
    [[nodiscard]] static constexpr T SSign (const T aValue) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        //return (T(0) < aValue) - (aValue < T(0));
        return std::copysign(T(1), aValue);
    }

    template<typename T>
    [[nodiscard]] static constexpr T SIsNegative (const T aValue) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        return std::signbit(aValue);
    }

    template<typename T>
    [[nodiscard]] static constexpr T SIsPositiveOrZero (const T aValue) noexcept
    {
        return !SIsNegative(aValue);
    }

    template<typename T>
    [[nodiscard]] static consteval T SMin (void) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        return std::numeric_limits<T>::min();
    }

    template<typename T>
    [[nodiscard]] static consteval T SMax (void) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        return std::numeric_limits<T>::max();
    }

    template<typename T>
    [[nodiscard]] static consteval T SMin (const T aValue) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        return std::numeric_limits<decltype(aValue)>::min();
    }

    template<typename T>
    [[nodiscard]] static consteval T SMax (const T aValue) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        return std::numeric_limits<decltype(aValue)>::max();
    }

    template<typename T>
    [[nodiscard]] static constexpr std::tuple<T, T> SMinMax (const T aValueA, const T aValueB) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if (aValueA > aValueB)
        {
            return {aValueB, aValueA};
        } else
        {
            return {aValueA, aValueB};
        }
    }

    template<typename T>
    [[nodiscard]] static constexpr std::tuple<T, T> SMinMaxFast (const T aValueMin, const T aValueMax, const T aValue) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if (aValue < aValueMin)
        {
            return {aValue, aValueMax};
        } else if (aValue > aValueMax)
        {
            return {aValueMin, aValue};
        } else
        {
            return {aValueMin, aValueMax};
        }
    }

    template<typename T>
    [[nodiscard]] static constexpr T SClamp (const T aValueMin, const T aValueMax, const T aValue) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if (aValue < aValueMin)
        {
            return aValueMin;
        } else if (aValue > aValueMax)
        {
            return aValueMax;
        } else
        {
            return aValue;
        }
    }

    template<typename T>
    [[nodiscard]] constexpr static bool SIsEqual (const T a, const T b) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if constexpr (std::is_floating_point<T>())
        {
            return SIsEqualFP<T>(a, b);
        } else if constexpr (std::is_integral<T>())
        {
            return a == b;
        }
    }

    template<typename T>
    [[nodiscard]] constexpr static bool SIsEqualToDelta (const T a, const T b, const T delta) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        return std::abs(SSub(a, b)) <= delta;
    }

    template<typename T>
    [[nodiscard]] constexpr static bool SIsNotEqual (const T a, const T b) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        return !SIsEqual(a, b);
    }

    template<typename T>
    [[nodiscard]] static constexpr bool SIsLess (const T a, const T b) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        return a < b;
    }

    template<typename T>
    [[nodiscard]] static constexpr bool SIsLessOrEqual (const T a, const T b) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if constexpr (std::is_floating_point<T>())
        {
            return (a < b) || SIsEqualFP<T>(a, b);
        } else if constexpr (std::is_integral<T>())
        {
            return a <= b;
        }
    }

    template<typename T>
    [[nodiscard]] static constexpr bool SIsGreater (const T a, const T b) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        return a > b;
    }

    template<typename T>
    [[nodiscard]] static constexpr bool SIsGreaterOrEqual (const T a, const T b) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if constexpr (std::is_floating_point<T>())
        {
            return (a > b) || SIsEqualFP<T>(a, b);
        } else if constexpr (std::is_integral<T>())
        {
            return a >= b;
        }
    }

    template<typename T>
    [[nodiscard]] static constexpr bool SIsBetween (const T a, const T aMin, const T aMax)
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        return    SIsGreaterOrEqual(a, aMin)
               && SIsLessOrEqual(a, aMax);
    }

    template<typename T>
    static constexpr T SInc (const T a)
    {
        return SAdd<T>(a, T(1));
    }

    template<typename T>
    static constexpr T SDec (const T a)
    {
        return SSub<T>(a, T(1));
    }

    template<typename T>
    [[nodiscard]] static constexpr T SAdd (const T a, const T b)
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if constexpr (std::is_floating_point<T>())
        {
            return a + b;
        } else //if constexpr (std::is_integral<T>())
        {
#if defined(BUILTIN_OVERFLOW_CONSTEXPR)
            T res = a;

            if (__builtin_add_overflow(a, b, &res))
            {
                GpThrowCe<std::out_of_range>("Add overflow");
            }

            return res;
#else
            return a + b;
#endif
        }
    }

    template<typename T>
    [[nodiscard]] static constexpr T SSub (const T a, const T b)
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if constexpr (std::is_floating_point<T>())
        {
            return a - b;
        } else //if constexpr (std::is_integral<T>())
        {
#if defined(BUILTIN_OVERFLOW_CONSTEXPR)
            T res = a;

            if (__builtin_sub_overflow(a, b, &res))
            {
                GpThrowCe<std::out_of_range>("Sub overflow");
            }

            return res;
#else
            return a - b;
#endif
        }
    }

    template<typename T>
    [[nodiscard]] static constexpr T SMul (const T a, const T b)
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if constexpr (std::is_floating_point<T>())
        {
            return a * b;
        } else //if constexpr (std::is_integral<T>())
        {
#if defined(BUILTIN_OVERFLOW_CONSTEXPR)
            T res = a;

            if (__builtin_mul_overflow(a, b, &res))
            {
                GpThrowCe<std::out_of_range>("Mul overflow");
            }

            return res;
#else
            return a * b;
#endif
        }
    }

    template<typename T>
    [[nodiscard]] static constexpr T SDiv (const T a, const T b)
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if (SIsNotEqual(b,T(0))) //b != 0
        {
            return a / b;
        } else
        {
            GpThrowCe<std::out_of_range>("Div by zero");
        }

        return 0;
    }

    template<typename T>
    [[nodiscard]] static constexpr T SDivCeil (const T a, const T b)
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if (SIsNotEqual(b,T(0))) //b != 0
        {
            if constexpr (std::is_floating_point<T>())
            {
                return std::ceil(a / b);
            } else if constexpr (std::is_integral<T>())
            {
                const T d = a / b;
                const T m = a % b;

                return d + ((m != 0) ? 1 : 0);
            }
        } else
        {
            GpThrowCe<std::out_of_range>("Div by zero");
        }

        return 0;
    }

    template<typename T>
    [[nodiscard]] static constexpr T SMod (const T a, const T b)
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");

        if (SIsNotEqual(b,T(0))) //b != 0
        {
            return a % b;
        } else
        {
            GpThrowCe<std::out_of_range>("Mod by zero");
        }

        return 0;
    }

    template<typename T>
    [[nodiscard]] static constexpr T SNegative (const T a) noexcept
    {
        static_assert(std::is_arithmetic<T>(), "T must be integral or floating point");
        static_assert(std::is_signed<T>(), "T must be signed");
        return -a;
    }

    template<typename TO, typename FROM>
    [[nodiscard]] static constexpr TO SConvertSafe (const FROM aValueFrom) noexcept
    {
        static_assert(std::is_arithmetic<FROM>(), "FROM must be integral or floating point");
        static_assert(std::is_arithmetic<TO>(), "TO must be integral or floating point");

        if constexpr(std::is_integral<FROM>() && std::is_integral<TO>())
        {
            using unsigned_from = std::make_unsigned_t<FROM>;
            using unsigned_to   = std::make_unsigned_t<TO>;
            using signed_from   = std::make_signed_t<FROM>;
            using signed_to     = std::make_signed_t<TO>;

            if constexpr(std::is_same<FROM, TO>())
            {
                return TO(aValueFrom);
            } else if constexpr ((unsigned_to(GpNumericOps::SMax<TO>()) >= unsigned_from(GpNumericOps::SMax<FROM>())) &&
                                 (signed_to(GpNumericOps::SMin<TO>()) <= signed_from(GpNumericOps::SMin<FROM>())))
            {
                return TO(aValueFrom);
            } else
            {
                GP_TEMPLATE_THROW(TO, "FROM is out of range of TO");
            }
        } else if constexpr(std::is_floating_point<FROM>() && std::is_floating_point<TO>())
        {
            return TO(aValueFrom);
        } else if constexpr(std::is_integral<FROM>() && std::is_floating_point<TO>())
        {
            return TO(aValueFrom);
        } else if constexpr(std::is_floating_point<FROM>() && std::is_integral<TO>())
        {
            if ((aValueFrom > FROM(GpNumericOps::SMax<TO>())) ||
                (aValueFrom < FROM(GpNumericOps::SMin<TO>())))
            {
                GpThrowCe<std::out_of_range>("GpNumericOps::SConvert out of range (B)");
            }

            return TO(aValueFrom);
        } else
        {
            GP_TEMPLATE_THROW(TO, "FROM and TO must be integral or float");
        }
    }

    template<typename TO, typename FROM>
    [[nodiscard]] static constexpr TO SConvert (const FROM aValueFrom)
    {
        static_assert(std::is_arithmetic<FROM>(), "FROM must be integral or floating point");
        static_assert(std::is_arithmetic<TO>(), "TO must be integral or floating point");

        if constexpr(std::is_integral<FROM>() && std::is_integral<TO>())
        {
            using unsigned_from = std::make_unsigned_t<FROM>;
            using unsigned_to   = std::make_unsigned_t<TO>;
            using signed_from   = std::make_signed_t<FROM>;
            using signed_to     = std::make_signed_t<TO>;

            if constexpr(std::is_same<FROM, TO>())
            {
                return TO(aValueFrom);
            } else if constexpr(unsigned_to(GpNumericOps::SMax<TO>()) >= unsigned_from(GpNumericOps::SMax<FROM>()))
            {
                if constexpr(signed_to(GpNumericOps::SMin<TO>()) <= signed_from(GpNumericOps::SMin<FROM>()))
                {
                    return TO(aValueFrom);
                } else
                {
                    //FROM(Signed) -> TO(Unsigned)
                    //Check TO lower bound

                    if (aValueFrom < FROM(0))
                    {
                        GpThrowCe<std::out_of_range>("GpNumericOps::SConvert out of range (A)");
                    }

                    return TO(aValueFrom);
                }
            } else
            {
                if constexpr(signed_to(GpNumericOps::SMin<TO>()) > signed_from(GpNumericOps::SMin<FROM>()))
                {
                    //Check TO upper bound
                    //Check TO lower bound
                    if ((aValueFrom > FROM(GpNumericOps::SMax<TO>())) ||
                        (aValueFrom < FROM(GpNumericOps::SMin<TO>())))
                    {
                        GpThrowCe<std::out_of_range>("GpNumericOps::SConvert out of range (B)");
                    }

                    return TO(aValueFrom);
                } else
                {
                    //Check TO upper bound
                    if (aValueFrom > FROM(GpNumericOps::SMax<TO>()))
                    {
                        GpThrowCe<std::out_of_range>("GpNumericOps::SConvert out of range (C)");
                    }

                    return TO(aValueFrom);
                }
            }
        } else if constexpr(std::is_floating_point<FROM>() && std::is_floating_point<TO>())
        {
            return TO(aValueFrom);
        } else if constexpr(std::is_integral<FROM>() && std::is_floating_point<TO>())
        {
            return TO(aValueFrom);
        } else if constexpr(std::is_floating_point<FROM>() && std::is_integral<TO>())
        {
            if ((aValueFrom > FROM(GpNumericOps::SMax<TO>())) ||
                (aValueFrom < FROM(GpNumericOps::SMin<TO>())))
            {
                GpThrowCe<std::out_of_range>("GpNumericOps::SConvert out of range (B)");
            }

            return TO(aValueFrom);
        } else
        {
            GP_TEMPLATE_THROW(TO, "FROM and TO must be integral or float");
        }
    }

private:
    template<typename T>
    [[nodiscard]] constexpr static bool SIsEqualFP (const T a, const T b) noexcept
    {
        constexpr T ulp = T(10);
        return std::abs(a - b) <= std::numeric_limits<T>::epsilon() * std::fmax(std::abs(a), std::abs(b)) * ulp;
    }
};

size_t  GpNumericOps::SDecDigsCountUI64 (const u_int_64 aValue) noexcept
{
    return SDecDigsCount<u_int_64>(aValue);
}

size_t  GpNumericOps::SDecDigsCountSI64 (const s_int_64 aValue) noexcept
{
    return SDecDigsCount<s_int_64>(aValue);
}

template<typename T> struct NumOps_IsEqual
{
    constexpr bool operator()(const T a, const T b) const noexcept
    {
        return NumOps::SIsEqual<T>(a, b);
    }
};

template<typename T> struct NumOps_IsNotEqual
{
    constexpr bool operator()(const T a, const T b) const noexcept
    {
        return NumOps::SIsNotEqual<T>(a, b);
    }
};

template<typename T> struct NumOps_IsLess
{
    constexpr bool operator()(const T a, const T b) const noexcept
    {
        return NumOps::SIsLess<T>(a, b);
    }
};

template<typename T> struct NumOps_IsLessOrEqual
{
    constexpr bool operator()(const T a, const T b) const noexcept
    {
        return NumOps::SIsLessOrEqual<T>(a, b);
    }
};

template<typename T> struct NumOps_IsGreater
{
    constexpr bool operator()(const T a, const T b) const noexcept
    {
        return NumOps::SIsGreater<T>(a, b);
    }
};

template<typename T> struct NumOps_IsGreaterOrEqual
{
    constexpr bool operator()(const T a, const T b) const noexcept
    {
        return NumOps::SIsGreaterOrEqual<T>(a, b);
    }
};

template<typename T> struct NumOps_Add
{
    constexpr T operator()(const T a, const T b) const noexcept
    {
        return NumOps::SAdd<T>(a, b);
    }
};

template<typename T> struct NumOps_Sub
{
    constexpr T operator()(const T a, const T b) const noexcept
    {
        return NumOps::SSub<T>(a, b);
    }
};

template<typename T> struct NumOps_Mul
{
    constexpr T operator()(const T a, const T b) const noexcept
    {
        return NumOps::SMul<T>(a, b);
    }
};

template<typename T> struct NumOps_Div
{
    constexpr T operator()(const T a, const T b) const noexcept
    {
        return NumOps::SDiv<T>(a, b);
    }
};

template<typename T> struct NumOps_Mod
{
    constexpr T operator()(const T a, const T b) const noexcept
    {
        return NumOps::SMod<T>(a, b);
    }
};

template<typename T> struct NumOps_Negative
{
    constexpr T operator()(const T aValue) const noexcept
    {
        return NumOps::SNegative<T>(aValue);
    }
};

template<typename T> struct NumOps_Inc
{
    constexpr T operator()(const T aValue) const noexcept
    {
        return NumOps::SInc<T>(aValue);
    }
};

template<typename T> struct NumOps_Dec
{
    constexpr T operator()(const T aValue) const noexcept
    {
        return NumOps::SDec<T>(aValue);
    }
};

}//GPlatform
