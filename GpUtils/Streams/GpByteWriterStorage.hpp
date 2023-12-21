#pragma once

#include "../Types/Containers/GpBytesArray.hpp"

namespace GPlatform {

class GP_UTILS_API GpByteWriterStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteWriterStorage)

protected:
    inline                  GpByteWriterStorage     (GpSpanPtrByteRW aStoragePtr) noexcept;

public:
    virtual                 ~GpByteWriterStorage    (void) noexcept = default;

    inline GpSpanPtrByteRW  StoragePtr              (void) const noexcept;
    inline size_t           SizeLeft                (void) const noexcept;
    inline size_t           TotalWrite              (void) const noexcept;
    inline void             OnEnd                   (void);
    inline void             ReserveNext             (const size_t aSize);
    inline void             Write                   (GpSpanPtrByteR aData);
    void                    Write                   (const void*    aPtr,
                                                     const size_t   aSize);

    /**
     * @brief Offset - make iData offset (allocate memory if need)
     * @param aOffset - size in bytes to `skip`
     * @return - SpanPtr to memory before offset
     */
    GpSpanPtrByteRW         Offset                  (const size_t aOffset);
    inline void             OffsetToEnd             (void);

protected:
    virtual void            AllocateAdd             (const size_t       aSizeToAdd,
                                                     GpSpanPtrByteRW&   aStoragePtr) = 0;
    virtual void            _OnEnd                  (void) = 0;

private:
    GpSpanPtrByteRW         iStoragePtr;
    size_t                  iTotalWrite = 0;
};

GpByteWriterStorage::GpByteWriterStorage (GpSpanPtrByteRW aStoragePtr) noexcept:
iStoragePtr(aStoragePtr)
{
}

GpSpanPtrByteRW GpByteWriterStorage::StoragePtr (void) const noexcept
{
    return iStoragePtr;
}

size_t  GpByteWriterStorage::SizeLeft (void) const noexcept
{
    return iStoragePtr.Count();
}

size_t  GpByteWriterStorage::TotalWrite (void) const noexcept
{
    return iTotalWrite;
}

void    GpByteWriterStorage::OnEnd (void)
{
    _OnEnd();
}

void    GpByteWriterStorage::ReserveNext (const size_t aSize)
{
    const size_t sizeLeft = SizeLeft();

    if (sizeLeft < aSize) [[unlikely]]
    {
        AllocateAdd(aSize - sizeLeft, iStoragePtr);
    }
}

void    GpByteWriterStorage::Write (GpSpanPtrByteR aData)
{
    Write(aData.Ptr(), aData.Count());
}

void    GpByteWriterStorage::OffsetToEnd (void)
{
    iStoragePtr.OffsetAdd(iStoragePtr.Count());
}

}// namespace GPlatform
