#pragma once

#include <GpCore2/Config/GpConfig.hpp>

#if defined(GP_USE_RANDOM_GENERATORS)

#include <GpCore2/GpUtils/SyncPrimitives/GpSpinLock.hpp>
#include <GpCore2/GpUtils/Random/GpRandom.hpp>

#include <mutex>

namespace GPlatform {

class GP_UTILS_API GpSRandom final: public GpRandomIf
{
    CLASS_REMOVE_CTRS_MOVE_COPY(GpSRandom)

private:
                            GpSRandom       (void) noexcept;
    virtual                 ~GpSRandom      (void) noexcept override final;

public:
    static GpSRandom&       S               (void) noexcept {return sInstance;}

    GpSpinLock&             GetLock         (void) noexcept RETURN_CAPABILITY(iSpinLock) {return iSpinLock;}

    virtual void            SetSeedFromRD   (void) override final;
    virtual void            SetSeed         (random_mt19937::result_type aSeed) override final;

    virtual s_int_8         SI8             (s_int_8    aMin = NumOps::SMin<s_int_8>(),
                                             s_int_8    aMax = NumOps::SMax<s_int_8>()) noexcept override final;
    virtual u_int_8         UI8             (u_int_8    aMin = NumOps::SMin<u_int_8>(),
                                             u_int_8    aMax = NumOps::SMax<u_int_8>()) noexcept override final;
    virtual s_int_16        SI16            (s_int_16   aMin = NumOps::SMin<s_int_16>(),
                                             s_int_16   aMax = NumOps::SMax<s_int_16>()) noexcept override final;
    virtual u_int_16        UI16            (u_int_16   aMin = NumOps::SMin<u_int_16>(),
                                             u_int_16   aMax = NumOps::SMax<u_int_16>()) noexcept override final;
    virtual s_int_32        SI32            (s_int_32   aMin = NumOps::SMin<s_int_32>(),
                                             s_int_32   aMax = NumOps::SMax<s_int_32>()) noexcept override final;
    virtual u_int_32        UI32            (u_int_32   aMin = NumOps::SMin<u_int_32>(),
                                             u_int_32   aMax = NumOps::SMax<u_int_32>()) noexcept override final;
    virtual s_int_64        SI64            (s_int_64   aMin = NumOps::SMin<s_int_64>(),
                                             s_int_64   aMax = NumOps::SMax<s_int_64>()) noexcept override final;
    virtual u_int_64        UI64            (u_int_64   aMin = NumOps::SMin<u_int_64>(),
                                             u_int_64   aMax = NumOps::SMax<u_int_64>()) noexcept override final;
    virtual ssize_t         SSizeT          (ssize_t    aMin = NumOps::SMin<ssize_t>(),
                                             ssize_t    aMax = NumOps::SMax<ssize_t>()) noexcept override final;
    virtual size_t          SizeT           (size_t     aMin = NumOps::SMin<size_t>(),
                                             size_t     aMax = NumOps::SMax<size_t>()) noexcept override final;
    virtual bool            Bool            (void) noexcept override final;
    virtual double          Double          (double aMin,
                                             double aMax) noexcept override final;
    virtual float           Float           (float  aMin,
                                             float  aMax) noexcept override final;
    virtual std::string     String          (GpRandomStrMode::EnumT aMode,
                                             size_t                 aSize) override final;
    virtual GpBytesArray    BytesArray      (size_t aSize) override final;
    virtual void            BytesArray      (GpSpanByteRW   aSpanByteRW) override final;
    virtual void            BytesArray      (GpByteWriter&  aDataWriter,
                                             size_t         aSize) override final;

    template<typename T>
    T&                      BytesArray      (T&     aBufferToWrite,
                                             size_t aSize);

private:
    mutable GpSpinLock  iSpinLock;
    GpRandom            iRandom GUARDED_BY(iSpinLock);

    static GpSRandom    sInstance;
};

template<typename T>
T&  GpSRandom::BytesArray
(
    T&              aBufferToWrite,
    const size_t    aSize
)
{
    return GpRandomIf::BytesArray<T>(aBufferToWrite, aSize);
}

}// namespace GPlatform

#endif// #if defined(GP_USE_RANDOM_GENERATORS)
