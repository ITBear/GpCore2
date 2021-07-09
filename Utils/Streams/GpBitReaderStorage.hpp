#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

class GPCORE_API GpBitReaderStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_COPY(GpBitReaderStorage)

public:
    inline                  GpBitReaderStorage  (GpBitReaderStorage&& aStorage) noexcept;
    inline                  GpBitReaderStorage  (GpRawPtrByteR      aData,
                                                 const size_bit_t   aOffset = 0_bit);

                            ~GpBitReaderStorage (void) noexcept {}

    const std::byte*        Data                (void) const noexcept {return iData;}
    size_bit_t              Size                (void) const noexcept {return iSize;}
    size_bit_t              Left                (void) const noexcept {return iLeft;}
    void                    SetLeftSub          (const size_bit_t aValue) {iLeft -= aValue;}

private:
    const std::byte*        iData = nullptr;
    const size_bit_t        iSize;
    size_bit_t              iLeft;
};

GpBitReaderStorage::GpBitReaderStorage (GpBitReaderStorage&& aStorage) noexcept:
iData(std::move(aStorage.iData)),
iSize(std::move(aStorage.iSize)),
iLeft(std::move(aStorage.iLeft))
{
}

GpBitReaderStorage::GpBitReaderStorage (GpRawPtrByteR       aData,
                                        const size_bit_t    aOffset):
iData(aData.Ptr()),
iSize(aData.SizeLeft()),
iLeft(iSize - aOffset)
{
}

}//GPlatform
