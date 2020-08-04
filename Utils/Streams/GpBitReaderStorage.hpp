#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

class GPCORE_API GpBitReaderStorage
{
    CLASS_REMOVE_CTRS(GpBitReaderStorage)

public:
    inline                  GpBitReaderStorage  (GpRawPtrByteR      aData,
                                                 const size_bit_t   aOffset = 0_bit) noexcept;

                            ~GpBitReaderStorage (void) noexcept {}

    const std::byte*        Data                (void) const noexcept {return iData;}
    size_bit_t              Size                (void) const noexcept {return iSize;}
    size_bit_t              Offset              (void) const noexcept {return iOffset;}
    size_bit_t              Left                (void) const noexcept {return iLeft;}
    void                    SetLeftSub          (const size_bit_t aValue) {iLeft -= aValue;}

private:
    const std::byte*        iData = nullptr;
    const size_bit_t        iSize;
    const size_bit_t        iOffset;
    size_bit_t              iLeft;
};

GpBitReaderStorage::GpBitReaderStorage (GpRawPtrByteR       aData,
                                        const size_bit_t    aOffset) noexcept:
iData(aData.Ptr()),
iSize(aData.CountLeftV<size_byte_t>()),
iOffset(aOffset),
iLeft(iSize)
{
}

}//GPlatform
