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
    GpRawPtrByteR           TryReadAndShift         (const size_byte_t aSize);
    size_byte_t             SizeLeft                (void) const noexcept {return iData.SizeLeft();}

protected:
    GpRawPtrByteR           iData;
};

GpByteReaderStorage::GpByteReaderStorage (GpRawPtrByteR aData) noexcept:
iData(aData)
{
}

}//GPlatform
