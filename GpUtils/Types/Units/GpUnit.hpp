#pragma once

#include "../../../Config/GpConfig.hpp"
#include "../../Macro/GpMacroTags.hpp"
#include "../Strings/GpStringTemplateArg.hpp"
#include "../Numerics/GpNumericOps.hpp"

#include <chrono>
#include <type_traits>

namespace GPlatform {

TAG_REGISTER(GpUnit)

namespace Concepts::Unit {

template <typename T>
concept IsUnit = requires()
{
    requires GpHasTag_GpUnit<T>();
};

template <typename FROM, typename TO>
concept IsConvertable = requires()
{
    typename FROM::value_type;
    typename FROM::unit_type;
    typename TO::value_type;
    typename TO::unit_type;

    requires
       std::is_same_v<typename FROM::value_type, typename TO::value_type>
    && std::is_same_v<typename FROM::unit_type, typename TO::unit_type>
    && GpHasTag_GpUnit<FROM>();
};

}//namespace Concepts::Unit

template<Concepts::IsArithmetic T,
         typename               UNIT_TYPE,
         typename               SCALE,
         typename               UNIT_NAME>
class GpUnit
{
public:
#if defined(GP_COMPILER_GCC)
    static_assert(std::chrono::__is_ratio<SCALE>::value, "Scale must be a specialization of std::ratio");
#endif

    static_assert(SCALE::num > 0, "SCALE::num must be positive");

    using value_type    = T;
    using unit_type     = UNIT_TYPE;
    using scale_ratio   = SCALE;
    using this_type     = GpUnit<T, UNIT_TYPE, SCALE, UNIT_NAME>;

    TAG_SET(GpUnit)

protected:
    constexpr           GpUnit      (const value_type aValue) noexcept:
                                    iValue(aValue)
                                    {}

public:
    constexpr           GpUnit      (void) noexcept:iValue()
                                    {}

    constexpr explicit  GpUnit      (const this_type& aUnit) noexcept:
                                    iValue(aUnit.iValue)
                                    {}

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr           GpUnit      (const T2& aUnit):
                        iValue(SFromUnit<T2>(aUnit))
                        {}

    [[nodiscard]] static std::u8string_view SName (void) noexcept
    {
        return GpStringTemplateArgHolder<UNIT_NAME>::SAsStringViewU8();
    }

    [[nodiscard]] std::u8string_view    Name (void) const noexcept
    {
        return SName();
    }

    [[nodiscard]] constexpr T   Value (void) const noexcept
    {
        return iValue;
    }

    [[nodiscard]] constexpr T&  Value (void) noexcept
    {
        return iValue;
    }

    template<typename TO>
    requires Concepts::IsArithmetic<TO> || Concepts::Unit::IsConvertable<TO, this_type>
    [[nodiscard]] constexpr TO As (void) const
    {
        if constexpr (GpHasTag_GpUnit<TO>())
        {
            return TO(*this);
        } else
        {
            return NumOps::SConvert<TO, T>(iValue);
        }
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr this_type& Set (const T2& aUnit)
    {
        iValue = SFromUnit(aUnit);
        return *this;
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    IsEqual (const T2& aUnit) const
    {
        return Compare<typename T2::scale_ratio, NumOps_IsEqual<T>>(aUnit.Value());
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    IsNotEqual (const T2& aUnit) const
    {
        return !IsEqual(aUnit);
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    IsLess (const T2& aUnit) const
    {
        return Compare<typename T2::scale_ratio, NumOps_IsLess<T>>(aUnit.Value());
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    IsLessOrEqual(const T2& aUnit) const
    {
        return Compare<typename T2::scale_ratio, NumOps_IsLessOrEqual<T>>(aUnit.Value());
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    IsGrater (const T2& aUnit) const
    {
        return Compare<typename T2::scale_ratio, NumOps_IsGreater<T>>(aUnit.Value());
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    IsGraterOrEqual (const T2& aUnit) const
    {
        return Compare<typename T2::scale_ratio, NumOps_IsGreaterOrEqual<T>>(aUnit.Value());
    }

    constexpr this_type&    operator= (const this_type& aUnit) noexcept
    {
        return Set(aUnit);
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr this_type&    operator= (const T2& aUnit) noexcept
    {
        return Set(aUnit);
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    operator== (const T2& aUnit) const noexcept
    {
        return IsEqual(aUnit);
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    operator!= (const T2& aUnit) const noexcept
    {
        return IsNotEqual(aUnit);
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    operator> (const T2& aUnit) const noexcept
    {
        return IsGrater(aUnit);
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    operator>= (const T2& aUnit) const noexcept
    {
        return IsGraterOrEqual(aUnit);
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    operator< (const T2& aUnit) const noexcept
    {
        return IsLess(aUnit);
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    [[nodiscard]] constexpr bool    operator<= (const T2& aUnit) const noexcept
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

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr this_type&    operator+=  (const T2& aUnit)
    {
        Apply<typename T2::scale_ratio, NumOps_Add<T>>(aUnit.Value());

        return *this;
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr this_type&    operator-= (const T2& aUnit)
    {
        Apply<typename T2::scale_ratio, NumOps_Sub<T>>(aUnit.Value());
        return *this;
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr this_type&    operator*= (const T2& aUnit)
    {
        Apply<typename T2::scale_ratio, NumOps_Mul<T>>(aUnit.Value());
        return *this;
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr this_type&    operator/=  (const T2& aUnit)
    {
        Apply<typename T2::scale_ratio, NumOps_Div<T>>(aUnit.Value());
        return *this;
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr this_type&    operator%= (const T2& aUnit)
    {
        Apply<typename T2::scale_ratio, NumOps_Mod<T>>(aUnit.Value());
        return *this;
    }

    constexpr const this_type operator- (void)
    {
        Apply<NumOps_Negative<T>>();
        return *this;
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr friend this_type operator+ (const this_type& aLeft, const T2& aRight)
    {
        this_type res(aLeft);
        res += aRight;
        return res;
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr friend this_type operator-(const this_type& aLeft, const T2& aRight)
    {
        this_type res(aLeft);
        res -= aRight;
        return res;
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr friend this_type operator*(const this_type& aLeft, const T2& aRight)
    {
        this_type res(aLeft);
        res *= aRight;
        return res;
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr friend this_type operator/(const this_type& aLeft, const T2& aRight)
    {
        this_type res(aLeft);
        res /= aRight;
        return res;
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    constexpr friend this_type operator%(const this_type& aLeft, const T2& aRight)
    {
        this_type res(aLeft);
        res %= aRight;
        return res;
    }

    template<Concepts::Unit::IsUnit T2>
    requires Concepts::Unit::IsConvertable<T2, this_type>
    static constexpr value_type SFromUnit (const T2& aUnit)
    {
        return SConvertByRatio<typename T2::scale_ratio>(aUnit.Value());
    }

    template<typename T_2>
    static constexpr this_type SMake (const T_2 aUnit)
    {
        static_assert(std::is_arithmetic<T_2>(), "T_2 must be arithmetic");
        return this_type(NumOps::SConvert<T>(aUnit));
    }

private:
    template <typename SCALE_RATIO>
    static constexpr T  SConvertByRatio (const T aValue)
    {
        if constexpr (std::ratio_equal_v<SCALE, SCALE_RATIO>)
        {
            return aValue;
        }

        using RatioT = std::ratio_divide<SCALE_RATIO, SCALE>;

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

    template<typename SCALE_RATIO,
             typename CompareFnT>
    [[nodiscard]] constexpr bool    Compare (const value_type& aValue) const
    {
        CompareFnT cmpFn;
        return cmpFn(iValue, SConvertByRatio<SCALE_RATIO>(aValue));
    }

    template<typename ApplyFnT>
    constexpr void  Apply (void)
    {
        ApplyFnT applyFn;

        iValue = applyFn(iValue);
    }

    template<typename SCALE_RATIO,
             typename ApplyFnT>
    constexpr void  Apply (const value_type& aValue)
    {
        ApplyFnT applyFn;

        iValue = applyFn(iValue, SConvertByRatio<SCALE_RATIO>(aValue));
    }

private:
    value_type  iValue;
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
