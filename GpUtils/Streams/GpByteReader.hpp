#pragma once

#include "GpByteReaderStorage.hpp"
#include "../Types/Bits/GpBitOps.hpp"

namespace GPlatform {

class GP_UTILS_API GpByteReader
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteReader)

public:
    inline                  GpByteReader    (GpByteReaderStorage& aStorage) noexcept;
                            ~GpByteReader   (void) noexcept = default;

    inline size_t           SizeLeft        (void) const noexcept;
    inline GpSpanPtrByteR   Offset          (const size_t aOffset);//Same as Bytes(aOffset)
    inline GpSpanPtrByteR   StoragePtr      (void) noexcept;

    u_int_8                 UInt8           (void);
    s_int_8                 SInt8           (void);
    u_int_16                UInt16          (void);
    s_int_16                SInt16          (void);
    u_int_32                UInt32          (void);
    s_int_32                SInt32          (void);
    u_int_64                UInt64          (void);
    s_int_64                SInt64          (void);
    s_int_32                CompactSInt32   (void);
    GpSpanPtrByteR          BytesWithLen    (void);
    inline GpSpanPtrByteR   Bytes           (const size_t aSize);
    //inline GpSpanPtrByteR TryBytes        (const size_t aSize);


private:
    template<typename T>
    T                       ReadPOD         (void)
    {
        GpSpanPtrByteR data = Bytes(sizeof(T));

        T val;
        std::memcpy(&val, data.Ptr(), data.Count());
        val = BitOps::H2N(val);
        return val;
    }

private:
    GpByteReaderStorage&    iStorage;
};

GpByteReader::GpByteReader (GpByteReaderStorage& aStorage) noexcept:
iStorage(aStorage)
{
}

size_t  GpByteReader::SizeLeft (void) const noexcept
{
    return iStorage.SizeLeft();
}

GpSpanPtrByteR  GpByteReader::Offset (const size_t aOffset)
{
    return Bytes(aOffset);
}

GpSpanPtrByteR  GpByteReader::StoragePtr (void) noexcept
{
    return iStorage.StoragePtr();
}

GpSpanPtrByteR  GpByteReader::Bytes (const size_t aSize)
{
    return iStorage.Read(aSize);
}

/*GpSpanPtrByteR    GpByteReader::TryBytes (const size_t aSize)
{
    return iStorage.TryReadAndShift(aSize);
}*/

}//GPlatform
