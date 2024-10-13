#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_RANDOM_GENERATORS)

#include <random>

#include <GpCore2/GpUtils/GpUtils_global.hpp>
#include <GpCore2/GpUtils/Macro/GpMacroClass.hpp>
#include <GpCore2/GpUtils/Types/Numerics/GpNumericOps.hpp>
#include <GpCore2/GpUtils/Types/Enums/GpEnum.hpp>
#include <GpCore2/GpUtils/Types/Strings/GpStringOps.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriter.hpp>
#include <GpCore2/GpUtils/Streams/GpByteWriterStorageFixedSize.hpp>

namespace GPlatform {

GP_ENUM(GP_UTILS_API, GpRandomStrMode,
    NUM,
    ALPHA_NUM,
    ALPHA_NUM_AND_SPECIAL,
    ALL_VISIBLE
);

#if defined(GP_ENVIRONMENT_64)
    using random_mt19937 = std::mt19937_64;
#elif defined(GP_ENVIRONMENT_32)
    using random_mt19937 = std::mt19937;
#else
#   error Unknown environment
#endif

class GP_UTILS_API GpRandomIf
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpRandomIf)

public:
                            GpRandomIf      (void) noexcept;
    virtual                 ~GpRandomIf     (void) noexcept;

    virtual void            SetSeedFromRD   (void) = 0;
    virtual void            SetSeed         (random_mt19937::result_type aSeed) = 0;

    virtual s_int_8         SI8             (s_int_8    aMin = NumOps::SMin<s_int_8>(),
                                             s_int_8    aMax = NumOps::SMax<s_int_8>()) noexcept = 0;
    virtual u_int_8         UI8             (u_int_8    aMin = NumOps::SMin<u_int_8>(),
                                             u_int_8    aMax = NumOps::SMax<u_int_8>()) noexcept = 0;
    virtual s_int_16        SI16            (s_int_16   aMin = NumOps::SMin<s_int_16>(),
                                             s_int_16   aMax = NumOps::SMax<s_int_16>()) noexcept = 0;
    virtual u_int_16        UI16            (u_int_16   aMin = NumOps::SMin<u_int_16>(),
                                             u_int_16   aMax = NumOps::SMax<u_int_16>()) noexcept = 0;
    virtual s_int_32        SI32            (s_int_32   aMin = NumOps::SMin<s_int_32>(),
                                             s_int_32   aMax = NumOps::SMax<s_int_32>()) noexcept = 0;
    virtual u_int_32        UI32            (u_int_32   aMin = NumOps::SMin<u_int_32>(),
                                             u_int_32   aMax = NumOps::SMax<u_int_32>()) noexcept = 0;
    virtual s_int_64        SI64            (s_int_64   aMin = NumOps::SMin<s_int_64>(),
                                             s_int_64   aMax = NumOps::SMax<s_int_64>()) noexcept = 0;
    virtual u_int_64        UI64            (u_int_64   aMin = NumOps::SMin<u_int_64>(),
                                             u_int_64   aMax = NumOps::SMax<u_int_64>()) noexcept = 0;
    virtual ssize_t         SSizeT          (ssize_t    aMin = NumOps::SMin<ssize_t>(),
                                             ssize_t    aMax = NumOps::SMax<ssize_t>()) noexcept = 0;
    virtual size_t          SizeT           (size_t     aMin = NumOps::SMin<size_t>(),
                                             size_t     Max = NumOps::SMax<size_t>()) noexcept = 0;
    virtual bool            Bool            (void) noexcept = 0;
    virtual double          Double          (double aMin,
                                             double aMax) noexcept = 0;
    virtual float           Float           (float  aMin,
                                             float  aMax) noexcept = 0;

    virtual std::string     String          (GpRandomStrMode::EnumT aMode,
                                             size_t                 aSize) = 0;
    virtual GpBytesArray    BytesArray      (size_t aSize) = 0;
    virtual void            BytesArray      (GpSpanByteRW   aSpanByteRW) = 0;
    virtual void            BytesArray      (GpByteWriter&  aDataWriter,
                                             size_t         aSize) = 0;

    template<typename T>
    T&                      BytesArray      (T&     aBufferToWrite,
                                             size_t aSize);

    template<EnumConcepts::IsEnum T>
    typename T::EnumT       Enum            (void) noexcept;

    template<typename T>
    T                       Numeric         (T aMin = NumOps::SMin<T>(),
                                             T aMax = NumOps::SMax<T>()) noexcept;

protected:
    static std::array<const std::string, 4> sStrs;
};


template<typename T>
T&  GpRandomIf::BytesArray
(
    T&              aBufferToWrite,
    const size_t    aSize
)
{
    aBufferToWrite.resize(aSize);

    GpByteWriterStorageFixedSize    storage{aBufferToWrite};
    GpByteWriter                    writer{storage};

    BytesArray(writer, aSize);

    return aBufferToWrite;
}

template<EnumConcepts::IsEnum T>
typename T::EnumT   GpRandomIf::Enum (void) noexcept
{
    const u_int_64 pos = UI64
    (
        u_int_64{0},
        NumOps::SSub<u_int_64>(u_int_64{T::SCount()}, 1)
    );

    return T().FromNumPos(NumOps::SConvert<size_t>(pos));
}

template<typename T>
T   GpRandomIf::Numeric
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

}// namespace GPlatform

#endif// #if defined(GP_USE_RANDOM_GENERATORS)
