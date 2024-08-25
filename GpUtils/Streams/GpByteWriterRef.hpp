#pragma once

#include <GpCore2/GpUtils/Types/Containers/GpBytesArray.hpp>
#include <GpCore2/GpUtils/Types/Bits/GpBitOps.hpp>

namespace GPlatform {

template<typename T,
         typename WriterT>
requires Concepts::IsIntegralUpTo64<T>
class GpByteWriterRef
{
public:
    using this_type     = GpByteWriterRef<T, WriterT>;
    using value_type    = T;

public:
                    GpByteWriterRef     (void) = delete;
                    GpByteWriterRef     (size_t     aOffset,
                                         WriterT&   aWriter) noexcept;
                    ~GpByteWriterRef    (void) noexcept = default;

    void            Write               (T aValue);

private:
    const size_t    iOffset;
    WriterT&        iWriter;
};

template<typename T,
         typename WriterT>
requires Concepts::IsIntegralUpTo64<T>
GpByteWriterRef<T, WriterT>::GpByteWriterRef
(
    const size_t    aOffset,
    WriterT&        aWriter
) noexcept:
iOffset{aOffset},
iWriter{aWriter}
{
}

template<typename T,
         typename WriterT>
requires Concepts::IsIntegralUpTo64<T>
void    GpByteWriterRef<T, WriterT>::Write (const T aValue)
{
    GpSpanByteRW    storage     = iWriter.StoragePtr();
    const size_t    totalWrite  = iWriter.TotalWrite();
    void*           ptr         = storage.Ptr() - totalWrite + iOffset;
    const T         val         = BitOps::H2N(aValue);

    std::memcpy(ptr, &val, sizeof(T));
}

}// namespace GPlatform
