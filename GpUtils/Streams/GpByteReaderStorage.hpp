#pragma once

#include "../Types/Containers/GpBytesArray.hpp"

namespace GPlatform {

class GP_UTILS_API GpByteReaderStorage
{
public:
    inline                      GpByteReaderStorage     (GpSpanPtrByteR aStoragePtr) noexcept;
    inline                      GpByteReaderStorage     (const GpByteReaderStorage& aStorage) noexcept;
    inline                      GpByteReaderStorage     (GpByteReaderStorage&& aStorage) noexcept;
                                ~GpByteReaderStorage    (void) noexcept = default;

    inline GpSpanPtrByteR       StoragePtr              (void) const noexcept;
    inline size_t               SizeLeft                (void) const noexcept;
    inline GpSpanPtrByteR       Read                    (const size_t aSize);
    inline GpSpanPtrByteR       TryRead                 (const size_t aSize);

    inline GpByteReaderStorage& operator=               (const GpByteReaderStorage& aStorage) noexcept;
    inline GpByteReaderStorage& operator=               (GpByteReaderStorage&& aStorage) noexcept;


protected:
    GpSpanPtrByteR              iStoragePtr;
};

GpByteReaderStorage::GpByteReaderStorage (GpSpanPtrByteR aStoragePtr) noexcept:
iStoragePtr(aStoragePtr)
{
}

GpByteReaderStorage::GpByteReaderStorage (const GpByteReaderStorage& aStorage) noexcept:
iStoragePtr(aStorage.iStoragePtr)
{
}

GpByteReaderStorage::GpByteReaderStorage (GpByteReaderStorage&& aStorage) noexcept:
iStoragePtr(std::move(aStorage.iStoragePtr))
{
}

GpSpanPtrByteR  GpByteReaderStorage::StoragePtr (void) const noexcept
{
    return iStoragePtr;
}


size_t  GpByteReaderStorage::SizeLeft (void) const noexcept
{
    return iStoragePtr.Count();
}

GpSpanPtrByteR  GpByteReaderStorage::Read (const size_t aSize)
{
    GpSpanPtrByteR res = iStoragePtr.SubspanBegin(0, aSize);
    iStoragePtr.OffsetAdd(aSize);

    return res;
}

/*GpSpanPtrByteR    GpByteReaderStorage::TryRead (const size_t aSize)
{
    const size_t sizeLeftToRead = std::min(aSize, iStoragePtr.Count());
    GpSpanPtrByteR res          = iStoragePtr.SubspanBegin(0, sizeLeftToRead);
    iStoragePtr.OffsetAdd(sizeLeftToRead);

    return res;
}*/

GpByteReaderStorage&    GpByteReaderStorage::operator= (const GpByteReaderStorage& aStorage) noexcept
{
    iStoragePtr = aStorage.iStoragePtr;
    return *this;
}

GpByteReaderStorage&    GpByteReaderStorage::operator= (GpByteReaderStorage&& aStorage) noexcept
{
    iStoragePtr = std::move(aStorage.iStoragePtr);
    return *this;
}

}//GPlatform
