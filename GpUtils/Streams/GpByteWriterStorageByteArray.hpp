#pragma once

#include "GpByteWriterStorage.hpp"

namespace GPlatform {

class GP_UTILS_API GpByteWriterStorageByteArray final: public GpByteWriterStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteWriterStorageByteArray)

public:
    inline                  GpByteWriterStorageByteArray    (GpBytesArray& aStorage) noexcept;
    virtual                 ~GpByteWriterStorageByteArray   (void) noexcept override final = default;

protected:
    virtual void            AllocateAdd                     (const size_t       aSizeToAdd,
                                                             GpSpanPtrByteRW&   aStoragePtr) override final;
    virtual void            OnShrinkToFit                   (void) override final;

private:
    GpBytesArray&           iStorage;
};

GpByteWriterStorageByteArray::GpByteWriterStorageByteArray (GpBytesArray& aStorage) noexcept:
GpByteWriterStorage(GpSpanPtrByteRW(aStorage)),
iStorage(aStorage)
{
}

}//GPlatform
