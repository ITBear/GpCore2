#include "GpByteWriterStorage.hpp"

namespace GPlatform {

GpByteWriterStorage::~GpByteWriterStorage (void) noexcept
{
}

void    GpByteWriterStorage::WriteAndShift (GpRawPtrByteR aData)
{
    iDataOut.CopyFrom(aData);
    iDataOut.OffsetAdd(aData.CountLeft());
}

void    GpByteWriterStorage::OffsetAdd (const size_byte_t aOffset)
{
    iDataOut.OffsetAdd(aOffset.As<count_t>());
}

void    GpByteWriterStorage::OffsetToEnd (void)
{
    iDataOut.OffsetAdd(iDataOut.CountLeft());
}

void    GpByteWriterStorage::AllocateNext (const size_byte_t aSize)
{
    THROW_GPE_COND
    (
        iDataOut.SizeLeft() >= aSize,
        "Out of range"_sv
    );
}

}//GPlatform
