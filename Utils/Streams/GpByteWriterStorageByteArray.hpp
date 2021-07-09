#pragma once

#include "GpByteWriterStorage.hpp"

namespace GPlatform {

class GPCORE_API GpByteWriterStorageByteArray final: public GpByteWriterStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteWriterStorageByteArray)

public:
    inline                  GpByteWriterStorageByteArray    (GpBytesArray& aOut) noexcept;
    virtual                 ~GpByteWriterStorageByteArray   (void) noexcept override final;

    virtual void            AllocateNext                    (const size_byte_t aSize) override final;

private:
    GpBytesArray&           iOut;
};

GpByteWriterStorageByteArray::GpByteWriterStorageByteArray (GpBytesArray& aOut) noexcept:
GpByteWriterStorage(GpRawPtrByteRW(aOut)),
iOut(aOut)
{
}

}//GPlatform
