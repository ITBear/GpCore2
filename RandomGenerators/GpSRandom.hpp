#pragma once

#include "GpRandom.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

namespace GPlatform {

class GPCORE_API GpSRandom
{
    CLASS_REMOVE_CTRS_EXCEPT_DEFAULT(GpSRandom)

private:
                                GpSRandom       (void) noexcept;
                                ~GpSRandom      (void) noexcept;

public:
    static GpSRandom&           S               (void) noexcept;

    GpSpinlock&                 Lock            (void) noexcept {return iLock;}

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
    GpRandom                    iRandom;
    mutable GpSpinlock          iLock;
};

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
