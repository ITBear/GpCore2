#pragma once

#include "../Types/Containers/GpBytesArray.hpp"

namespace GPlatform {

class GP_UTILS_API GpBitWriterStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpBitWriterStorage)

protected:
    inline                  GpBitWriterStorage  (GpSpanPtrByteRW    aDataOut,
                                                 const size_bit_t   aOffset = 0_bit) noexcept;

public:
    virtual                 ~GpBitWriterStorage (void) noexcept;

    const u_int_8*          Data                (void) const noexcept {return iData;}
    u_int_8*                Data                (void) noexcept {return iData;}
    size_bit_t              Size                (void) const noexcept {return iSize;}
    size_bit_t              Offset              (void) const noexcept {return iOffset;}
    size_bit_t              Left                (void) const noexcept {return iLeft;}
    void                    SetLeftSub          (const size_bit_t aValue) {iLeft -= aValue;}

    virtual void            AllocateNext        (const size_bit_t aSize);

protected:
    void                    SetData             (u_int_8*           aData) noexcept {iData = aData;}
    void                    SetSize             (const size_bit_t   aValue)noexcept {iSize = aValue;}
    void                    SetLeft             (const size_bit_t   aValue)noexcept {iLeft = aValue;}

private:
    u_int_8*                iData = nullptr;
    size_bit_t              iSize;
    const size_bit_t        iOffset;
    size_bit_t              iLeft;
};

GpBitWriterStorage::GpBitWriterStorage
(
    GpSpanPtrByteRW     aDataOut,
    const size_bit_t    aOffset
) noexcept:
iData(aDataOut.Ptr()),
iSize(aDataOut.Size()),
iOffset(aOffset),
iLeft(iSize)
{
}

}//GPlatform
