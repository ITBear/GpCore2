#include "GpByteReaderStorage.hpp"

namespace GPlatform {

GpRawPtrByteR   GpByteReaderStorage::ReadAndShift (const size_byte_t aSize)
{
    const count_t   size    = aSize.As<count_t>();
    GpRawPtrByteR   res     = iData.Subrange(0_cnt, size);
    iData.OffsetAdd(size);

    return res;
}

GpRawPtrByteR   GpByteReaderStorage::TryReadAndShift (const size_byte_t aSize)
{
    const count_t sizeLeftToRead    = std::min(aSize.As<count_t>(), iData.CountLeft());
    GpRawPtrByteR res               = iData.Subrange(0_cnt, sizeLeftToRead);
    iData.OffsetAdd(sizeLeftToRead);

    return res;
}

}//GPlatform
