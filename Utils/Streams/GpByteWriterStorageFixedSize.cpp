#include "GpByteWriterStorageFixedSize.hpp"

namespace GPlatform {

GpByteWriterStorageFixedSize::~GpByteWriterStorageFixedSize (void) noexcept
{
}

void    GpByteWriterStorageFixedSize::AllocateNext (const size_byte_t aSize)
{
    if (Left() < aSize)
    {
        THROW_GPE("Out of range"_sv);
    }
}

}//namespace GPlatform
