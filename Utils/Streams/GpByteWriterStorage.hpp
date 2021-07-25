#pragma once

#include "../../Types/GpTypes.hpp"

namespace GPlatform {

class GPCORE_API GpByteWriterStorage
{
    CLASS_REMOVE_CTRS_DEFAULT_MOVE_COPY(GpByteWriterStorage)

protected:
    inline                  GpByteWriterStorage     (GpRawPtrByteRW aDataOut) noexcept;

public:
    virtual                 ~GpByteWriterStorage    (void) noexcept;

    GpRawPtrByteRW          DataOut                 (void) noexcept {return iDataOut;}
    size_byte_t             SizeLeft                (void) const noexcept {return iDataOut.SizeLeft();}
    void                    WriteAndShift           (GpRawPtrByteR aData);
    void                    OffsetAdd               (const size_byte_t aOffset);
    void                    OffsetToEnd             (void);

    virtual void            AllocateNext            (const size_byte_t aSize);

protected:
    GpRawPtrByteRW          iDataOut;
};

GpByteWriterStorage::GpByteWriterStorage (GpRawPtrByteRW aDataOut) noexcept:
iDataOut(aDataOut)
{
}

}//GPlatform
