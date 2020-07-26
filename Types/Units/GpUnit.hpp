#pragma once

#include "../Strings/GpStringTemplateArg.hpp"
#include "../Numerics/GpNumericOps.hpp"
#include "../Bits/GpBitCast.hpp"
#include <chrono>
#include <type_traits>

namespace GPlatform {

template<typename T,
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

    using unit_type     = UNIT_TYPE;
    using unit_name     = UNIT_NAME;
    using value_type    = T;
    using scale_ratio   = SCALE;
    using this_type     = GpUnit<T, UNIT_TYPE, SCALE, UNIT_NAME>;

    template<typename T_2, typename UNIT_TYPE_2>
    using Convertible   = std::enable_if_t<   std::is_same_v<T, T_2>
                                           && std::is_same_v<UNIT_TYPE, UNIT_TYPE_2>, int>;

    CLASS_TAG(GpUnit)
    CLASS_TAG_DETECTOR(GpUnit)

private:
    constexpr           GpUnit      (const T aValue) noexcept:
                                    iValue(aValue)
                                    {}
public:
    constexpr           GpUnit      (void) noexcept:
                                    iValue(T())
                                    {}

    constexpr           GpUnit      (const this_type& aValue) noexcept:
                                    iValue(aValue.iValue)
                                    {}

    constexpr           GpUnit      (this_type&& aValue) noexcept:
                                    iValue(std::move(aValue.iValue))
                                    {}

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr           GpUnit      (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2>& aValue):
                        iValue(SConvertFrom(aValue))
                        {}

    [[nodiscard]] static std::string_view   SName (void) noexcept
    {
        return GpStringTemplateArgHolder<UNIT_NAME>::SAsStringView();
    }

    [[nodiscard]] const std::string_view    Name (void) const noexcept
    {
        return SName();
    }

    [[nodiscard]] constexpr T   Value (void) const noexcept
    {
        return iValue;
    }

    template<typename AsT>
    [[nodiscard]] constexpr AsT ValueAs (void) const noexcept
    {
        if constexpr (SHasTag_GpUnit<AsT>)
        {
            return AsT::SMake(Value());
        } else
        {
            return NumOps::SConvert<AsT, value_type>(iValue);
        }
    }

    [[nodiscard]] constexpr this_type Max (void) const noexcept
    {
        return SMax();
    }

    [[nodiscard]] static constexpr this_type SMax (void) noexcept
    {
        return this_type(NumOps::SMax<T>());
    }

    [[nodiscard]] constexpr this_type Min (void) const noexcept
    {
        return SMin();
    }

    [[nodiscard]] static constexpr this_type SMin (void) noexcept
    {
        return this_type(NumOps::SMin<T>());
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr void Set (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue)
    {
        iValue = SConvertFrom(aValue);
    }

    constexpr void Set (const this_type aValue) noexcept
    {
        iValue = aValue.Value();
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsEqual (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const
    {
        return NumOps::SIsEqual<T>(iValue, SConvertFrom(aValue));
    }

    [[nodiscard]] constexpr bool    IsEqual (const this_type aValue) const noexcept
    {
        return NumOps::SIsEqual<T>(iValue, aValue.Value());
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsNotEqual (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const
    {
        return NumOps::SIsNotEqual<T>(iValue, SConvertFrom(aValue));
    }

    [[nodiscard]] constexpr bool    IsNotEqual (const this_type aValue) const noexcept
    {
        return NumOps::SIsNotEqual<T>(iValue, aValue.Value());
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsLess (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const
    {
        return NumOps::SIsLess<T>(iValue, SConvertFrom(aValue));
    }

    [[nodiscard]] constexpr bool    IsLess (const this_type aValue) const noexcept
    {
        return NumOps::SIsLess<T>(iValue, aValue.Value());
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsLessOrEqual(const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const
    {
        return NumOps::SIsLessOrEqual(iValue, SConvertFrom(aValue));
    }

    [[nodiscard]] constexpr bool    IsLessOrEqual (const this_type aValue) const noexcept
    {
        return NumOps::SIsLessOrEqual(iValue, aValue.Value());
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsGrater (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const
    {
        return NumOps::SIsGreater(iValue, SConvertFrom(aValue));
    }

    [[nodiscard]] constexpr bool    IsGrater (const this_type aValue) const noexcept
    {
        return NumOps::SIsGreater(iValue, aValue.Value());
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    IsGraterOrEqual (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const
    {
        return NumOps::SIsGreaterOrEqual(iValue, SConvertFrom(aValue));
    }

    [[nodiscard]] constexpr bool    IsGraterOrEqual (const this_type aValue) const noexcept
    {
        return NumOps::SIsGreaterOrEqual(iValue, aValue.Value());
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator== (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const noexcept
    {
        return IsEqual(aValue);
    }

    [[nodiscard]] constexpr bool    operator== (const this_type aValue) const noexcept
    {
        return IsEqual(aValue);
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator!= (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const noexcept
    {
        return IsNotEqual(aValue);
    }

    [[nodiscard]] constexpr bool    operator!= (const this_type aValue) const noexcept
    {
        return IsNotEqual(aValue);
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator> (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const noexcept
    {
        return IsGrater(aValue);
    }

    [[nodiscard]] constexpr bool    operator> (const this_type aValue) const noexcept
    {
        return IsGrater(aValue);
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator>= (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const noexcept
    {
        return IsGraterOrEqual(aValue);
    }

    [[nodiscard]] constexpr bool    operator>= (const this_type aValue) const noexcept
    {
        return IsGraterOrEqual(aValue);
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator< (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const noexcept
    {
        return IsLess(aValue);
    }

    [[nodiscard]] constexpr bool    operator< (const this_type aValue) const noexcept
    {
        return IsLess(aValue);
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    [[nodiscard]] constexpr bool    operator<= (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue) const noexcept
    {
        return IsLessOrEqual(aValue);
    }

    [[nodiscard]] constexpr bool    operator<= (const this_type aValue) const noexcept
    {
        return IsLessOrEqual(aValue);
    }

    constexpr this_type&    operator++ (void)
    {       
        iValue = NumOps::SAdd<T>(iValue, T(1));
        return *this;
    }

    constexpr this_type operator++ (int)
    {
        const T oldValue = iValue;
        iValue = NumOps::SAdd<T>(iValue, T(1));
        return this_type(oldValue);
    }

    constexpr this_type&    operator-- (void)
    {
        iValue = NumOps::SSub<T>(iValue, T(1));
        return *this;
    }

    constexpr this_type operator-- (int)
    {
        const T oldValue = iValue;
        iValue = NumOps::SSub<T>(iValue, T(1));
        return this_type(oldValue);
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator=   (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue)
    {
        iValue = SConvertFrom(aValue);
        return *this;
    }

    constexpr this_type&    operator=   (const this_type aValue) noexcept
    {
        iValue = aValue.Value();
        return *this;
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator+=  (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue)
    {
        iValue = NumOps::SAdd<T>(iValue, SConvertFrom(aValue));
        return *this;
    }

    constexpr this_type&    operator+= (const this_type aValue)
    {
        iValue = NumOps::SAdd<T>(iValue, aValue.Value());
        return *this;
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator-= (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue)
    {
        iValue = NumOps::SSub<T>(iValue, SConvertFrom(aValue));
        return *this;
    }

    constexpr this_type&    operator-= (const this_type aValue)
    {
        iValue = NumOps::SSub<T>(iValue, aValue.Value());
        return *this;
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator*= (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue)
    {
        iValue = NumOps::SMul<T>(iValue, SConvertFrom(aValue));
        return *this;
    }

    constexpr this_type&    operator*=  (const this_type aValue)
    {
        iValue = NumOps::SMul<T>(iValue, aValue.Value());
        return *this;
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator/=  (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue)
    {
        iValue = NumOps::SDiv<T>(iValue, SConvertFrom(aValue));
        return *this;
    }

    constexpr this_type&    operator/=  (const this_type aValue)
    {
        iValue = NumOps::SDiv<T>(iValue, aValue.Value());
        return *this;
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr this_type&    operator%= (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue)
    {
        iValue = NumOps::SMod<T>(iValue, SConvertFrom(aValue));
        return *this;
    }

    constexpr this_type&    operator%= (const this_type aValue)
    {
        iValue = NumOps::SMod<T>(iValue, aValue.Value());
        return *this;
    }

    constexpr const this_type operator- (void) const noexcept
    {
        return this_type(NumOps::SNegative<T>(Value()));
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr friend const this_type operator+ (const this_type aLeft, const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aRight)
    {
        return this_type(NumOps::SAdd<T>(aLeft.Value(), SConvertFrom(aRight)));
    }

    constexpr friend const this_type operator+(const this_type aLeft, const this_type aRight) noexcept
    {
        return this_type(NumOps::SAdd<T>(aLeft.Value(), aRight.Value()));
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr friend const this_type operator-(const this_type aLeft, const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aRight)
    {
        return this_type(NumOps::SSub<T>(aLeft.Value(), SConvertFrom(aRight)));
    }

    constexpr friend const this_type operator-(const this_type aLeft, const this_type aRight) noexcept
    {
        return this_type(NumOps::SSub<T>(aLeft.Value(), aRight.Value()));
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr friend const this_type operator*(const this_type aLeft, const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aRight)
    {
        return this_type(NumOps::SMul<T>(aLeft.Value(), SConvertFrom(aRight)));
    }

    constexpr friend const this_type operator*(const this_type aLeft, const this_type aRight) noexcept
    {
        return this_type(NumOps::SMul<T>(aLeft.Value(), aRight.Value()));
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr friend const this_type operator/(const this_type aLeft, const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aRight)
    {
        return this_type(NumOps::SDiv<T>(aLeft.Value(), SConvertFrom(aRight)));
    }

    constexpr friend const this_type operator/(const this_type aLeft, const this_type aRight) noexcept
    {
        return this_type(NumOps::SDiv<T>(aLeft.Value(), aRight.Value()));
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr friend const this_type operator%(const this_type aLeft, const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aRight)
    {
        return this_type(NumOps::SMod<T>(aLeft.Value(), SConvertFrom(aRight)));
    }

    constexpr friend const this_type operator%(const this_type aLeft, const this_type aRight) noexcept
    {
        return this_type(NumOps::SMod<T>(aLeft.Value(), aRight.Value()));
    }

    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2,
             Convertible<T_2, UNIT_TYPE_2> = 0>
    constexpr static T  SConvertFrom (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue)
    {
        using scale_ratio_2 = SCALE_2;
        using s             = std::ratio_divide<scale_ratio_2, scale_ratio>;

        if constexpr (std::is_floating_point<T>())
        {
            const T n = NumOps::SMul<T>(aValue.Value(), T(s::num));
            const T d = NumOps::SDiv<T>(n, T(s::den));

            return d;
        } else if constexpr (std::is_integral<T>())
        {
            using ratio_t = std::remove_cv_t<decltype(s::num)>;

            const ratio_t v = NumOps::SConvert<ratio_t>(aValue.Value());
            const ratio_t n = NumOps::SMul<ratio_t>(v, s::num);
            const ratio_t d = NumOps::SDiv<ratio_t>(n, s::den);

            return NumOps::SConvert<T>(d);
        }
    }

    template<typename T_2>
    constexpr static this_type  SMake (const T_2 aValue)
    {
        return this_type(NumOps::SConvert<T>(aValue));
    }

    template<typename T_2>
    constexpr static this_type  SBitCast (const T_2 aValue)
    {
        return this_type(std::bit_cast<value_type>(aValue));
    }


    template<typename T_2,
             typename UNIT_TYPE_2,
             typename SCALE_2,
             typename UNIT_NAME_2>
    constexpr static this_type  SBitCast (const GpUnit<T_2, UNIT_TYPE_2, SCALE_2, UNIT_NAME_2> aValue)
    {
        return this_type(std::bit_cast<value_type>(aValue.Value()));
    }

private:
    T   iValue;
};

}//GPlatform

namespace std {

template<typename T,
         typename UNIT_TYPE,
         typename SCALE,
         typename UNIT_NAME> struct hash<::GPlatform::GpUnit<T, UNIT_TYPE, SCALE, UNIT_NAME>>
{
    using argument_type = ::GPlatform::GpUnit<T, UNIT_TYPE, SCALE, UNIT_NAME>;
    using result_type   = size_t;

    result_type operator()(argument_type const& aArg) const noexcept
    {
        return std::hash<typename argument_type::value_type>(aArg.Value());
    }
};

}//std
