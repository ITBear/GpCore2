#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

class GPCORE_API GpByteReaderStorage
{
    CLASS_REMOVE_CTRS(GpByteReaderStorage)

public:
    inline                  GpByteReaderStorage     (GpRawPtrByteR aData) noexcept;
                            ~GpByteReaderStorage    (void) noexcept {}

    GpRawPtrByteR           ReadAndShift            (const size_byte_t aSize);

protected:
    GpRawPtrByteR           iData;
};

GpByteReaderStorage::GpByteReaderStorage (GpRawPtrByteR aData) noexcept:
iData(aData)
{
}

}//GPlatform
