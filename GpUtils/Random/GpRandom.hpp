#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

#include <random>

#include "../GpUtils_global.hpp"
#include "../Macro/GpMacroClass.hpp"
#include "../Types/Numerics/GpNumericOps.hpp"
#include "GpRandomStrMode.hpp"

namespace GPlatform {

#if defined(GP_ENVIRONMENT_64)
    using random_mt19937 = std::mt19937_64;
#elif defined(GP_ENVIRONMENT_32)
    using random_mt19937 = std::mt19937;
#else
#   error Unknown environment
#endif

class GP_UTILS_API GpRandom
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpRandom)

public:
                                GpRandom        (void) noexcept = default;
                                ~GpRandom       (void) noexcept = default;

    void                        SetSeedFromRD   (void);
    void                        SetSeed         (const random_mt19937::result_type aSeed);

    [[nodiscard]] s_int_8       SI8             (const s_int_8  aMin = NumOps::SMin<s_int_8>(),
                                                 const s_int_8  aMax = NumOps::SMax<s_int_8>()) noexcept;
    [[nodiscard]] u_int_8       UI8             (const u_int_8  aMin = NumOps::SMin<u_int_8>(),
                                                 const u_int_8  aMax = NumOps::SMax<u_int_8>()) noexcept;
    [[nodiscard]] s_int_16      SI16            (const s_int_16 aMin = NumOps::SMin<s_int_16>(),
                                                 const s_int_16 aMax = NumOps::SMax<s_int_16>()) noexcept;
    [[nodiscard]] u_int_16      UI16            (const u_int_16 aMin = NumOps::SMin<u_int_16>(),
                                                 const u_int_16 aMax = NumOps::SMax<u_int_16>()) noexcept;
    [[nodiscard]] s_int_32      SI32            (const s_int_32 aMin = NumOps::SMin<s_int_32>(),
                                                 const s_int_32 aMax = NumOps::SMax<s_int_32>()) noexcept;
    [[nodiscard]] u_int_32      UI32            (const u_int_32 aMin = NumOps::SMin<u_int_32>(),
                                                 const u_int_32 aMax = NumOps::SMax<u_int_32>()) noexcept;
    [[nodiscard]] s_int_64      SI64            (const s_int_64 aMin = NumOps::SMin<s_int_64>(),
                                                 const s_int_64 aMax = NumOps::SMax<s_int_64>()) noexcept;
    [[nodiscard]] u_int_64      UI64            (const u_int_64 aMin = NumOps::SMin<u_int_64>(),
                                                 const u_int_64 aMax = NumOps::SMax<u_int_64>()) noexcept;
    [[nodiscard]] ssize_t       SSizeT          (const ssize_t  aMin = NumOps::SMin<ssize_t>(),
                                                 const ssize_t  aMax = NumOps::SMax<ssize_t>()) noexcept;
    [[nodiscard]] size_t        SizeT           (const size_t   aMin = NumOps::SMin<size_t>(),
                                                 const size_t   aMax = NumOps::SMax<size_t>()) noexcept;
    [[nodiscard]] bool          Bool            (void) noexcept;
    [[nodiscard]] double        Double          (const double   aMin,
                                                 const double   aMax) noexcept;
    [[nodiscard]] float         Float           (const float    aMin,
                                                 const float    aMax) noexcept;

    [[nodiscard]] std::u8string String          (const GpRandomStrMode::EnumT   aMode,
                                                 const size_t                   aSize);

    template<EnumConcepts::IsEnum T>
    typename T::EnumT           Enum            (void) noexcept;

    template<typename T>
    T                           Numeric         (const T aMin = NumOps::SMin<T>(),
                                                 const T aMax = NumOps::SMax<T>()) noexcept;

private:
    template<typename T,
             template<typename> typename DistributionT = std::uniform_int_distribution>
    [[nodiscard]] T             Next            (const T aMin,
                                                 const T aMax) noexcept;

private:
    random_mt19937                              iEngine;
    static std::array<const std::u8string, 2>   sStrs;
};

template<EnumConcepts::IsEnum T>
typename T::EnumT   GpRandom::Enum (void) noexcept
{
    const u_int_64 pos = UI64
    (
        u_int_64(0),
        NumOps::SSub<u_int_64>(u_int_64(T::SCount()), 1)
    );

    return T().FromNumPos(NumOps::SConvert<size_t>(pos));
}

template<typename T>
T   GpRandom::Numeric
(
    const T aMin,
    const T aMax
) noexcept
{
    static_assert(std::is_integral_v<T>);
    static_assert(sizeof(T) <= sizeof(u_int_64));

    if constexpr (std::is_signed_v<T>)
    {
        return T(SI64(s_int_64(aMin), s_int_64(aMax)));
    } else
    {
        return T(UI64(u_int_64(aMin), u_int_64(aMax)));
    }
}

template<typename T,
         template<typename> typename DistributionT>
T   GpRandom::Next
(
    const T aMin,
    const T aMax
) noexcept
{
    DistributionT<T> dist(aMin, aMax);
    return T(dist(iEngine));
}

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
