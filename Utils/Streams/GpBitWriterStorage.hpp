#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

class GPCORE_API GpBitWriterStorage
{
    CLASS_REMOVE_CTRS(GpBitWriterStorage)

protected:
    inline                  GpBitWriterStorage  (GpRawPtrByteRW     aDataOut,
                                                 const size_bit_t   aOffset = 0_bit) noexcept;

public:
    virtual                 ~GpBitWriterStorage (void) noexcept;

    const std::byte*        Data                (void) const noexcept {return iData;}
    std::byte*              Data                (void) noexcept {return iData;}
    size_bit_t              Size                (void) const noexcept {return iSize;}
    size_bit_t              Offset              (void) const noexcept {return iOffset;}
    size_bit_t              Left                (void) const noexcept {return iLeft;}
    void                    SetLeftSub          (const size_bit_t aValue) {iLeft -= aValue;}

    virtual void            AllocateNext        (const size_bit_t aSize);

protected:
    void                    SetData             (std::byte*         aData) noexcept {iData = aData;}
    void                    SetSize             (const size_bit_t   aValue)noexcept {iSize = aValue;}
    void                    SetLeft             (const size_bit_t   aValue)noexcept {iLeft = aValue;}

private:
    std::byte*              iData = nullptr;
    size_bit_t              iSize;
    const size_bit_t        iOffset;
    size_bit_t              iLeft;
};

GpBitWriterStorage::GpBitWriterStorage (GpRawPtrByteRW      aDataOut,
                                        const size_bit_t    aOffset) noexcept:
iData(aDataOut.Ptr()),
iSize(aDataOut.CountLeftV<size_byte_t>()),
iOffset(aOffset),
iLeft(iSize)
{
}

}//GPlatform
