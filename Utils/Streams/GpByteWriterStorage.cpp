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

void    GpByteWriterStorage::AllocateNext (const size_byte_t aSize)
{
    THROW_GPE_COND_CHECK_M(iDataOut.SizeLeft() >= aSize, "Out of range"_sv);
}

}//GPlatform
