#pragma once

#include "GpByteWriterStorage.hpp"
#include "../Types/Bits/GpBitOps.hpp"

namespace GPlatform {

class GP_UTILS_API GpByteWriter
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteWriter)

public:
    inline                  GpByteWriter    (GpByteWriterStorage& aStorage) noexcept;
                            ~GpByteWriter   (void) noexcept = default;

    /**
     * @brief Offset - make offset (allocate memory if need)
     * @param aOffset - size in bytes to `skip`
     * @return - SpanPtr to memory before offset
     */
    inline GpSpanPtrByteRW  Offset          (const size_t aOffset);
    inline GpSpanPtrByteRW  StoragePtr      (void) noexcept;
    inline size_t           TotalWrite      (void) const noexcept;
    inline void             ShrinkToFit     (void);
    inline void             ReserveNext     (const size_t aSize);

    GpByteWriter&           UInt8           (const u_int_8 aValue);
    GpByteWriter&           SInt8           (const s_int_8 aValue);
    GpByteWriter&           UInt16          (const u_int_16 aValue);
    GpByteWriter&           SInt16          (const s_int_16 aValue);
    GpByteWriter&           UInt32          (const u_int_32 aValue);
    GpByteWriter&           SInt32          (const s_int_32 aValue);
    GpByteWriter&           UInt64          (const u_int_64 aValue);
    GpByteWriter&           SInt64          (const s_int_64 aValue);
    GpByteWriter&           BytesWithLen    (GpSpanPtrByteR aData);
    inline GpByteWriter&    Bytes           (GpSpanPtrByteR aData);
    inline GpByteWriter&    Bytes           (std::string_view aData);
    inline GpByteWriter&    Bytes           (const std::string& aData);
    inline GpByteWriter&    Bytes           (const void*    aPtr,
                                             const size_t   aSize);
    GpByteWriter&           CompactUInt64   (const u_int_64 aValue);
    GpByteWriter&           CompactSInt64   (const s_int_64 aValue);

private:
    template<typename T>
    void                    WritePOD        (const T aValue)
    {
        const T val = BitOps::H2N(aValue);
        Bytes({reinterpret_cast<const std::byte*>(&val), sizeof(T)});
    }

private:
    GpByteWriterStorage&    iStorage;
};

GpByteWriter::GpByteWriter (GpByteWriterStorage& aStorage) noexcept:
iStorage(aStorage)
{
}

GpSpanPtrByteRW GpByteWriter::Offset (const size_t aOffset)
{
    if (aOffset == 0)
    {
        return StoragePtr();
    }

    return iStorage.Offset(aOffset);
}

GpByteWriter&   GpByteWriter::Bytes (GpSpanPtrByteR aData)
{
    iStorage.Write(aData);
    return *this;
}

GpByteWriter&   GpByteWriter::Bytes (std::string_view aData)
{
    return Bytes(GpSpanPtrByteR(aData.data(), aData.size()));
}

GpByteWriter&   GpByteWriter::Bytes (const std::string& aData)
{
    return Bytes(GpSpanPtrByteR(aData.data(), aData.size()));
}

GpByteWriter&   GpByteWriter::Bytes
(
    const void*     aPtr,
    const size_t    aSize
)
{
    iStorage.Write(aPtr, aSize);
    return *this;
}

GpSpanPtrByteRW GpByteWriter::StoragePtr (void) noexcept
{
    return iStorage.StoragePtr();
}

size_t  GpByteWriter::TotalWrite (void) const noexcept
{
    return iStorage.TotalWrite();
}

void    GpByteWriter::ShrinkToFit (void)
{
    iStorage.ShrinkToFit();
}

void    GpByteWriter::ReserveNext (const size_t aSize)
{
    iStorage.ReserveNext(aSize);
}

}//GPlatform
