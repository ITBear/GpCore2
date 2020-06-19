#pragma once

#include "GpBitWriterStorage.hpp"

namespace GPlatform {

class GPCORE_API GpBitWriterStorageByteArray final: public GpBitWriterStorage
{
    CLASS_REMOVE_CTRS(GpBitWriterStorageByteArray);

public:
    inline                  GpBitWriterStorageByteArray     (GpBytesArray&      aOut) noexcept;
    inline                  GpBitWriterStorageByteArray     (GpBytesArray&      aOut,
                                                             const size_bit_t   aOffset) noexcept;
    virtual                 ~GpBitWriterStorageByteArray    (void) noexcept override final;

    virtual void            AllocateNext                    (const size_bit_t aSize) override final;

private:
    GpBytesArray&           iOut;
};

GpBitWriterStorageByteArray::GpBitWriterStorageByteArray (GpBytesArray& aOut) noexcept:
GpBitWriterStorageByteArray(aOut, 0_bit)
{
}

GpBitWriterStorageByteArray::GpBitWriterStorageByteArray (GpBytesArray&     aOut,
                                                          const size_bit_t  aOffset) noexcept:
GpBitWriterStorage(aOut.data(), size_byte_t::SMake(aOut.size()), aOffset),
iOut(aOut)
{
}

}//GPlatform
