#pragma once

/*
#include "../Types/Containers/GpBytesArray.hpp"

namespace GPlatform {

class GpBitReaderStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_COPY(GpBitReaderStorage)

public:
    inline                  GpBitReaderStorage  (GpBitReaderStorage&& aStorage) noexcept;
    inline                  GpBitReaderStorage  (GpSpanPtrByteR     aData,
                                                 const size_bit_t   aOffset = 0_bit);

                            ~GpBitReaderStorage (void) noexcept {}

    const u_int_8*          Data                (void) const noexcept {return iData;}
    size_bit_t              Size                (void) const noexcept {return iSize;}
    size_bit_t              Left                (void) const noexcept {return iLeft;}
    void                    SetLeftSub          (const size_bit_t aValue) {iLeft -= aValue;}

private:
    const u_int_8*          iData = nullptr;
    const size_bit_t        iSize;
    size_bit_t              iLeft;
};

GpBitReaderStorage::GpBitReaderStorage (GpBitReaderStorage&& aStorage) noexcept:
iData(std::move(aStorage.iData)),
iSize(std::move(aStorage.iSize)),
iLeft(std::move(aStorage.iLeft))
{
}

GpBitReaderStorage::GpBitReaderStorage
(
    GpSpanPtrByteR      aData,
    const size_bit_t    aOffset
):
iData(aData.Ptr()),
iSize(aData.Size()),
iLeft(iSize - aOffset)
{
}

}//GPlatform
*/
