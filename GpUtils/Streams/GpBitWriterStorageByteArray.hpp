#pragma once

/*
#include "GpBitWriterStorage.hpp"

namespace GPlatform {

class GP_UTILS_API GpBitWriterStorageByteArray final: public GpBitWriterStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBitWriterStorageByteArray)

public:
    inline                  GpBitWriterStorageByteArray     (GpBytesArray&      aOut,
                                                             const size_bit_t   aOffset = 0_bit) noexcept;
    virtual                 ~GpBitWriterStorageByteArray    (void) noexcept override final;

    virtual void            AllocateNext                    (const size_bit_t aSize) override final;

private:
    GpBytesArray&           iOut;
};

GpBitWriterStorageByteArray::GpBitWriterStorageByteArray
(
    GpBytesArray&       aOut,
    const size_bit_t    aOffset
) noexcept:
GpBitWriterStorage(GpSpanPtrByteRW(aOut.data(), aOut.size()), aOffset),
iOut(aOut)
{
}

}//GPlatform
*/
