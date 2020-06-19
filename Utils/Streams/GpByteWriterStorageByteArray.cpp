#include "GpByteWriterStorageByteArray.hpp"

namespace GPlatform {

GpByteWriterStorageByteArray::~GpByteWriterStorageByteArray (void) noexcept
{
}

void    GpByteWriterStorageByteArray::AllocateNext (const size_byte_t aSize)
{
    const size_byte_t left = Left();

    if (left >= aSize)
    {
        return;
    }

    const size_byte_t delta     = aSize - left;
    const size_byte_t newSize   = size_byte_t::SMake(iOut.size()) + delta;

    iOut.resize(newSize.Value());

    SetSize(newSize);
    SetLeft(left + delta);
    SetData(iOut.data() + (newSize - Left()).ValueAs<size_t>());
}

}//namespace GPlatform
