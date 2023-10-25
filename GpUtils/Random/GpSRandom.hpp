#pragma once

#include "../../Config/GpConfig.hpp"

#if defined(GP_USE_RANDOM_GENERATORS)

#include "../SyncPrimitives/GpSpinlock.hpp"
#include "GpRandom.hpp"
#include <mutex>

namespace GPlatform {

class GP_UTILS_API GpSRandom
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSRandom)

private:
                                        GpSRandom       (void) noexcept = default;
                                        ~GpSRandom      (void) noexcept = default;

public:
    static GpSRandom&                   S               (void) noexcept {return sInstance;}

    GpSpinlock&                         Lock            (void) noexcept {return iLock;}

    inline void                         SetSeedFromRD   (void);
    inline void                         SetSeed         (const random_mt19937::result_type aSeed);

    [[nodiscard]] inline s_int_8        SI8             (const s_int_8  aMin = NumOps::SMin<s_int_8>(),
                                                         const s_int_8  aMax = NumOps::SMax<s_int_8>()) noexcept;
    [[nodiscard]] inline u_int_8        UI8             (const u_int_8  aMin = NumOps::SMin<u_int_8>(),
                                                         const u_int_8  aMax = NumOps::SMax<u_int_8>()) noexcept;
    [[nodiscard]] inline s_int_16       SI16            (const s_int_16 aMin = NumOps::SMin<s_int_16>(),
                                                         const s_int_16 aMax = NumOps::SMax<s_int_16>()) noexcept;
    [[nodiscard]] inline u_int_16       UI16            (const u_int_16 aMin = NumOps::SMin<u_int_16>(),
                                                         const u_int_16 aMax = NumOps::SMax<u_int_16>()) noexcept;
    [[nodiscard]] inline s_int_32       SI32            (const s_int_32 aMin = NumOps::SMin<s_int_32>(),
                                                         const s_int_32 aMax = NumOps::SMax<s_int_32>()) noexcept;
    [[nodiscard]] inline u_int_32       UI32            (const u_int_32 aMin = NumOps::SMin<u_int_32>(),
                                                         const u_int_32 aMax = NumOps::SMax<u_int_32>()) noexcept;
    [[nodiscard]] inline s_int_64       SI64            (const s_int_64 aMin = NumOps::SMin<s_int_64>(),
                                                         const s_int_64 aMax = NumOps::SMax<s_int_64>()) noexcept;
    [[nodiscard]] inline u_int_64       UI64            (const u_int_64 aMin = NumOps::SMin<u_int_64>(),
                                                         const u_int_64 aMax = NumOps::SMax<u_int_64>()) noexcept;
    [[nodiscard]] inline ssize_t        SSizeT          (const ssize_t  aMin = NumOps::SMin<ssize_t>(),
                                                         const ssize_t  aMax = NumOps::SMax<ssize_t>()) noexcept;
    [[nodiscard]] inline size_t         SizeT           (const size_t   aMin = NumOps::SMin<size_t>(),
                                                         const size_t   aMax = NumOps::SMax<size_t>()) noexcept;
    [[nodiscard]] inline bool           Bool            (void) noexcept;
    [[nodiscard]] inline double         Double          (const double   aMin,
                                                         const double   aMax) noexcept;
    [[nodiscard]] inline float          Float           (const float    aMin,
                                                         const float    aMax) noexcept;

    [[nodiscard]] inline std::u8string  String          (const GpRandomStrMode::EnumT   aMode,
                                                         const size_t                   aSize);

    template<typename T>
    typename T::EnumT                   Enum            (void) noexcept;

    template<typename T>
    T                                   Numeric         (const T aMin = NumOps::SMin<T>(),
                                                         const T aMax = NumOps::SMax<T>()) noexcept;

private:
    GpRandom                            iRandom;
    mutable GpSpinlock                  iLock;

    static GpSRandom                    sInstance;
};

void    GpSRandom::SetSeedFromRD (void)
{
    std::scoped_lock l(iLock);
    iRandom.SetSeedFromRD();
}

void    GpSRandom::SetSeed (const random_mt19937::result_type aSeed)
{
    std::scoped_lock l(iLock);
    iRandom.SetSeed(aSeed);
}

s_int_8 GpSRandom::SI8 (const s_int_8 aMin, const s_int_8 aMax) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.SI8(aMin, aMax);
}

u_int_8 GpSRandom::UI8 (const u_int_8 aMin, const u_int_8 aMax) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.UI8(aMin, aMax);
}

s_int_16    GpSRandom::SI16 (const s_int_16 aMin, const s_int_16 aMax) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.SI16(aMin, aMax);
}

u_int_16    GpSRandom::UI16 (const u_int_16 aMin, const u_int_16 aMax) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.UI16(aMin, aMax);
}

s_int_32    GpSRandom::SI32 (const s_int_32 aMin, const s_int_32 aMax) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.SI32(aMin, aMax);
}

u_int_32    GpSRandom::UI32 (const u_int_32 aMin, const u_int_32 aMax) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.UI32(aMin, aMax);
}

s_int_64    GpSRandom::SI64 (const s_int_64 aMin, const s_int_64 aMax) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.SI64(aMin, aMax);
}

u_int_64    GpSRandom::UI64 (const u_int_64 aMin, const u_int_64 aMax) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.UI64(aMin, aMax);
}

ssize_t GpSRandom::SSizeT (const ssize_t aMin, const ssize_t aMax) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.SSizeT(aMin, aMax);
}

size_t  GpSRandom::SizeT (const size_t aMin, const size_t aMax) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.SizeT(aMin, aMax);
}

bool    GpSRandom::Bool (void) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.Bool();
}

double  GpSRandom::Double
(
    const double    aMin,
    const double    aMax
) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.Double(aMin, aMax);
}

float   GpSRandom::Float
(
    const float aMin,
    const float aMax
) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.Float(aMin, aMax);
}

std::u8string   GpSRandom::String
(
    const GpRandomStrMode::EnumT    aMode,
    const size_t                    aSize
)
{
    std::scoped_lock l(iLock);
    return iRandom.String(aMode, aSize);
}

template<typename T>
typename T::EnumT   GpSRandom::Enum (void) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.Enum<T>();
}

template<typename T>
T   GpSRandom::Numeric
(
    const T aMin,
    const T aMax
) noexcept
{
    std::scoped_lock l(iLock);
    return iRandom.Numeric<T>(aMin, aMax);
}

}//GPlatform

#endif//#if defined(GP_USE_RANDOM_GENERATORS)
