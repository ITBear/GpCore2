#include <GpCore2/GpUtils/Streams/GpByteWriterStorage.hpp>

namespace GPlatform {

void    GpByteWriterStorage::Write
(
    const void*     aPtr,
    const size_t    aSize
)
{
    ReserveNext(aSize);

    std::memcpy(iStoragePtr.Ptr(), aPtr, aSize);
    iStoragePtr.OffsetAdd(aSize);

    iTotalWrite = NumOps::SAdd(iTotalWrite, aSize);
}

GpSpanByteRW    GpByteWriterStorage::SubspanThenOffsetAdd (const size_t aOffset)
{
    ReserveNext(aOffset);

    GpSpanByteRW subspan = iStoragePtr.SubspanThenOffsetAdd(aOffset);
    iTotalWrite = NumOps::SAdd(iTotalWrite, aOffset);

    return subspan;
}

}// namespace GPlatform
