#pragma once

#include "../Strings/GpStringTemplateArg.hpp"
#include "../Numerics/GpNumericOps.hpp"
#include "../Bits/GpBitCast.hpp"
#include <chrono>
#include <type_traits>
#include <functional>

namespace GPlatform {

template<typename T,
         size_t   N,
         typename UNIT_TYPE,
         typename SCALE,
         typename UNIT_NAME>
class GpUnit
{
public:
#if defined(GP_COMPILER_GCC)
    static_assert(std::chrono::__is_ratio<SCALE>::value, "Scale must be a specialization of std::ratio");
#endif
    static_assert(std::is_arithmetic<T>(), "T must be arithmetic");
    static_assert(SCALE::num > 0, "SCALE::num must be positive");

    using value_type        = T;
    using unit_type         = UNIT_TYPE;
    using unit_name         = UNIT_NAME;
    using container_type    = std::array<T, N>;
    using scale_ratio       = SCALE;
    using this_type         = GpUnit<T, N, UNIT_TYPE, SCALE, UNIT_NAME>;

    template<typename T_2, size_t N_2, typename UNIT_TYPE_2>
    using Convertible   = std::enable_if_t<   std::is_same_v<T, T_2>
                                           && (N_2 == N)
                                           && std::is_same_v<UNIT_TYPE, UNIT_TYPE_2>, int>;

    CLASS_TAG(GpUnit)
    CLASS_TAG_DETECTOR(GpUnit)

protected:
    constexpr           GpUnit      (const container_type aContainer) noexcept:
                                    iContainer(aContainer)
                                    {
                                    }

public:
    constexpr           GpUnit      (void) noexcept
                                    {iContainer.fill(T());}

    constexpr           GpUnit      (const this_type& aUnit) noexcept:
                                    iContainer(aUnit.iContainer)
                                    {}

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr           GpUnit      (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit):
                        iContainer(SFromUnit(aUnit))
                        {}

    [[nodiscard]] static std::string_view   SName (void) noexcept
    {
        return GpStringTemplateArgHolder<UNIT_NAME>::SAsStringView();
    }

    [[nodiscard]] std::string_view  Name (void) const noexcept
    {
        return SName();
    }

    [[nodiscard]] constexpr T   Value (void) const noexcept
    {
        return iContainer[0];
    }

    [[nodiscard]] constexpr const container_type&   Container (void) const noexcept
    {
        return iContainer;
    }

    [[nodiscard]] constexpr container_type& Container (void) noexcept
    {
        return iContainer;
    }

    [[nodiscard]] constexpr T   At (const size_t aId) const noexcept
    {
        return iContainer.at(aId);
    }

    [[nodiscard]] constexpr T   operator[] (const size_t aId) const noexcept
    {
        return iContainer[aId];
    }

    template<typename AsT>
    [[nodiscard]] constexpr AsT As (void) const
    {
        if constexpr (SHasTag_GpUnit<AsT>())
        {
            if constexpr(std::is_same_v<unit_type, typename AsT::unit_type>)
            {
                return AsT(*this);
            } else//unit_type != AsT::unit_type
            {
                return AsT::SMake(iContainer);
            }
        } else
        {
            if constexpr (N == 1)
            {
                return NumOps::SConvert<AsT, T>(iContainer[0]);
            } else
            {
                return SMakeArray(iContainer);
            }
        }
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr void Set (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit)
    {
        iContainer = SFromUnit(aUnit);
    }

    constexpr void Set (const this_type& aUnit) noexcept
    {
        iContainer = aUnit.Container();
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsEqual (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const
    {
        return Compare<SCALE_2, NumOps_IsEqual<T>>(aUnit.Container());
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsNotEqual (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const
    {
        return !IsEqual(aUnit);
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsLess (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const
    {
        return Compare<SCALE_2, NumOps_IsLess<T>>(aUnit.Container());
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsLessOrEqual(const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const
    {
        return Compare<SCALE_2, NumOps_IsLessOrEqual<T>>(aUnit.Container());
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsGrater (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const
    {
        return Compare<SCALE_2, NumOps_IsGreater<T>>(aUnit.Container());
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsGraterOrEqual (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const
    {
        return Compare<SCALE_2, NumOps_IsGreaterOrEqual<T>>(aUnit.Container());
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator== (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const noexcept
    {
        return IsEqual(aUnit);
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator!= (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const noexcept
    {
        return IsNotEqual(aUnit);
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator> (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const noexcept
    {
        return IsGrater(aUnit);
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator>= (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const noexcept
    {
        return IsGraterOrEqual(aUnit);
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator< (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const noexcept
    {
        return IsLess(aUnit);
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator<= (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit) const noexcept
    {
        return IsLessOrEqual(aUnit);
    }

    constexpr this_type&    operator++ (void)
    {
        Apply<NumOps_Inc<T>>();
        return *this;
    }

    constexpr this_type operator++ (int)
    {
        this_type old(*this);
        (*this).operator++();
        return old;
    }

    constexpr this_type&    operator-- (void)
    {
        Apply<NumOps_Dec<T>>();
        return *this;
    }

    constexpr this_type operator-- (int)
    {
        this_type old(*this);
        (*this).operator--();
        return old;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator=   (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit)
    {
        Set(aUnit);
        return *this;
    }

    constexpr this_type&    operator=   (const this_type& aUnit)
    {
        Set(aUnit);
        return *this;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator+=  (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit)
    {
        Apply<SCALE_2, NumOps_Add<T>>(aUnit.Container());

        return *this;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator-= (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit)
    {
        Apply<SCALE_2, NumOps_Sub<T>>(aUnit.Container());
        return *this;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator*= (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit)
    {
        Apply<SCALE_2, NumOps_Mul<T>>(aUnit.Container());
        return *this;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator/=  (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit)
    {
        Apply<SCALE_2, NumOps_Div<T>>(aUnit.Container());
        return *this;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator%= (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit)
    {
        Apply<SCALE_2, NumOps_Mod<T>>(aUnit.Container());
        return *this;
    }

    constexpr const this_type operator- (void)
    {
        Apply<NumOps_Negative<T>>();
        return *this;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr friend const this_type operator+ (const this_type& aLeft, const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aRight)
    {
        this_type res(aLeft);
        res += aRight;
        return res;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr friend const this_type operator-(const this_type& aLeft, const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aRight)
    {
        this_type res(aLeft);
        res -= aRight;
        return res;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr friend const this_type operator*(const this_type& aLeft, const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aRight)
    {
        this_type res(aLeft);
        res *= aRight;
        return res;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr friend const this_type operator/(const this_type& aLeft, const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aRight)
    {
        this_type res(aLeft);
        res /= aRight;
        return res;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr friend const this_type operator%(const this_type& aLeft, const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aRight)
    {
        this_type res(aLeft);
        res %= aRight;
        return res;
    }

    template<typename T_2,
             size_t   N_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, N_2, UNIT_TYPE_2> = 0>
    constexpr static container_type SFromUnit (const GpUnit<T_2, N_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aUnit)
    {
        container_type          res;
        constexpr const size_t  count = N;
        const container_type&   c = aUnit.Container();

        for (size_t id = 0; id < count; id++)
        {
            const auto val = c[id];
            res[id] = SConvertByRatio<SCALE_2>(val);
        }

        return res;
    }

    template<typename T_2>
    constexpr static this_type SMake (const T_2 aUnit)
    {
        static_assert(std::is_arithmetic<T_2>(), "T_2 must be arithmetic");
        return this_type(container_type{NumOps::SConvert<T>(aUnit)});
    }

    template<typename T_2,
             size_t   N_2,
             typename = std::enable_if_t<N == N_2>>
    constexpr static this_type SMake (const std::array<T_2, N_2> aUnit)
    {
        static_assert(std::is_arithmetic<T_2>(), "T_2 must be arithmetic");
        return this_type(SMakeArray(aUnit));
    }

private:
    template<typename T_2,
             size_t   N_2,
             typename = std::enable_if_t<N == N_2>>
    constexpr static container_type SMakeArray (const std::array<T_2, N_2> aContainer)
    {
        static_assert(std::is_arithmetic<T_2>(), "T_2 must be arithmetic");

        container_type          res;
        constexpr const size_t  count = N;

        for (size_t id = 0; id < count; id++)
        {
            res[id] = NumOps::SConvert<T>(aContainer[id]);
        }

        return res;
    }

    template <typename SCALE_2>
    constexpr static T  SConvertByRatio (const T aValue)
    {
        if constexpr (std::ratio_equal_v<SCALE, SCALE_2>)
        {
            return aValue;
        }

        using RatioT = std::ratio_divide<SCALE_2, SCALE>;

        if constexpr (std::is_floating_point<T>())
        {
            constexpr const double num = NumOps::SConvert<double>(RatioT::num);
            constexpr const double den = NumOps::SConvert<double>(RatioT::den);

            const double n = NumOps::SMul<double>(double(aValue), num);
            const double d = NumOps::SDiv<double>(n, den);

            return T(d);
        } else if constexpr (std::is_integral<T>())
        {
            using ratio_t = std::remove_cv_t<decltype(RatioT::num)>;

            const ratio_t v = NumOps::SConvert<ratio_t>(aValue);
            const ratio_t n = NumOps::SMul<ratio_t>(v, RatioT::num);
            const ratio_t d = NumOps::SDiv<ratio_t>(n, RatioT::den);

            return NumOps::SConvert<T>(d);
        }
    }

    template<typename SCALE_2,
             typename CompareFnT>
    [[nodiscard]] constexpr bool    Compare (const container_type& aContainer) const
    {
        CompareFnT              fn;
        constexpr const size_t  count   = N;
        bool                    res     = true;

        for (size_t id = 0; id < count; ++id)
        {
            res &= fn(iContainer[id], SConvertByRatio<SCALE_2>(aContainer[id]));
        }

        return res;
    }

    template<typename ApplyFnT>
    constexpr void  Apply (void)
    {
        ApplyFnT                fn;
        constexpr const size_t  count = N;

        for (size_t id = 0; id < count; ++id)
        {
            iContainer[id] = fn(iContainer[id]);
        }
    }

    template<typename SCALE_2,
             typename ApplyFnT>
    constexpr void  Apply (const container_type& aContainer)
    {
        ApplyFnT                fn;
        constexpr const size_t  count = N;

        for (size_t id = 0; id < count; ++id)
        {
            iContainer[id] = fn(iContainer[id], SConvertByRatio<SCALE_2>(aContainer[id]));
        }
    }

private:
    container_type  iContainer;
};

}//GPlatform

namespace std {

/*template<typename T,
         size_t   N,
         typename UNIT_TYPE,
         typename SCALE,
         typename UNIT_NAME> struct hash<::GPlatform::GpUnit<T, N, UNIT_TYPE, SCALE, UNIT_NAME>>
{
    using argument_type = ::GPlatform::GpUnit<T, N, UNIT_TYPE, SCALE, UNIT_NAME>;
    using result_type   = size_t;

    result_type operator()(argument_type const& aArg) const noexcept
    {
        return std::hash<typename argument_type::value_type>(aArg.Value());
    }
};*/

}//std
