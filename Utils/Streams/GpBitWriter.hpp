#pragma once

#include "GpBitWriterStorage.hpp"

namespace GPlatform {

class GPCORE_API GpBitWriter
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBitWriter)

public:
                            GpBitWriter     (GpBitWriterStorage& aStorage) noexcept: iStorage(aStorage) {}
                            ~GpBitWriter    (void) noexcept = default;

    void                    Bits            (GpRawPtrByteR      aData,
                                             const size_bit_t   aSize);

    void                    UInt8           (const u_int_8 aValue);
    void                    UInt8           (const u_int_8 aValue, const size_bit_t aSize);
    void                    SInt8           (const s_int_8 aValue);
    void                    SInt8           (const s_int_8 aValue, const size_bit_t aSize);
    void                    UInt16          (const u_int_16 aValue);
    void                    UInt16          (const u_int_16 aValue, const size_bit_t aSize);
    void                    SInt16          (const s_int_16 aValue);
    void                    SInt16          (const s_int_16 aValue, const size_bit_t aSize);
    void                    UInt32          (const u_int_32 aValue);
    void                    UInt32          (const u_int_32 aValue, const size_bit_t aSize);
    void                    SInt32          (const s_int_32 aValue);
    void                    SInt32          (const s_int_32 aValue, const size_bit_t aSize);
    void                    UInt64          (const u_int_64 aValue);
    void                    UInt64          (const u_int_64 aValue, const size_bit_t aSize);
    void                    SInt64          (const s_int_64 aValue);
    void                    SInt64          (const s_int_64 aValue, const size_bit_t aSize);
    void                    SizeT           (const size_t aValue, const size_bit_t aSize);

    size_bit_t              LeftToWite      (void) const noexcept {return iStorage.Left();}

private:
    void                    _Bits           (const std::byte*   aData,
                                             const size_bit_t   aSize);

    template<typename T>
    void                    WritePOD        (const T aValue, const size_bit_t aSize)
    {
        THROW_GPE_COND
        (
            aSize <= size_byte_t::SMake(sizeof(T)),
            "Out of range"_sv
        );

        T val = aValue;
#if defined(GP_ORDER_BIG_ENDIAN)
        val = BitOps::BSwap(val);
#endif
        _Bits(reinterpret_cast<const std::byte*>(&val), aSize);
    }

private:
    GpBitWriterStorage&     iStorage;
};

}//GPlatform
