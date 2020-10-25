#include "GpByteWriterStorageByteArray.hpp"

namespace GPlatform {

GpByteWriterStorageByteArray::~GpByteWriterStorageByteArray (void) noexcept
{
}

void    GpByteWriterStorageByteArray::AllocateNext (const size_byte_t aSize)
{
    const count_t sizeToWrite   = aSize.As<count_t>();
    const count_t left          = iDataOut.CountLeft();

    if (left >= sizeToWrite)
    {
        return;
    }

    const count_t used          = iDataOut.Offset();
    const count_t deltaToAdd    = sizeToWrite - left;
    const count_t newSize       = count_t::SMake(iOut.size()) + deltaToAdd;

    iOut.resize(newSize.As<size_t>());
    iDataOut.Set(iOut.data(), newSize, used);
}

}//namespace GPlatform
