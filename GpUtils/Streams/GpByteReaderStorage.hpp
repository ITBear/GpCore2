#pragma once

#include "../GpUtils_global.hpp"
#include "../Types/Containers/GpBytesArray.hpp"

namespace GPlatform {

class GP_UTILS_API GpByteReaderStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteReaderStorage)

public:
    inline                  GpByteReaderStorage     (GpSpanByteR aStoragePtr) noexcept;
                            ~GpByteReaderStorage    (void) noexcept = default;

    inline GpSpanByteR      StoragePtr              (void) const noexcept;
    inline size_t           SizeLeft                (void) const noexcept;
    inline size_t           TotalRead               (void) const noexcept;
    inline GpSpanByteR      Read                    (const size_t aSize);

protected:
    GpSpanByteR             iStoragePtr;
    size_t                  iTotalRead = 0;
};

GpByteReaderStorage::GpByteReaderStorage (GpSpanByteR aStoragePtr) noexcept:
iStoragePtr(aStoragePtr)
{
}

GpSpanByteR GpByteReaderStorage::StoragePtr (void) const noexcept
{
    return iStoragePtr;
}

size_t  GpByteReaderStorage::SizeLeft (void) const noexcept
{
    return iStoragePtr.Count();
}

inline size_t   GpByteReaderStorage::TotalRead (void) const noexcept
{
    return iTotalRead;
}

GpSpanByteR GpByteReaderStorage::Read (const size_t aSize)
{
    GpSpanByteR res = iStoragePtr.SubspanBegin(0, aSize);
    iStoragePtr.OffsetAdd(aSize);

    iTotalRead = NumOps::SAdd(iTotalRead, aSize);

    return res;
}

}// namespace GPlatform
