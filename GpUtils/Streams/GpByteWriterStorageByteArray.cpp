#include "GpByteWriterStorageByteArray.hpp"

namespace GPlatform {

void    GpByteWriterStorageByteArray::AllocateAdd
(
    const size_t        aSizeToAdd,
    GpSpanPtrByteRW&    aStoragePtr
)
{
    const size_t usedSize       = size_t(aStoragePtr.Ptr() - iStorage.data());
    const size_t freeSize       = aStoragePtr.Count();
    const size_t newFreeSize    = NumOps::SAdd(freeSize, aSizeToAdd);
    const size_t newStorageSize = NumOps::SAdd(usedSize, newFreeSize);

    if (newStorageSize > iStorage.size())
    {
        iStorage.resize(newStorageSize);
    }

    aStoragePtr.Set(iStorage.data() + usedSize, newFreeSize);
}

void    GpByteWriterStorageByteArray::_OnEnd (void)
{
    iStorage.resize(TotalWrite());
}

}// namespace GPlatform
