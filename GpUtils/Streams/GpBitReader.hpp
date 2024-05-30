#pragma once

/*#include "GpBitReaderStorage.hpp"
#include "../Types/Bits/GpBitOps.hpp"

namespace GPlatform {

class GP_UTILS_API GpBitReader
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBitReader)

public:
                            GpBitReader     (GpBitReaderStorage&& aStorage) noexcept: iStorage(std::move(aStorage)){}
                            ~GpBitReader    (void) noexcept = default;

    void                    Bits            (GpSpanByteRW       aDataOut,
                                             const size_bit_t   aSize,
                                             const size_bit_t   aOffset);

    u_int_8                 UI8             (void);
    u_int_8                 UI8             (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    s_int_8                 SI8             (void);
    s_int_8                 SI8             (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    u_int_16                UI16            (void);
    u_int_16                UI16            (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    s_int_16                SI16            (void);
    s_int_16                SI16            (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    u_int_32                UI32            (void);
    u_int_32                UI32            (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    s_int_32                SI32            (void);
    s_int_32                SI32            (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    u_int_64                UI64            (void);
    u_int_64                UI64            (const size_bit_t aSize,
                                             const size_bit_t aOffset);
    s_int_64                SI64            (void);
    s_int_64                SI64            (const size_bit_t aSize,
                                             const size_bit_t aOffset);

    size_bit_t              LeftToRead      (void) const noexcept {return iStorage.Left();}

private:
    void                    _Bits           (u_int_8*           aDataOut,
                                             const size_bit_t   aSize,
                                             const size_bit_t   aOffset);

    template<typename T>
    T                       ReadPOD         (const size_bit_t aSize,
                                             const size_bit_t aOffset)
    {
        THROW_COND_GP
        (
            aSize <= size_byte_t::SMake(sizeof(T)),
            "Out of range"_sv
        );

        T val = T();
        _Bits(reinterpret_cast<u_int_8*>(&val), aSize, aOffset);
        val = T(BitOps::N2H(val) >> (sizeof(T)*8 - aSize.As<size_t>()));

        return val;
    }

    void                    Copy            (u_int_8*           aDst,
                                             const size_bit_t   aDstOffset,
                                             const u_int_8*     aSrc,
                                             const size_bit_t   aSrcOffset,
                                             const size_bit_t   aSize);

private:
    GpBitReaderStorage      iStorage;
};

}// namespace GPlatform
*/
