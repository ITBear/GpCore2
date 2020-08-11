#pragma once

#include "GpBitReaderStorage.hpp"

namespace GPlatform {

class GPCORE_API GpBitReader
{
    CLASS_REMOVE_CTRS(GpBitReader)

public:
                            GpBitReader     (GpBitReaderStorage&& aStorage) noexcept: iStorage(std::move(aStorage)){}
                            ~GpBitReader    (void) noexcept = default;

    void                    Bits            (GpRawPtrByteRW     aDataOut,
                                             const size_bit_t   aSize,
                                             const size_bit_t   aOffset);

    u_int_8                 UInt8           (void);
    u_int_8                 UInt8           (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    s_int_8                 SInt8           (void);
    s_int_8                 SInt8           (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    u_int_16                UInt16          (void);
    u_int_16                UInt16          (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    s_int_16                SInt16          (void);
    s_int_16                SInt16          (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    u_int_32                UInt32          (void);
    u_int_32                UInt32          (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    s_int_32                SInt32          (void);
    s_int_32                SInt32          (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    u_int_64                UInt64          (void);
    u_int_64                UInt64          (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    s_int_64                SInt64          (void);
    s_int_64                SInt64          (const size_bit_t aSize,
                                             const size_bit_t aOffset);

    size_bit_t              LeftToRead      (void) const noexcept {return iStorage.Left();}

private:
    void                    _Bits           (std::byte*         aDataOut,
                                             const size_bit_t   aSize,
                                             const size_bit_t   aOffset);

    template<typename T>
    T                       ReadPOD         (const size_bit_t aSize,
                                             const size_bit_t aOffset)
    {
        THROW_GPE_COND_CHECK_M(aSize <= size_byte_t::SMake(sizeof(T)), "Out of range"_sv);

        T val = T();
        _Bits(reinterpret_cast<std::byte*>(&val), aSize, aOffset);
        //val = T(BitOps::N2H(val) >> (sizeof(T) - aSize.ValueAs<size_t>()));

        val = T(BitOps::N2H(val) >> (sizeof(T)*8 - aSize.ValueAs<size_t>()));

        return val;
    }

    void                    Copy            (std::byte*         aDst,
                                             const size_bit_t   aDstOffset,
                                             const std::byte*   aSrc,
                                             const size_bit_t   aSrcOffset,
                                             const size_bit_t   aSize);

private:
    GpBitReaderStorage      iStorage;
};

}//GPlatform
