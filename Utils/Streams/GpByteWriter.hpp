#pragma once

#include "GpByteWriterStorage.hpp"

namespace GPlatform {

class GPCORE_API GpByteWriter
{
    CLASS_REMOVE_CTRS(GpByteWriter)

public:
                            GpByteWriter    (GpByteWriterStorage& aStorage) noexcept: iStorage(aStorage) {}
                            ~GpByteWriter   (void) noexcept = default;

    GpByteWriter&           OffsetAdd       (const size_byte_t aOffset);
    GpRawPtrByteRW          DataOut         (void) noexcept {return iStorage.DataOut();}

    GpByteWriter&           UInt8           (const u_int_8 aValue);
    GpByteWriter&           SInt8           (const s_int_8 aValue);
    GpByteWriter&           UInt16          (const u_int_16 aValue);
    GpByteWriter&           SInt16          (const s_int_16 aValue);
    GpByteWriter&           UInt32          (const u_int_32 aValue);
    GpByteWriter&           SInt32          (const s_int_32 aValue);
    GpByteWriter&           UInt64          (const u_int_64 aValue);
    GpByteWriter&           SInt64          (const s_int_64 aValue);
    GpByteWriter&           BytesWithLen    (GpRawPtrByteR aData);
    GpByteWriter&           Bytes           (GpRawPtrByteR aData);
    GpByteWriter&           CompactUInt32   (const u_int_32 aValue);
    GpByteWriter&           CompactUInt16   (const u_int_16 aValue);

private:
    template<typename T>
    void                    WritePOD        (const T aValue)
    {
        const count_t   size    = count_t::SMake(sizeof(T));
        const T         val     = BitOps::H2N(aValue);

        Bytes({reinterpret_cast<const std::byte*>(&val), size});
    }

private:
    GpByteWriterStorage&    iStorage;
};

}//GPlatform
