#pragma once

#include "GpByteWriterStorage.hpp"
#include "GpByteWriterRef.hpp"
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
    inline GpSpanByteRW     OffsetAdd       (const size_t aOffset);
    inline GpSpanByteRW     StoragePtr      (void) noexcept;
    inline size_t           SizeLeft        (void) const noexcept;
    inline size_t           TotalWrite      (void) const noexcept;
    inline void             OnEnd           (void);
    inline void             ReserveNext     (const size_t aSize);

    template<Concepts::IsIntegralUpTo64 T>
    GpByteWriterRef<T, GpByteWriter>
                            Ref             (void);

    inline GpByteWriter&    UI8             (const u_int_8  aValue);
    inline GpByteWriter&    SI8             (const s_int_8  aValue);
    inline GpByteWriter&    UI16            (const u_int_16 aValue);
    inline GpByteWriter&    SI16            (const s_int_16 aValue);
    inline GpByteWriter&    UI32            (const u_int_32 aValue);
    inline GpByteWriter&    SI32            (const s_int_32 aValue);
    inline GpByteWriter&    UI64            (const u_int_64 aValue);
    inline GpByteWriter&    SI64            (const s_int_64 aValue);
    GpByteWriter&           BytesWithLen    (GpSpanByteR aData);
    inline GpByteWriter&    Bytes           (GpSpanByteR aData);
    inline GpByteWriter&    Bytes           (std::string_view aData);
    inline GpByteWriter&    Bytes           (const std::string& aData);
    inline GpByteWriter&    Bytes           (const void*    aPtr,
                                             const size_t   aSize);
    GpByteWriter&           CompactUI64     (const u_int_64 aValue);
    GpByteWriter&           CompactSI64     (const s_int_64 aValue);

private:
    template<typename T>
    void                    WritePOD        (const T aValue)
    {
        std::remove_const_t<T> val;
        val = BitOps::H2N(aValue);
        Bytes({reinterpret_cast<const std::byte*>(&val), sizeof(T)});
    }

private:
    GpByteWriterStorage&    iStorage;
};

GpByteWriter::GpByteWriter (GpByteWriterStorage& aStorage) noexcept:
iStorage(aStorage)
{
}

GpSpanByteRW    GpByteWriter::OffsetAdd (const size_t aOffset)
{
    if (aOffset == 0) [[unlikely]]
    {
        return StoragePtr();
    }

    return iStorage.OffsetAdd(aOffset);
}

GpByteWriter&   GpByteWriter::UI8 (const u_int_8 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::SI8 (const s_int_8 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::UI16 (const u_int_16 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::SI16 (const s_int_16 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::UI32 (const u_int_32 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::SI32 (const s_int_32 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::UI64 (const u_int_64 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::SI64 (const s_int_64 aValue)
{
    WritePOD<decltype(aValue)>(aValue);
    return *this;
}

GpByteWriter&   GpByteWriter::Bytes (GpSpanByteR aData)
{
    iStorage.Write(aData);
    return *this;
}

GpByteWriter&   GpByteWriter::Bytes (std::string_view aData)
{
    return Bytes(GpSpanByteR(std::data(aData), std::size(aData)));
}

GpByteWriter&   GpByteWriter::Bytes (const std::string& aData)
{
    return Bytes(GpSpanByteR(std::data(aData), std::size(aData)));
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

GpSpanByteRW    GpByteWriter::StoragePtr (void) noexcept
{
    return iStorage.StoragePtr();
}

size_t  GpByteWriter::SizeLeft (void) const noexcept
{
    return iStorage.SizeLeft();
}

size_t  GpByteWriter::TotalWrite (void) const noexcept
{
    return iStorage.TotalWrite();
}

void    GpByteWriter::OnEnd (void)
{
    iStorage.OnEnd();
}

void    GpByteWriter::ReserveNext (const size_t aSize)
{
    iStorage.ReserveNext(aSize);
}

template<Concepts::IsIntegralUpTo64 T>
GpByteWriterRef<T, GpByteWriter>
GpByteWriter::Ref (void)
{
    auto res = GpByteWriterRef<T, GpByteWriter>(TotalWrite(), *this);
    OffsetAdd(sizeof(T));
    return res;
}

}// namespace GPlatform
