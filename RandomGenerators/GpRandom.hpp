#pragma once

#include "../GpCore_global.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

#include "../Multithreading/SyncPrimitives/GpSyncPrimitives.hpp"
#include "../Types/Numerics/GpNumericOps.hpp"
#include "../Types/Units/Numerics/GpUnitsNumerics.hpp"
#include <random>
#include <array>
#include <type_traits>

namespace GPlatform {

#if defined(GP_ENVIRONMENT_64)
    using random_mt19937 = std::mt19937_64;
#elif defined(GP_ENVIRONMENT_32)
    using random_mt19937 = std::mt19937;
#else
#   error Unknown environment
#endif

class GPCORE_API GpRandom
{
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpRandom)

public:
                                GpRandom        (void) noexcept;
                                ~GpRandom       (void) noexcept;

    void                        SetSeedFromRD   (void);
    void                        SetSeed         (const random_mt19937::result_type aSeed);

    [[nodiscard]] SInt8         SI8             (const SInt8    aMin = SInt8::SMake(NumOps::SMin<SInt8::value_type>()),
                                                 const SInt8    aMax = SInt8::SMake(NumOps::SMax<SInt8::value_type>()));
    [[nodiscard]] UInt8         UI8             (const UInt8    aMin = UInt8::SMake(NumOps::SMin<UInt8::value_type>()),
                                                 const UInt8    aMax = UInt8::SMake(NumOps::SMax<UInt8::value_type>()));
    [[nodiscard]] SInt16        SI16            (const SInt16   aMin = SInt16::SMake(NumOps::SMin<SInt16::value_type>()),
                                                 const SInt16   aMax = SInt16::SMake(NumOps::SMax<SInt16::value_type>()));
    [[nodiscard]] UInt16        UI16            (const UInt16   aMin = UInt16::SMake(NumOps::SMin<UInt16::value_type>()),
                                                 const UInt16   aMax = UInt16::SMake(NumOps::SMax<UInt16::value_type>()));
    [[nodiscard]] SInt32        SI32            (const SInt32   aMin = SInt32::SMake(NumOps::SMin<SInt32::value_type>()),
                                                 const SInt32   aMax = SInt32::SMake(NumOps::SMax<SInt32::value_type>()));
    [[nodiscard]] UInt32        UI32            (const UInt32   aMin = UInt32::SMake(NumOps::SMin<UInt32::value_type>()),
                                                 const UInt32   aMax = UInt32::SMake(NumOps::SMax<UInt32::value_type>()));
    [[nodiscard]] SInt64        SI64            (const SInt64   aMin = SInt64::SMake(NumOps::SMin<SInt64::value_type>()),
                                                 const SInt64   aMax = SInt64::SMake(NumOps::SMax<SInt64::value_type>()));
    [[nodiscard]] UInt64        UI64            (const UInt64   aMin = UInt64::SMake(NumOps::SMin<UInt64::value_type>()),
                                                 const UInt64   aMax = UInt64::SMake(NumOps::SMax<UInt64::value_type>()));

private:
    template<typename T,
             template<typename> typename Distribution = std::uniform_int_distribution>
    [[nodiscard]] T             Next            (const T aMin, const T aMax)
    {
        Distribution<typename T::value_type> dist(aMin.Value(), aMax.Value());
        return T::SMake(dist(iEngine));
    }

private:
    random_mt19937      iEngine;
};

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
