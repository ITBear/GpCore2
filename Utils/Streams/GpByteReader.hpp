#pragma once

#include "GpByteReaderStorage.hpp"

namespace GPlatform {

class GPCORE_API GpByteReader
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteReader)

public:
                            GpByteReader    (GpByteReaderStorage& aStorage) noexcept: iStorage(aStorage){}
                            ~GpByteReader   (void) noexcept = default;

    u_int_8                 UInt8           (void);
    s_int_8                 SInt8           (void);
    u_int_16                UInt16          (void);
    s_int_16                SInt16          (void);
    u_int_32                UInt32          (void);
    s_int_32                SInt32          (void);
    u_int_64                UInt64          (void);
    s_int_64                SInt64          (void);
    GpRawPtrByteR           BytesWithLen    (void);
    GpRawPtrByteR           Bytes           (const size_byte_t aSize) {return iStorage.ReadAndShift(aSize);}
    GpRawPtrByteR           TryBytes        (const size_byte_t aSize) {return iStorage.TryReadAndShift(aSize);}

    s_int_32                CompactSInt32   (void);

    size_byte_t             SizeLeft        (void) const noexcept {return iStorage.SizeLeft();}

    void                    Rollback        (const size_byte_t aSize) {return iStorage.Rollback(aSize);}

private:
    template<typename T>
    T                       ReadPOD         (void)
    {
        GpRawPtrByteR data = Bytes(size_byte_t::SMake(sizeof(T)));

        T val;
        std::memcpy(&val, data.PtrBegin(), data.CountTotal().As<size_t>());
        val = BitOps::H2N(val);
        return val;
    }

private:
    GpByteReaderStorage&    iStorage;
};

}//GPlatform
