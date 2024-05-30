#include "GpByteWriterStorageByteArray.hpp"

namespace GPlatform {

void    GpByteWriterStorageByteArray::AllocateAdd
(
    const size_t    aSizeToAdd,
    GpSpanByteRW&   aStoragePtr
)
{
    const size_t usedSize       = size_t(aStoragePtr.Ptr() - reinterpret_cast<const std::byte*>(std::data(iStorage)));
    const size_t freeSize       = aStoragePtr.Count();
    const size_t newFreeSize    = NumOps::SAdd(freeSize, aSizeToAdd);
    const size_t newStorageSize = NumOps::SAdd(usedSize, newFreeSize);

    if (newStorageSize > std::size(iStorage))
    {
        iStorage.resize(newStorageSize);
    }

    aStoragePtr.Set(std::data(iStorage) + usedSize, newFreeSize);
}

void    GpByteWriterStorageByteArray::_OnEnd (void)
{
    iStorage.resize(TotalWrite());
}

}// namespace GPlatform
