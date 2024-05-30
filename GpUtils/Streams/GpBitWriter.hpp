#pragma once

/*
#include "GpBitWriterStorage.hpp"

namespace GPlatform {

class GP_UTILS_API GpBitWriter
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBitWriter)

public:
                            GpBitWriter     (GpBitWriterStorage& aStorage) noexcept: iStorage(aStorage) {}
                            ~GpBitWriter    (void) noexcept = default;

    void                    Bits            (GpSpanByteR        aData,
                                             const size_bit_t   aSize);

    void                    UI8             (const u_int_8 aValue);
    void                    UI8             (const u_int_8 aValue, const size_bit_t aSize);
    void                    SI8             (const s_int_8 aValue);
    void                    SI8             (const s_int_8 aValue, const size_bit_t aSize);
    void                    UI16            (const u_int_16 aValue);
    void                    UI16            (const u_int_16 aValue, const size_bit_t aSize);
    void                    SI16            (const s_int_16 aValue);
    void                    SI16            (const s_int_16 aValue, const size_bit_t aSize);
    void                    UI32            (const u_int_32 aValue);
    void                    UI32            (const u_int_32 aValue, const size_bit_t aSize);
    void                    SI32            (const s_int_32 aValue);
    void                    SI32            (const s_int_32 aValue, const size_bit_t aSize);
    void                    UI64            (const u_int_64 aValue);
    void                    UI64            (const u_int_64 aValue, const size_bit_t aSize);
    void                    SI64            (const s_int_64 aValue);
    void                    SI64            (const s_int_64 aValue, const size_bit_t aSize);
    void                    SizeT           (const size_t aValue, const size_bit_t aSize);

    size_bit_t              LeftToWite      (void) const noexcept {return iStorage.Left();}

private:
    void                    _Bits           (const u_int_8*     aData,
                                             const size_bit_t   aSize);

    template<typename T>
    void                    WritePOD        (const T aValue, const size_bit_t aSize)
    {
        THROW_COND_GP
        (
            aSize <= size_byte_t::SMake(sizeof(T)),
            "Out of range"_sv
        );

        T val = aValue;
#if defined(GP_ORDER_BIG_ENDIAN)
        val = BitOps::BSwap(val);
#endif
        _Bits(reinterpret_cast<const u_int_8*>(&val), aSize);
    }

private:
    GpBitWriterStorage&     iStorage;
};

}// namespace GPlatform
*/
