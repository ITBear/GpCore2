#include "GpByteReaderStorage.hpp"

namespace GPlatform {

GpRawPtrByteR   GpByteReaderStorage::ReadAndShift (const size_byte_t aSize)
{
    const count_t size = aSize.ValueAs<count_t>();

    GpRawPtrByteR res = iData.Subrange(0_cnt, size);
    iData.OffsetAdd(size);
    return res;
}

}//GPlatform
