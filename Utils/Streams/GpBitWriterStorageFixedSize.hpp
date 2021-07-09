#pragma once

#include "GpBitWriterStorage.hpp"

namespace GPlatform {

class GPCORE_API GpBitWriterStorageFixedSize final: public GpBitWriterStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBitWriterStorageFixedSize)

public:
    inline                  GpBitWriterStorageFixedSize     (GpRawPtrByteRW     aDataOut,
                                                             const size_bit_t   aOffset = 0_bit) noexcept;
    virtual                 ~GpBitWriterStorageFixedSize    (void) noexcept override final;
};

GpBitWriterStorageFixedSize::GpBitWriterStorageFixedSize (GpRawPtrByteRW    aDataOut,
                                                          const size_bit_t  aOffset) noexcept:
GpBitWriterStorage(aDataOut, aOffset)
{
}

}//GPlatform
