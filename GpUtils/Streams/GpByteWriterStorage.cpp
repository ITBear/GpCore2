#include "GpByteWriterStorage.hpp"

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

GpSpanByteRW    GpByteWriterStorage::OffsetAdd (const size_t aOffset)
{
    ReserveNext(aOffset);

    GpSpanByteRW storagePtrBeforeOffset = iStoragePtr;
    iStoragePtr.OffsetAdd(aOffset);
    iTotalWrite = NumOps::SAdd(iTotalWrite, aOffset);

    return storagePtrBeforeOffset;
}

}// namespace GPlatform
