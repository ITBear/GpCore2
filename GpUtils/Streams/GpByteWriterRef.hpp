#pragma once

#include <GpCore2/GpUtils/Types/Containers/GpByteArray.hpp>
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
                        GpByteWriterRef     (void) noexcept = default;
                        GpByteWriterRef     (const GpByteWriterRef& aRef) noexcept;
                        GpByteWriterRef     (size_t     aOffset,
                                             WriterT&   aWriter) noexcept;
                        ~GpByteWriterRef    (void) noexcept = default;

    GpByteWriterRef&    operator=           (const GpByteWriterRef& aRef) noexcept;

    void                Write               (T aValue);

    size_t              Offset              (void) const noexcept {return iOffset;}
    const WriterT*      Writer              (void) const noexcept {return iWriter;}
    WriterT*            Writer              (void) noexcept {return iWriter;}

private:
    size_t      iOffset = 0;
    WriterT*    iWriter = nullptr;
};

template<typename T,
         typename WriterT>
requires Concepts::IsIntegralUpTo64<T>
GpByteWriterRef<T, WriterT>::GpByteWriterRef (const GpByteWriterRef& aRef) noexcept:
iOffset{aRef.iOffset},
iWriter{aRef.iWriter}
{
}

template<typename T,
         typename WriterT>
requires Concepts::IsIntegralUpTo64<T>
GpByteWriterRef<T, WriterT>::GpByteWriterRef
(
    const size_t    aOffset,
    WriterT&        aWriter
) noexcept:
iOffset{aOffset},
iWriter{&aWriter}
{
}


template<typename T,
         typename WriterT>
requires Concepts::IsIntegralUpTo64<T>
auto GpByteWriterRef<T, WriterT>::operator= (const GpByteWriterRef& aRef) noexcept -> GpByteWriterRef&
{
    iOffset = aRef.iOffset;
    iWriter = aRef.iWriter;

    return *this;
}

template<typename T,
         typename WriterT>
requires Concepts::IsIntegralUpTo64<T>
void    GpByteWriterRef<T, WriterT>::Write (const T aValue)
{
    GpSpanByteRW    storage     = iWriter->StoragePtr();
    const size_t    totalWrite  = iWriter->TotalWrite();
    void*           ptr         = storage.Ptr() - totalWrite + iOffset;
    const T         val         = BitOps::H2N(aValue);

    std::memcpy(ptr, &val, sizeof(T));
}

}// namespace GPlatform
