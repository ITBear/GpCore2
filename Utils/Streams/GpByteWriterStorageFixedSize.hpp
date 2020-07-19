#pragma once

#include "GpByteWriterStorage.hpp"

namespace GPlatform {

class GPCORE_API GpByteWriterStorageFixedSize final: public GpByteWriterStorage
{
    CLASS_REMOVE_CTRS(GpByteWriterStorageFixedSize)

public:
    inline                  GpByteWriterStorageFixedSize    (GpRawPtrByteRW aDataOut) noexcept;
    virtual                 ~GpByteWriterStorageFixedSize   (void) noexcept override final;
};

GpByteWriterStorageFixedSize::GpByteWriterStorageFixedSize (GpRawPtrByteRW aDataOut) noexcept:
GpByteWriterStorage(aDataOut)
{
}

}//GPlatform
